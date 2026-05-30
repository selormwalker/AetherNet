#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

namespace AetherNet {

class UDPClient {
private:
    int sockfd;
    struct sockaddr_in servaddr;

public:
    UDPClient(const std::string& ip, int port) {
#ifdef _WIN32
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            std::cerr << "Failed. Error Code : " << WSAGetLastError() << std::endl;
        }
#endif
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            std::cerr << "Socket creation failed" << std::endl;
        }

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr);
    }

    ~UDPClient() {
#ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
#else
        close(sockfd);
#endif
    }

    void send(const std::string& message) {
        sendto(sockfd, message.c_str(), message.length(), 0, 
               (const struct sockaddr*)&servaddr, sizeof(servaddr));
    }
};

} // namespace AetherNet

#endif
