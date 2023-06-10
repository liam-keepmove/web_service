#pragma once
/*
* author:ajin
* create time:5/6/2022 21:13:39
*/


#ifndef TINYWEB_MISC_HPP
#define TINYWEB_MISC_HPP

#include <string>
#include <sys/time.h>

using std::string;
using std::to_string;

//字符串是否全由十进制数字组成
bool isAllDigit(const string& str);

//获取当前时间,精确到毫秒
string getNowTimeStr();

#endif //TINYWEB_MISC_HPP
