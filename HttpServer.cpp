/*
* author:ajin
* create time:4/23/2022 17:49:07
*/

#include "HttpServer.hpp"

static thread_local LogOutput logOutput;

void HttpServer::sigpipeHandler(int sig) {  //多线程的每个线程都需要注册这个信号.
    logOutput.e("The peer closes the connection and the remaining bytes may not be received\n");
}

HttpServer::HttpServer(int port, int threadNum) : port(port), threadNum(threadNum) {
    if (port < 0) throw std::range_error("port不能小于0");
    if (threadNum < 0) throw std::range_error("线程数不能小于0");
    logOutput.reDirect(logDirPath + string("0_") + to_string(port) + ".log");
    updateErrorMsgCache();
}

void HttpServer::updateErrorMsgCache() {
    errorMsgCache = {};  //先清空
    std::regex fileNameRegex(R"(\d+?\.html)");
    auto errorDir = Opendir(errorDirPath);
    struct dirent* dirEnt = nullptr;
    while ((dirEnt = Readdir(errorDir)) != nullptr) {
        if (dirEnt->d_type == DT_REG && std::regex_match(dirEnt->d_name, fileNameRegex)) {  //是常规文件而且名称全为数字
            struct stat fileStat = {};
            string fileName = (errorDirPath + string() + dirEnt->d_name);
            Stat(fileName.c_str(), &fileStat);
            char* fileBuf = (char*) Malloc(fileStat.st_size);
            auto errorCodeFileFd = Open(fileName.c_str(), O_RDONLY, 0);
            Rio::readn(errorCodeFileFd, fileBuf, fileStat.st_size);
            errorMsgCache[atoi(dirEnt->d_name)] = HttpResponse::byteBuf(fileBuf);
            Close(errorCodeFileFd);
            Free(fileBuf);
        }
    }
    Closedir(errorDir);
}

bool HttpServer::checkAndInit(HttpRequest& request, HttpResponse& response) {
    string path = request.getPath();
    if (path.find("../") != string::npos || path.find("~/") != string::npos || path == "/")
        request.setPath(webFileDirPath + string() + defaultHtmlName);
    else
        request.setPath(webFileDirPath + path);
    return true;
}

bool HttpServer::baseHandle(HttpRequest& request, HttpResponse& response) {
    string fileName = request.getPath();

    struct stat fileStat = {};
    stat(fileName.c_str(), &fileStat);

    if (S_ISDIR(fileStat.st_mode)) {  //如果是文件夹的话,则变为访问此文件夹下的index.html文件.
        request.setPath(request.getPath() + string("/index.html"));
        fileName = request.getPath();
        stat(fileName.c_str(), &fileStat);
    }

    bool isStatic = std::find(dynamicExtNameArr.begin(), dynamicExtNameArr.end(), request.getExt()) ==
                    dynamicExtNameArr.end() ? true : false;
    bool isHeadMethod = request.getMethod() == "HEAD" ? true : false;
    response.setHeader("Server", "HJ Tiny Web");

    if (access(fileName.c_str(), F_OK) != 0) {  //文件不存在
        response.setStatusCode(404);
        response.setHeader("Content-Length", to_string(errorMsgCache.at(404).size()));
        response.setHeader("Content-Type", getFileMiMeType("html"));
        if (!isHeadMethod)
            response.setBody(errorMsgCache.at(404));
        return true;
    }

    if ((isStatic && access(fileName.c_str(), R_OK) != 0) ||
        (!isStatic && access(fileName.c_str(), X_OK) != 0)) {  //静态文件无权限可读或可执行文件无权限可执行.
        response.setStatusCode(403);
        response.setHeader("Content-Length", to_string(errorMsgCache.at(403).size()));
        response.setHeader("Content-Type", getFileMiMeType("html"));
        if (!isHeadMethod)
            response.setBody(errorMsgCache.at(403));
        return true;
    }

    if (isStatic) {  //静态文件访问且不是文件夹
        response.setHeader("Content-Type", getFileMiMeType(request.getExt()));
        response.setHeader("Content-Length", to_string(fileStat.st_size));
        if (!isHeadMethod) {
            int fd = Open(fileName.c_str(), O_RDONLY, 0);
            char* fdBuf = (char*) Malloc(fileStat.st_size);
            Rio::readn(fd, fdBuf, fileStat.st_size);
            response.setBody(HttpResponse::byteBuf(fdBuf, fileStat.st_size));
            Free(fdBuf);
            fdBuf = nullptr;
            Close(fd);
        }
        return true;
    } else {  //动态文件访问,开一个新进程然后执行CGI程序,使用管道进行IPC
        int fileds[2] = {};
        Pipe(fileds);
        if (Fork() == 0) {  //多进程中执行fork只会复制当前线程,小心死锁,所以立刻进行exec函数
            Close(fileds[0]);  //子进程中关闭读取端
            setenv("QUERY_STRING", request.getParam().c_str(), true);//第三个参数是覆盖选项.
            Dup2(fileds[1], STDOUT_FILENO);
            Close(fileds[1]);  //关闭多余的写入端
            if (execle(fileName.c_str(), fileName.c_str(), NULL, environ) == -1) {
                throw std::runtime_error("execle失败.");
            }
        }
        Close(fileds[1]);  //父进程中关闭写入端
        Wait(nullptr);
        HttpResponse::byteBuf fdBuf;
        char buf[PAGE_SIZE_AVE] = {};
        int n = sizeof(buf);
        while (n == sizeof(buf)) {
            n = Rio::readn(fileds[0], buf, sizeof(buf));
            fdBuf += HttpResponse::byteBuf(buf, n);  //防止0截断.
        }
        response = HttpResponse::getRespForCgiBuf(fdBuf);
        response.setHeader("Server", "HJ Tiny Web");
        if (isHeadMethod) {
            response.setBody("");
        }
    }
    return true;
}

void HttpServer::addHandler(std::function<bool(const HttpRequest&, HttpResponse&)> requestHandler) {
    handlers.push_back(requestHandler);
}

//按配置文件的规则,调用请求处理函数.
void HttpServer::scheduler(HttpRequest& request, HttpResponse& response) {
    //TODO: 目前只是按顺序访问,以后实现按照配置文件访问.
//    bool nextOne = true;
//    for (const auto& handler: handlers) {
//        nextOne = handler(request, response);
//        if (!nextOne)
//            break;
//    }
    handlers[0](request, response);
    handlers[1](request, response);
}

void HttpServer::httpThread(const int threadSerial) {
    logOutput.reDirect(logDirPath + to_string(threadSerial) + "_" + to_string(port) + ".log");
    Signal(SIGPIPE, &sigpipeHandler);
    while (true) {
        HttpRequest request;
        HttpResponse response;
        int fd = 0;
        try {
            fd = fdPool.pop();
            request.fillData(fd);
            scheduler(request, response);
        } catch (const std::invalid_argument& err) {
            fprintf(stderr, "%s\n", err.what());  //一般是 HTTP原始报文不符合规范
            continue;
        } catch (const std::regex_error& err) {
            fprintf(stderr, "%s\n", err.what());
            continue;
        } catch (const std::runtime_error& err) {
            fprintf(stderr, "%s\n", err.what());
            response.setStatusCode(500);
            response.setHeader("Content-Length", to_string(errorMsgCache.at(500).size()));
            response.setHeader("Content-Type", getFileMiMeType("html"));
            if (request.getMethod() != "HEAD")
                response.setBody(errorMsgCache.at(500));
        } catch (const std::out_of_range& err) {
            fprintf(stderr, "%s\n", err.what());
            continue;
        } catch (...) {
            fprintf(stderr, "%s\n", __PRETTY_FUNCTION__);
            continue;
        }

        try {
            response.sendData(fd);
            fprintf(stderr, "response.body size:%lu\n", response.getBody().size());
            Close(fd);
        } catch (...) {
            fprintf(stderr, "%s\n", __PRETTY_FUNCTION__);
        }
    }

}

void HttpServer::run() {
    try {
        for (int i = 1; i <= threadNum; ++i) {
            std::thread(&HttpServer::httpThread, this, i).detach();
        }
        int listenFd = openListenFd(to_string(port).c_str());
        logOutput.i("Http Server Start.");
        while (true) {
            int tcpFd = Accept(listenFd, nullptr, nullptr);
            fdPool.push(tcpFd);
        }
    } catch (const std::runtime_error& err) {
        logOutput.e(err.what());
    } catch (...) {
        logOutput.e(__PRETTY_FUNCTION__);
    }
}

