#pragma once
/*
* author:ajin
* create time:4/23/2022 17:49:07
*/


#ifndef TINYWEB_HTTPSERVER_HPP
#define TINYWEB_HTTPSERVER_HPP

#include<vector>
#include<functional>
#include<csignal>
#include<thread>
#include "wrapper.h"
#include "LogOutput.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Pool.hpp"  //模板函数或模板类不能分离编译,链接器对于重复实例化代码会优化为一份,不会报符号冲突的错误.  相关参考
// https://blog.csdn.net/shaochuang1/article/details/102943701
// https://blog.csdn.net/wanggao_1990/article/details/111048286
#include "NetworkAction.hpp"

static string getFileMiMeType(const string& key) {
    static std::map<string, const string> fileMiMeType = {
            {"html", "text/html"},
            {"gif",  "image/gif"},
            {"png",  "image/png"},
            {"jpg",  "image/jpeg"},
            {"ico",  "image/icon"},
            {"mp4",  "video/mp4"}
            //其他都是text/plain
    };
    auto it = fileMiMeType.find(key);
    if (it == fileMiMeType.end()) {
        return "text/plain";
    } else {
        return it->second;
    }
}

static std::map<int, HttpResponse::byteBuf> errorMsgCache;  //错误信息缓存,不用每次发生404,500之类的读取文件,缓存在这里

static std::vector<string> dynamicExtNameArr = {
        "out", "exe", "bat", "sh", "php", "py"
};

class HttpServer {
private:
    const int port = 80;  //需要监听的端口
    const int threadNum = 7;  //线程数量
    Pool<int> fdPool;  //请求池
    static constexpr char const* const errorDirPath = "./error/";
    static constexpr char const* const webFileDirPath = "./htdocs/";
    static constexpr char const* const logDirPath = "./logs/";
    static constexpr char const* const defaultHtmlName = "index.html";

    //管道破裂的信号的处理函数
    static void sigpipeHandler(int sig);  //多线程的每个线程都需要注册这个信号

    //更新错误码对应的文件缓存
    static void updateErrorMsgCache();

    //从请求池取出请求并处理
    void httpThread(int threadSerial);

    //基础的请求处理函数,返回真继续调用后续函数,返回假停止调用后续处理函数.
    static bool baseHandle(HttpRequest& request, HttpResponse& response);

    //对请求进行安全检查和一些初始化工作.返回真继续调用后续函数,返回假停止调用后续处理函数.
    static bool checkAndInit(HttpRequest& request,HttpResponse& response);

    std::vector<std::function<bool(HttpRequest&, HttpResponse&)>> handlers={checkAndInit,baseHandle};

    void scheduler(HttpRequest& request, HttpResponse& response);  //请求处理函数的调度器

public:
    explicit HttpServer(int port = 80, int threadNum = 7);

    void addHandler(std::function<bool(const HttpRequest&, HttpResponse&)> requestHandler);  //增加处理函数

    void run();  //开始监听端口,生成线程,运行服务

};

#endif //TINYWEB_HTTPSERVER_HPP
