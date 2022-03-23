/*
* author:ajin
* create time:4/17/2022
*/

#include "HttpResponse.hpp"

using byteBuf = HttpResponse::byteBuf;

string HttpResponse::getVersion() const {
    return std::get<0>(line).second;
}

void HttpResponse::setVersion(string version) {
    std::get<0>(line).second = version;
}

int HttpResponse::getStatusCode() const {
    return std::get<1>(line).second;
}

void HttpResponse::setStatusCode(int statusCode, bool autoFillStatusName) {
    if(statusCode<0)
        throw std::invalid_argument("statusCode不能小于0");
    std::get<1>(line).second = statusCode;
    if(autoFillStatusName){
        setStatusName(codeNameMap.at(statusCode));
    }
}

string HttpResponse::getStatusName() const {
    return std::get<2>(line).second;
}

void HttpResponse::setStatusName(string statusName) {
    std::get<2>(line).second = statusName;
}

byteBuf HttpResponse::getLine() const {
    return std::get<0>(line).second + ' ' + std::to_string(std::get<1>(line).second) + ' ' + std::get<2>(line).second +
           "\r\n";
}

void HttpResponse::setLine(const string& version, int statusCode, const string& statusName) {
    setVersion(version);
    setStatusCode(statusCode);
    setStatusName(statusName);
}

void HttpResponse::setHeader(string head, string value) {
    headers[head] = value;
}

string HttpResponse::getHeader(string head) const {
    if (headers.find(head) == headers.end()) {
        return "";
    } else {
        return headers.at(head);
    }
}

byteBuf HttpResponse::getHeaders() const {
    byteBuf retStr;
    for (const auto& item: headers) {
        retStr += item.first + ": " + item.second + "\r\n";
    }
    return retStr;
}

void HttpResponse::setBody(byteBuf body) {
    this->body = body;
}

byteBuf HttpResponse::getBody() const {
    return body;
}

byteBuf HttpResponse::getSerialize() const {
    byteBuf retStr;
    retStr += getLine();
    retStr += getHeaders() + "\r\n";
    retStr += getBody();
    return retStr;
}

void HttpResponse::sendData(int fd) const {
    if (fd < 0) throw std::range_error("fd不能小于0");
    auto byteBuf = this->getSerialize();
    if (Rio::writen(fd, byteBuf.data(), byteBuf.length()) != byteBuf.length())
        throw std::runtime_error("响应发送失败");
}


