#ifndef MULTICAST_RECEIVER_HPP
#define MULTICAST_RECEIVER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

namespace AetherNet {

class MulticastReceiver {
private:
    int sockfd;
    struct sockaddr_in addr;

public:
    MulticastReceiver(const std::string& group, int port) {
#ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);

        int reuse = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

        struct ip_mreq mreq;
        inet_pton(AF_INET, group.c_str(), &mreq.imr_multiaddr);
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
    }

    ~MulticastReceiver() {
#ifdef _WIN32
        closesocket(sockfd);
#else
        close(sockfd);
#endif
    }

    int receive(char* buffer, int size) {
        socklen_t addrlen = sizeof(addr);
        return recvfrom(sockfd, buffer, size, 0, (struct sockaddr*)&addr, &addrlen);
    }
};

} // namespace AetherNet

#endif
