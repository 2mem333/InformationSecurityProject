// Source.cpp (Windows, length-prefixed JSON client)
// cl /EHsc /std:c++17 Source.cpp ws2_32.lib
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "DES.h"

#pragma comment(lib, "ws2_32.lib")

const char* host = "10.208.107.155";
const char* port = "9090";

std::string DesKeyForUser(const std::string& pwd)
{
    std::string key(8, 0);
    for (size_t i = 0; i < pwd.size(); ++i)
        key[i % 8] ^= pwd[i];

    // 2) Sezar kaydýrma (printable ASCII 32..126 içinde kalsýn)
    // shift deðerini pwd uzunluðundan türetiyoruz (deterministik)
    int shift = static_cast<int>(pwd.size() % 95); // 95 printable karakter var
    for (int i = 0; i < 8; ++i)
    {
        unsigned char c = static_cast<unsigned char>(key[i]);
        c = static_cast<unsigned char>((c % 95) + 32);                 // 32..126'ya getir
        c = static_cast<unsigned char>(((c - 32 + shift) % 95) + 32);  // Sezar
        key[i] = static_cast<char>(c);
    }

    return key; // 8 karakterlik DES key
}

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

// bu base 64 encode kodu silinebilir, zaten DES içinde varr..
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

void sendPacket(std::string req)
{
    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* res = nullptr;
    if (getaddrinfo(host, port, &hints, &res) != 0) {
        std::cerr << "getaddrinfo failed\n";
        WSACleanup();
        return;
    }

    SOCKET s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s == INVALID_SOCKET) {
        std::cerr << "socket failed\n";
        freeaddrinfo(res);
        WSACleanup();
        return;
    }

    if (connect(s, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        std::cerr << "connect failed\n";
        closesocket(s);
        freeaddrinfo(res);
        WSACleanup();
        return;
    }
    freeaddrinfo(res);

    if (!send_json(s, req)) {
        std::cerr << "send_json failed\n";
        closesocket(s);
        WSACleanup();
        return;
    }

    //std::string resp;
    //if (!recv_json(s, resp)) {
    //    std::cerr << "recv_json failed\n";
    //    closesocket(s);
    //    WSACleanup();
    //    return;
    //}

    //std::cout << "Server replied: " << resp << "\n";

    closesocket(s);
    WSACleanup();
    return;
}

std::string receivePacket(std::string req)
{
    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return "";
    }

    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* res = nullptr;
    if (getaddrinfo(host, port, &hints, &res) != 0) {
        std::cerr << "getaddrinfo failed\n";
        WSACleanup();
        return "";
    }

    SOCKET s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s == INVALID_SOCKET) {
        std::cerr << "socket failed\n";
        freeaddrinfo(res);
        WSACleanup();
        return "";
    }

    if (connect(s, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        std::cerr << "connect failed\n";
        closesocket(s);
        freeaddrinfo(res);
        WSACleanup();
        return "";
    }
    freeaddrinfo(res);

    if (!send_json(s, req)) {
        std::cerr << "send_json failed\n";
        closesocket(s);
        WSACleanup();
        return "";
    }

    std::string resp;
    if (!recv_json(s, resp)) {
        std::cerr << "recv_json failed\n";
        closesocket(s);
        WSACleanup();
        return "";
    }

    closesocket(s);
    WSACleanup();
    return resp;
}

//MESAJ PAKETLERI GONDERME
void sendMessage(std::string from, std::string to, std::string msg,std::string pwd)
{
    std::string key = DesKeyForUser(pwd); 
    std::cout << key << "\n";
    std::string encMsg = simple_des::DES_EncryptBase64(msg, key);

    std::string req = std::string("{\"type\":\"SENDMESSAGE\",\"from\":\"") + json_escape(from) +
        "\",\"to\":\"" + json_escape(to) +
        "\",\"message\":\"" + json_escape(encMsg) + "\"}";

    sendPacket(req);
}
void signup(std::string username, std::string passsword, std::string path)
{
    auto img = read_file_bytes(path);
    if (img.empty()) {
        std::cerr << "Cannot read image or empty: " << path << "\n";
        return;
    }
    std::string b64 = base64_encode(img);
    std::string req = std::string("{\"type\":\"REGISTER\",\"username\":\"") + json_escape(username) +
        "\",\"image_b64\":\"" + b64 + "\"}";

    sendPacket(req);
    std::cout << "register packet sent!";

}

bool login(std::string username, std::string passsword)
{
    std::string req = ("{\"type\":\"LOGIN\",\"username\":\"") + json_escape(username) +
        "\",\"password\":\"" + json_escape(passsword) + "\"}";

    std::string resp = receivePacket(req);
    if (resp == "granted")
        return true;
    else
        return false;

}
std::string listUsers()
{
    std::string req = "{\"type\":\"LISTUSERS\"}";
    std::string resp = receivePacket(req);
    return resp;
}
std::string listUsersOnline()
{
    std::string req = "{\"type\":\"LISTONLINEUSERS\"}";
    std::string resp = receivePacket(req);
    return resp;
}

std::string refreshMailbox(std::string username, std::string pwd)
{
    std::string key = DesKeyForUser(pwd);

    std::string req = "{\"type\":\"REFRESHMAILBOX\",\"username\":\"" + json_escape(username) + "\"}";
    std::string resp = receivePacket(req);


    std::stringstream ss(resp);
    std::string token;
    std::string rebuilt;

    while (std::getline(ss, token, ',')) // virgülle parça parça al
    {
        if (token.empty()) continue;

        // ilk ':' konumu
        std::size_t pos = token.find(':');
        if (pos == std::string::npos)
        {
            // format bozuksa olduðu gibi ekle (istersen atlayabilirsin)
            if (!rebuilt.empty()) rebuilt += ",";
            rebuilt += token;
            continue;
        }

        std::string user = token.substr(0, pos);
        std::string encrypted = token.substr(pos + 1);

        // Þifreli kýsmý çöz
        std::string decoded = simple_des::DES_DecryptBase64(encrypted, key); // decode imzan farklýysa burayý deðiþtir

        // user:decoded þeklinde yeniden kur
        if (!rebuilt.empty()) rebuilt += ",";
        rebuilt += user + ":" + decoded;
    }

    resp = rebuilt;

    return resp;
}


void onlineTick(std::string username)
{
    std::string req = ("{\"type\":\"ONLINE\",\"username\":\"") + json_escape(username) +
        "\"}";
    sendPacket(req);
}
void offlineTick(std::string username)
{
    std::string req = ("{\"type\":\"OFFLINE\",\"username\":\"") + json_escape(username) +
        "\"}";
    sendPacket(req);
}


