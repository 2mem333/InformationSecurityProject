// Source.cpp (Windows, length-prefixed JSON client)
// cl /EHsc /std:c++17 Source.cpp ws2_32.lib

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

static bool send_all(SOCKET s, const void* data, int len) {
    const char* p = (const char*)data;
    int off = 0;
    while (off < len) {
        int n = send(s, p + off, len - off, 0);
        if (n <= 0) return false;
        off += n;
    }
    return true;
}

static bool recv_all(SOCKET s, void* data, int len) {
    char* p = (char*)data;
    int off = 0;
    while (off < len) {
        int n = recv(s, p + off, len - off, 0);
        if (n <= 0) return false;
        off += n;
    }
    return true;
}

// Framing: [uint32 len][json bytes]
static bool send_json(SOCKET s, const std::string& json) {
    uint32_t len = (uint32_t)json.size();
    uint32_t net = htonl(len);
    if (!send_all(s, &net, 4)) return false;
    if (len > 0 && !send_all(s, json.data(), (int)len)) return false;
    return true;
}

static bool recv_json(SOCKET s, std::string& json_out) {
    uint32_t net = 0;
    if (!recv_all(s, &net, 4)) return false;
    uint32_t len = ntohl(net);
    if (len > 50u * 1024u * 1024u) return false;
    json_out.resize(len);
    if (len > 0 && !recv_all(s, &json_out[0], (int)len)) return false;
    return true;
}

static std::vector<uint8_t> read_file_bytes(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return {};
    f.seekg(0, std::ios::end);
    std::streamoff sz = f.tellg();
    if (sz < 0) return {};
    f.seekg(0, std::ios::beg);
    std::vector<uint8_t> b((size_t)sz);
    if (sz > 0) f.read((char*)b.data(), sz);
    return b;
}

// base64 encode (standard)
static const char* B64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string base64_encode(const std::vector<uint8_t>& in) {
    std::string out;
    size_t i = 0;
    while (i + 2 < in.size()) {
        uint32_t v = (in[i] << 16) | (in[i + 1] << 8) | (in[i + 2]);
        out.push_back(B64[(v >> 18) & 63]);
        out.push_back(B64[(v >> 12) & 63]);
        out.push_back(B64[(v >> 6) & 63]);
        out.push_back(B64[v & 63]);
        i += 3;
    }
    if (i < in.size()) {
        uint32_t v = (in[i] << 16);
        out.push_back(B64[(v >> 18) & 63]);
        if (i + 1 < in.size()) {
            v |= (in[i + 1] << 8);
            out.push_back(B64[(v >> 12) & 63]);
            out.push_back(B64[(v >> 6) & 63]);
            out.push_back('=');
        }
        else {
            out.push_back(B64[(v >> 12) & 63]);
            out.push_back('=');
            out.push_back('=');
        }
    }
    return out;
}

static std::string json_escape(const std::string& s) {
    std::string o;
    o.reserve(s.size() + 8);
    for (char c : s) {
        switch (c) {
        case '\\': o += "\\\\"; break;
        case '"':  o += "\\\""; break;
        case '\n': o += "\\n";  break;
        case '\r': o += "\\r";  break;
        case '\t': o += "\\t";  break;
        default:   o += c;      break;
        }
    }
    return o;
}

static void usage() {
    std::cout <<
        "Usage:\n"
        "  Source.exe <host> <port> register <username> <imagePath>\n"
        "  Source.exe <host> <port> login <username> <password>\n"
        "  Source.exe <host> <port> listusers\n"
        "  Source.exe <host> <port> sendmessage <from> <to> <message>\n";
}

int main() {

    const char* host = "172.19.35.66";
    const char* port = "9090";


    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* res = nullptr;
    if (getaddrinfo(host, port, &hints, &res) != 0) {
        std::cerr << "getaddrinfo failed\n";
        WSACleanup();
        return 1;
    }

    SOCKET s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s == INVALID_SOCKET) {
        std::cerr << "socket failed\n";
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }

    if (connect(s, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        std::cerr << "connect failed\n";
        closesocket(s);
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(res);

    std::string req;

    std::string cmd = "login";

    if (cmd == "register") {
        if (10 < 6) { usage(); closesocket(s); WSACleanup(); return 1; }

        std::string username = "bassar";
        std::string path = "file";

        auto img = read_file_bytes(path);
        if (img.empty()) {
            std::cerr << "Cannot read image or empty: " << path << "\n";
            closesocket(s); WSACleanup(); return 1;
        }
        std::string b64 = base64_encode(img);
        req = std::string("{\"type\":\"REGISTER\",\"username\":\"") + json_escape(username) +
            "\",\"image_b64\":\"" + b64 + "\"}";
    }
    else if (cmd == "login") {
        if (10 < 6) { usage(); closesocket(s); WSACleanup(); return 1; }
        req = std::string("{\"type\":\"LOGIN\",\"username\":\"") + json_escape("basar") +
            "\",\"password\":\"" + json_escape("123") + "\"}";
    }
    else if (cmd == "listusers") {
        req = "{\"type\":\"LISTUSERS\"}";
    }
    else if (cmd == "refreshmailbox") {
        req = "{\"type\":\"REFRESHMAILBOX\"}";
    }
    else if (cmd == "sendmessage") {
        if (10 < 7) { usage(); closesocket(s); WSACleanup(); return 1; }
        req = std::string("{\"type\":\"SENDMESSAGE\",\"from\":\"") + json_escape("basar") +
            "\",\"to\":\"" + json_escape("efe") +
            "\",\"message\":\"" + json_escape("msg") + "\"}";
    }
    else {
        usage();
        closesocket(s);
        WSACleanup();
        return 1;
    }

    if (!send_json(s, req)) {
        std::cerr << "send_json failed\n";
        closesocket(s);
        WSACleanup();
        return 1;
    }

    std::string resp;
    if (!recv_json(s, resp)) {
        std::cerr << "recv_json failed\n";
        closesocket(s);
        WSACleanup();
        return 1;
    }

    std::cout << "Server replied: " << resp << "\n";

    closesocket(s);
    WSACleanup();
    return 0;
}
