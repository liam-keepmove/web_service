#pragma once
/*
* author:ajin
* create time:4/17/2022
*/
#ifndef TINYWEB_HTTP_RESPONSE_HPP
#define TINYWEB_HTTP_RESPONSE_HPP

#include<map>
#include<string>
#include<regex>
#include"Rio.hpp"

using std::string;
using std::map;

static std::map<int,string> codeNameMap{
        {100,"Continue"},
        {101,"Switching Protocols"},
        {200,"OK"},
        {201,"Created"},
        {202,"Accepted"},
        {203,"Non-Authoritative"},
        {204,"No Content"},
        {205,"Reset Content"},
        {206,"Partial Content"},
        {300,"Multiple Choices"},
        {301,"Moved Permanently"},
        {302,"Found"},
        {303,"See Other"},
        {304,"Not Modified"},
        {305,"Use Proxy"},
        {306,"Unused"},
        {307,"Temporary Redirect"},
        {400,"Bad Request"},
        {401,"Unauthorized	"},
        {402,"Payment Required"},
        {403,"Forbidden"},
        {404,"Not Found"},
        {405,"Method Not Allowed"},
        {406,"Not Acceptable"},
        {407,"Proxy Authentication Required"},
        {408,"Request Time-out"},
        {409,"Conflict"},
        {410,"Gone"},
        {411,"Length Required	Content-Length"},
        {412,"Precondition Failed"},
        {413,"Request Entity Too Large"},
        {414,"Request-URI Too Large"},
        {415,"Unsupported Media Type"},
        {416,"Requested range not satisfiable"},
        {417,"Expectation Failed"},
        {500,"Internal Server Error"},
        {501,"Not Implemented"},
        {502,"Bad Gateway"},
        {503,"Service Unavailable"},
        {504,"Gateway Time-out"},
        {505,"HTTP Version not supported"}
};


class HttpResponse {
public:
    using byteBuf = string;
private:
    //"版本 状态码 状态描述" eg:HTTP/1.0 200 OK
    std::map<string,string> line =
            {{"version",    "HTTP/1.0"},
             {"statusCode", "200"},
             {"statusName", "OK"}};
    map<string, string> headers;
    byteBuf body;
public:
    static HttpResponse getRespForCgiBuf(const byteBuf& cgiBuf);

    static HttpResponse getRespForRawMessage(const byteBuf& rawMessage);

    string getVersion() const;

    void setVersion(const string& version);

    int getStatusCode() const;

    void setStatusCode(int statusCode, bool autoFillStatusName = true);

    string getStatusName() const;

    void setStatusName(const string& statusName);

    byteBuf getLine() const;

    void setLine(const string& version,int statusCode,const string& statusName);

    void setHeader(const string& head, const string& value);

    void deleteHeader(const string& head);

    void deleteHeaders();

    string getHeader(const string& head) const;

    byteBuf getHeaders() const;

    void setBody(const byteBuf& body);

    byteBuf getBody() const;

    //httpResponse转换为字节流
    byteBuf convertByteBuf() const;

    void sendData(int fd)const;
};

#endif //TINYWEB_HTTP_RESPONSE_HPP
