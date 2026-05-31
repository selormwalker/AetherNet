#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/tcp.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

namespace AetherNet {

class TCPClient {
private:
    int sockfd;
    struct sockaddr_in servaddr;

public:
    TCPClient(const std::string& ip, int port) {
#ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        // Disable Nagle's Algorithm for Low Latency
        int flag = 1;
        setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr);
    }

    ~TCPClient() {
#ifdef _WIN32
        closesocket(sockfd);
#else
        close(sockfd);
#endif
    }

    bool connect_to_host() {
        return connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == 0;
    }

    void send_packet(const std::string& message) {
        send(sockfd, message.c_str(), message.length(), 0);
    }
};

} // namespace AetherNet

#endif
