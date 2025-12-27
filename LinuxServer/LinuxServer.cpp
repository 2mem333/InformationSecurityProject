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

bool checkPassword(std::string name, std::string password)
{

}

bool registerTheUser(std::string name)
{

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

            std::cout << "REQ: " << req << "\n";
            std::cout << "PARSED type: [" << type << "]\n";

            if (type == "REGISTER")
            {
               
            }
            else if(type == "LOGIN")
            {
                std::string username;
                std::string password;
                json_get_value(req, "username", username);
                json_get_value(req, "password", password);

                //burada veritabanýnda kontrol yapcak sonra cccevap göndericek..
                std::cout << "CHECKING: " << username << "  " << password << "\n";

                std::string resp = "{\"type\":\"ACK\",\"access granted\":\"" + type + "\"}";
                if (!send_json(fd, resp)) break;

            }
            else if(type == "LISTUSERS")
            {
                std::string resp = "{\"type\":\"ACK\",\"listing users\":\"" + type + "\"}";

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
