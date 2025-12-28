// LinuxServer.cpp (Linux, multi-client, length-prefixed JSON)
// g++ -std=c++17 -O2 -Wall -pthread LinuxServer.cpp -o server

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <fstream>

#include "StegLSB.h"

// Senin alfabeten
static const std::string B64_CHARS =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static inline int b64_index(unsigned char c)
{
    // Hýzlý mapping: find ile de olur ama bu daha net
    if ('A' <= c && c <= 'Z') return c - 'A';
    if ('a' <= c && c <= 'z') return c - 'a' + 26;
    if ('0' <= c && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

std::vector<uint8_t> base64_decode(std::string s)
{
    // Eðer "data:image/png;base64,...." gibi geldiyse header kýrp
    std::size_t comma = s.find(',');
    if (comma != std::string::npos)
        s = s.substr(comma + 1);

    std::vector<uint8_t> out;
    out.reserve((s.size() * 3) / 4);

    int val = 0;
    int valb = -8;

    for (unsigned char c : s)
    {
        if (std::isspace(c))
            continue;

        if (c == '=')
            break;

        int idx = b64_index(c);
        if (idx < 0)
        {
            // Geçersiz karakter -> istersen return {} diyebilirsin
            return std::vector<uint8_t>();
        }

        val = (val << 6) + idx;
        valb += 6;
        if (valb >= 0)
        {
            out.push_back(static_cast<uint8_t>((val >> valb) & 0xFF));
            valb -= 8;
        }
    }

    return out;
}

bool checkPassword(std::string name, std::string password)
{
    std::ifstream file("databases/users.txt");
    if (!file.is_open())
        return false;

    std::string fileName, filePassword;

    while (file >> fileName >> filePassword)
    {
        if (fileName == name && filePassword == password)
        {
            file.close();
            return true; // doðru kullanýcý
        }
    }

    file.close();
    return false; // bulunamadý veya yanlýþ
}

bool savePicture(const std::string &name, const std::string& file64)
{
    // Basit güvenlik
    if (name.empty() || name.find("..") != std::string::npos ||
        name.find('/') != std::string::npos || name.find('\\') != std::string::npos)
        return false;

    std::vector<uint8_t> data = base64_decode(file64);
    if (data.empty())
        return false;

    // PNG magic number kontrolü (opsiyonel ama önerilir)
    if (data.size() < 8 ||
        data[0] != 0x89 || data[1] != 0x50 || data[2] != 0x4E || data[3] != 0x47 ||
        data[4] != 0x0D || data[5] != 0x0A || data[6] != 0x1A || data[7] != 0x0A)
        return false;

    std::ofstream out("pictures/" + name + ".png", std::ios::binary);
    if (!out.is_open())
        return false;

    out.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
    return out.good();
}

bool registerUser(const std::string& name)
{
    std::string filePath = "pictures/" + name + ".png";
    std::string password = fotograftanSifreyiCikar(filePath);

    if (name.empty() || password.empty())
        return false;

    if (name.find(' ') != std::string::npos ||
        password.find(' ') != std::string::npos)
        return false;

    // Kullanýcý var mý kontrol et
    {
        std::ifstream in("databases/users.txt");
        if (in.is_open())
        {
            std::string u, p;
            while (in >> u >> p)
            {
                if (u == name)
                    return false;
            }
        }
    }

    // Son karakter \n mi kontrol et
    bool endsWithNewline = false;
    {
        std::ifstream in("databases/users.txt", std::ios::binary);
        if (in.is_open())
        {
            in.seekg(0, std::ios::end);
            if (in.tellg() > 0)
            {
                in.seekg(-1, std::ios::end);
                char last;
                in.get(last);
                endsWithNewline = (last == '\n');
            }
        }
    }

    // Ekle
    std::ofstream out("databases/users.txt", std::ios::app);
    if (!out.is_open())
        return false;

    if (!endsWithNewline)
        out << "\n";   // BOÞ SATIRI AÇ

    out << name << " " << password << "\n";
    return true;
}

std::string listUsers()
{
    std::ifstream file("databases/users.txt");
    if (!file.is_open())
        return ""; // dosya açýlamadýysa boþ string

    std::string result;
    std::string username, password;

    while (file >> username >> password)
    {
        if (!result.empty())
            result += ",";   // araya virgül koy

        result += username;
    }

    file.close();
    return result;
}



static bool send_all(int fd, const void* data, size_t len) {
    const char* p = (const char*)data;
    size_t off = 0;
    while (off < len) {
        ssize_t n = ::send(fd, p + off, len - off, 0);
        if (n <= 0) return false;
        off += (size_t)n;
    }
    return true;
}

static bool recv_all(int fd, void* data, size_t len) {
    char* p = (char*)data;
    size_t off = 0;
    while (off < len) {
        ssize_t n = ::recv(fd, p + off, len - off, 0);
        if (n <= 0) return false;
        off += (size_t)n;
    }
    return true;
}

// Framing: [uint32 len][json bytes]
static bool send_json(int fd, const std::string& json) {
    uint32_t len = (uint32_t)json.size();
    uint32_t net = htonl(len);
    if (!send_all(fd, &net, 4)) return false;
    if (len > 0 && !send_all(fd, json.data(), len)) return false;
    return true;
}

static bool recv_json(int fd, std::string& json_out) {
    uint32_t net = 0;
    if (!recv_all(fd, &net, 4)) return false;

    uint32_t len = ntohl(net);
    if (len > 50u * 1024u * 1024u) return false; // 50MB limit

    json_out.resize(len);
    if (len > 0 && !recv_all(fd, &json_out[0], len)) return false;
    return true;
}

// Very small helper: extracts string field "type":"..."
// Not a full JSON parser; only for ACK message.
static bool json_get_value(const std::string& j, const std::string& key, std::string& outValue)
{
    // "key"
    std::string needle = "\"" + key + "\"";
    size_t k = j.find(needle);
    if (k == std::string::npos) return false;

    // :
    size_t colon = j.find(':', k + needle.size());
    if (colon == std::string::npos) return false;

    // boþluklarý geç
    size_t i = colon + 1;
    while (i < j.size() &&
        (j[i] == ' ' || j[i] == '\t' || j[i] == '\r' || j[i] == '\n'))
        i++;

    // string mi?
    if (i >= j.size() || j[i] != '"') return false;
    i++; // opening quote sonrasý

    std::string val;
    bool esc = false;

    for (; i < j.size(); i++) {
        char c = j[i];
        if (!esc) {
            if (c == '\\') {
                esc = true;
                continue;
            }
            if (c == '"') break; // closing quote
            val.push_back(c);
        }
        else {
            // minimal escape handling
            switch (c) {
            case 'n': val.push_back('\n'); break;
            case 'r': val.push_back('\r'); break;
            case 't': val.push_back('\t'); break;
            case '\\': val.push_back('\\'); break;
            case '"': val.push_back('"'); break;
            default: val.push_back(c); break;
            }
            esc = false;
        }
    }

    if (i >= j.size() || j[i] != '"') return false;

    outValue = val;
    return true;
}

static void handle_client(int fd, sockaddr_in addr) {
    char ip[INET_ADDRSTRLEN]{};
    ::inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
    uint16_t port = ntohs(addr.sin_port);
    std::cout << "Client connected: " << ip << ":" << port << "\n";

    while (true) {
        std::string req;
        if (!recv_json(fd, req)) break;

        // sadece logla
        std::cout << "Received JSON (" << req.size() << " bytes)\n";

        // sadece ACK dön
        std::string type;
        if (json_get_value(req, "type", type)) {

            //std::cout << "REQ: " << req << "\n";
            std::cout << "PARSED type: [" << type << "]\n";

            if (type == "REGISTER")
            {
                std::string username;
                std::string base64;

                json_get_value(req, "username", username);
                json_get_value(req, "image_b64", base64);

                savePicture(username, base64);
                registerUser(username);
            }

            else if(type == "LOGIN")
            {
                std::string username;
                std::string password;
                json_get_value(req, "username", username);
                json_get_value(req, "password", password);

                //burada veritabanýnda kontrol yapcak sonra cccevap göndericek..
                bool loginStatus = checkPassword(username, password);

                std::string resp;
                if (loginStatus) //success
                {
                    resp = "{\"type\":\"ACK\",\"access granted\":\"" + type + "\"}";
                }
                else 
                {
                    resp = "{\"type\":\"ACK\",\"access denied\":\"" + type + "\"}";
                }
                if (!send_json(fd, resp)) break;

            }
            else if(type == "LISTUSERS")
            {
                std::string resp = "{\"type\":\"ACK\",\"USERS\":\"" + listUsers() + "\"}";

                if (!send_json(fd, resp)) break;
            }

            else if (type == "REFRESHMAILBOX")
            {
                std::string resp = "{\"type\":\"ACK\",\"refreshing mailbox\":\"" + type + "\"}";

                if (!send_json(fd, resp)) break;
            }

            else if(type == "SENDMESSAGE")
            {
             
            }
        }
        else {
            // type yoksa yine ACK
            if (!send_json(fd, "{\"type\":\"ACK\"}")) break;
        }
    }

    ::close(fd);
    std::cout << "Client disconnected\n";
}

int main(int argc, char** argv) {
    const char* bind_ip = (argc >= 2) ? argv[1] : "0.0.0.0";
    int port = (argc >= 3) ? std::atoi(argv[2]) : 9090;

    int lfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (lfd < 0) {
        std::cerr << "socket failed: " << std::strerror(errno) << "\n";
        return 1;
    }

    int yes = 1;
    ::setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    if (::inet_pton(AF_INET, bind_ip, &addr.sin_addr) != 1) {
        std::cerr << "inet_pton failed for bind ip\n";
        ::close(lfd);
        return 1;
    }

    if (::bind(lfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind failed: " << std::strerror(errno) << "\n";
        ::close(lfd);
        return 1;
    }

    if (::listen(lfd, 256) < 0) {
        std::cerr << "listen failed: " << std::strerror(errno) << "\n";
        ::close(lfd);
        return 1;
    }

    std::cout << "Server listening on " << bind_ip << ":" << port << "\n";

    while (true) {
        sockaddr_in caddr{};
        socklen_t clen = sizeof(caddr);
        int cfd = ::accept(lfd, (sockaddr*)&caddr, &clen);
        if (cfd < 0) {
            std::cerr << "accept failed: " << std::strerror(errno) << "\n";
            continue;
        }
        std::thread([cfd, caddr]() { handle_client(cfd, caddr); }).detach();
    }

    ::close(lfd);
    return 0;
}
