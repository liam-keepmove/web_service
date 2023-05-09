#pragma once
/*
* author:ajin
* create time:4/25/2022 18:23:06
*/


#ifndef TINYWEB_RIO_HPP
#define TINYWEB_RIO_HPP

#include<sys/types.h>
#include<unistd.h>
#include<cstring>
#include<cerrno>
#include<iostream>
#include"wrapper.h"

using std::string;

//The Rio class - Robust I/O
class Rio {
private:
    typedef struct {
        int fd;                /* File Descriptor for this internal buf */
        ssize_t unreadCount;         /* Unread bytes in internal buf */
        char* unreadPtr;            /* Next unread byte in internal buf */
        char buf[8192];             /* Internal buffer */
    } rio_t;
    rio_t rio{};

    //read带缓冲区的版本,read成功返回读取的字节数(<=bufLen)或者EOF返回0,发生错误抛出runtime_error异常.
    ssize_t rioRead(char* usrBuf, ssize_t n);

public:
    //不停的read,直到读满n字节或到达EOF,返回读取的字节数,EOF返回0,发生错误抛出runtime_error异常
    static ssize_t readn(int fd, void* usrBuf, ssize_t n);

    //不停的write,直到写够n字节,返回写的字节数,发生错误抛出runtime_error异常
    static ssize_t writen(int fd, void const* usrBuf, ssize_t n);

    //read的非阻塞版本,默认延时15秒,超时返回0,未超时返回read的字节数,发生错误抛出runtime_error异常.
    static ssize_t limitTimeToRead(int fd, void* usrBuf, ssize_t n, struct timeval* limit = nullptr);

    explicit Rio(int fd);

    //不停的从缓冲区中read,直到读满n字节或到达EOF,返回读取的字节数,发生错误抛出runtime_error异常
    ssize_t readnInBuf(void* usrBuf, ssize_t n);

    //不停的从缓冲区中read,直到读满一行或到达EOF,返回读取的字节数,发生错误抛出runtime_error异常
    ssize_t readLineInBuf(void* usrBuf, ssize_t maxLen);
};


#endif //TINYWEB_RIO_HPP
