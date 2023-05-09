/*
* author:ajin
* create time:5/6/2022 21:13:39
*/

#include "miscFunc.hpp"

//字符串是否全由十进制数字组成
bool isAllDigit(const string& str) {
    for (char ch: str) {
        if (!std::isdigit(ch))
            return false;
    }
    return true;
}

string getNowTimeStr() {
    char strTime[30] = {};
    struct timeval tv = {};
    gettimeofday(&tv, nullptr);
    strftime(strTime, sizeof(strTime), "%Y/%m/%d %H:%M:%S.", localtime(&tv.tv_sec));
    return strTime + to_string(tv.tv_usec / 1000);;
}
