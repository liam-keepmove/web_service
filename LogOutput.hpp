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
#include "misc.hpp"
using std::string;

class LogOutput {
private:
    FILE* logFilePtr=nullptr;
public:
    static void logf(const string& tag, FILE* fptr, char const* fmt, va_list vaList);

    static void log(const string& tag,FILE* fptr,char const* str);

public:
    explicit LogOutput(const string& logFileName);
    explicit LogOutput(FILE* filePtr);
    //默认输出流为stderr
    explicit LogOutput();
    //重定向到其他流
    void reDirect(const string& logFileName);
    void reDirect(FILE* filePtr);
    void ifmt(char const* fmt, ...);
    void dfmt(char const* fmt, ...);
    void wfmt(char const* fmt, ...);
    void efmt(char const* fmt, ...);

    void i(char const* str);
    void d(char const* str);
    void w(char const* str);
    void e(char const* str);
};


#endif //TINYWEB_LOGOUTPUT_HPP
