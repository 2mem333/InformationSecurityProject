#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

static bool send_all(SOCKET s, const char* data, int len) {
    int off = 0;
    while (off < len) {
        int n = send(s, data + off, len - off, 0);
        if (n <= 0) return false;
        off += n;
    }
    return true;
}

static bool recv_line(SOCKET s, std::string& out) {
    out.clear();
    out.reserve(128);

    char c;
    const size_t MAX = 4096;
    while (out.size() < MAX) {
        int n = recv(s, &c, 1, 0);
        if (n == 0) return false;   // closed
        if (n < 0) return false;    // error
        if (c == '\n') break;
        if (c != '\r') out.push_back(c);
    }
    return true;
}

void sendPacket()
{
    const char* host = "172.19.35.66";
    const char* port = "9090";

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

    const std::string msg = "Hello\n";
    if (!send_all(s, msg.c_str(), (int)msg.size())) {
        std::cerr << "send failed\n";
        closesocket(s);
        WSACleanup();
        return;
    }

    closesocket(s);
    WSACleanup();
}

void getReply()
{
    const char* host = "172.19.35.66";
    const char* port = "9090";

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

    const std::string msg = "Hello\n";
    if (!send_all(s, msg.c_str(), (int)msg.size())) {
        std::cerr << "send failed\n";
        closesocket(s);
        WSACleanup();
        return;
    }

    std::string reply;
    if (!recv_line(s, reply)) {
        std::cerr << "recv failed / connection closed\n";
        closesocket(s);
        WSACleanup();
        return;
    }

    std::cout << "Server replied: " << reply << "\n";

    closesocket(s);
    WSACleanup();
}