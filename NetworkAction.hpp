#pragma once
/*
* author:ajin
* create time:4/27/2022 16:03:00
*/


#ifndef TINYWEB_NETWORKACTION_HPP
#define TINYWEB_NETWORKACTION_HPP

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <string>
#include <iostream>
#include "wrapper.h"


//打开并返回指定服务(端口)的监听套接字,成功返回非负文件描述符,失败抛出runtime_error
int openListenFd(char const* serverName);

//打开并返回指定远端地址(hostName:serverName)的套接字,成功返回非负文件描述符,没有可用地址抛出0,异常抛出runtime_error
int openClientFd(char const* hostName, char const* serverName);

#endif //TINYWEB_NETWORKACTION_HPP
