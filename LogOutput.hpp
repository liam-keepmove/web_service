#pragma once
/*
* author:ajin
* create time:4/24/2022 14:23:20
*/


#ifndef TINYWEB_LOGOUTPUT_HPP
#define TINYWEB_LOGOUTPUT_HPP
#include<string>
#include<cstdarg>
#include<iostream>
#include "miscFunc.hpp"
using std::string;

class LogOutput {
private:
    FILE* logFilePtr=nullptr;

    //log(INFO,fd,"%s",http_header);  输出后会立刻刷新流.
    static void log(const string& tag,FILE* fptr,char const* fmt,va_list vaList);

public:
    explicit LogOutput(const string& logFileName);
    explicit LogOutput(FILE* filePtr);
    //默认输出流为stderr
    explicit LogOutput();
    //重定向到其他流
    void reDirect(const string& logFileName);
    void reDirect(FILE* filePtr);
    void i(char const* fmt,...);
    void d(char const* fmt,...);
    void w(char const* fmt,...);
    void e(char const* fmt,...);
};


#endif //TINYWEB_LOGOUTPUT_HPP
