#pragma once
/*
* author:ajin
* create time:4/17/2022
*/

#ifndef TINYWEB_HTTP_REQUEST_HPP
#define TINYWEB_HTTP_REQUEST_HPP

#include<map>
#include<string>
#include<regex>
#include"Rio.hpp"

using std::string;
using std::map;

class HttpRequest {
private:
    using byteBuf = string;
    std::map<string, string> line = //方法 url 版本 eg:GET /index.html HTTP/1.1
            {{"method",  ""},
             {"uri",     ""},
             {"version", ""}};
    map<string, string> headers;
    byteBuf body;

public:
    static HttpRequest getRequestForRawBuf(const string& buf);

    string getMethod() const;

    void setMethod(const string& method);

    string getPath() const;

    void setPath(const string& path);

    string getUri() const;

    void setUri(const string& uri);

    string getVersion() const;

    void setVersion(const string& version);

    map<string,string> getLine()const;
    void setLine(map<string,string> line);


    //ext不包含'.',eg:是"html",而不是".html"
    string getExt() const;

    void setExt(const string& ext);

    //根据GET还是POST获取参数
    string getParam() const;  //a=x&b=y&c=z
    void setParam(const string& paramStr);

    string getHeader(const string& head) const;

    void setHeader(const string& k, const string& v);

    map<string, string> getHeaders() const;

    void setHeaders(const map<string, string>& headers);

    string getBody() const;

    //注意'\0'截断,二进制文件最好指明文件长度
    void setBody(const byteBuf& body);

    void fillData(int fd);

};

#endif  //TINYWEB_HTTP_REQUEST_HPP