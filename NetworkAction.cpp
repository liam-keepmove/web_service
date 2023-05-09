/*
* author:ajin
* create time:4/27/2022 16:03:00
*/

#include "NetworkAction.hpp"

int openListenFd(char const* serverName) {
    struct addrinfo hints = {
            .ai_flags=AI_ADDRCONFIG | AI_PASSIVE,  //AI_PASSIVE 作为服务器套接字(被动套接字),此时host应该是nullptr.
            .ai_family=AF_INET,  //socket类型,AF_INET是ipv4
            .ai_socktype=SOCK_STREAM,
    };
    struct addrinfo* result = nullptr;
    int listenFd = 0;
    Getaddrinfo(nullptr, serverName, &hints, &result);

    for (struct addrinfo* p = result; p != nullptr; p = p->ai_next) {
        listenFd = 0;
        try {
            listenFd = Socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            Bind(listenFd, p->ai_addr, p->ai_addrlen);
            Listen(listenFd, 1024);
        } catch (...) {
            if (listenFd > 0)
                Close(listenFd);
            continue;
        }
        break;
    }
    freeaddrinfo(result);
    return listenFd;
}


int openClientFd(char const* hostName, char const* serverName) {
    struct addrinfo hints = {
            .ai_flags = AI_PASSIVE | AI_ADDRCONFIG, /* ... on any IP address , using port number*/
            .ai_socktype = SOCK_STREAM             /* Accept connections */
    };
    struct addrinfo* listPtr = nullptr;
    struct addrinfo* p = nullptr;
    int clientFd = 0;

    /* Get a list of potential server addresses */
    Getaddrinfo(hostName, serverName, &hints, &listPtr);

    /* Walk the list for one that we can bind to */
    for (p = listPtr; p != nullptr; p = p->ai_next) {
        clientFd = 0;
        try {
            clientFd = Socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            Connect(clientFd, p->ai_addr, p->ai_addrlen);
        } catch (...) {
            if (clientFd > 0)
                Close(clientFd);
            continue;
        }
        return clientFd;
    }
    throw std::runtime_error("打开客户端网络套接字失败.");
}
