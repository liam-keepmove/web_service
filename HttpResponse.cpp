/*
* author:ajin
* create time:4/17/2022
*/

#include "HttpResponse.hpp"

using byteBuf = HttpResponse::byteBuf;

HttpResponse HttpResponse::getRespForCgiBuf(const byteBuf& cgiBuf) {
    HttpResponse response;
    std::regex cgiRegex(R"((?:\S+: [^\r\n]+\r\n)+(\r\n)[^$]*)");
    std::regex headerRegex(R"((\S+): ([^\r\n]+)\r\n)");
    std::smatch results;
    if (std::regex_match(cgiBuf, results, cgiRegex)) {
        string headers = string(cgiBuf.begin(), cgiBuf.begin() + results.position(1));
        string body = string(cgiBuf.begin() + results.position(1) + 2, cgiBuf.end());;

        auto searchBegin = std::sregex_iterator(headers.begin(), headers.end(), headerRegex);
        auto searchEnd = std::sregex_iterator();
        for (auto it = searchBegin; it != searchEnd; ++it) {  //设置响应头
            string headerK = (*it).str(1);
            string headerV = (*it).str(2);
            if (headerK == "Status") {  //类似于"Status: 302 MOVE",用来设置响应行
                std::regex statusRegex(R"(^(\d{3}) (.+)$)");
                std::smatch results;
                if (std::regex_match(headerV, results, statusRegex)) {
                    response.setStatusCode(std::stoi(results.str(1)));
                    response.setStatusName(results.str(2));
                } else {
                    //Status格式错误
                    response.deleteHeaders();
                    response.setStatusCode(500);
                    response.setHeader("Content-Type", "text/plain");
                    response.body = "Status format error of response headersBuf.";
                    return response;
                }
            } else {
                response.setHeader(headerK, headerV);
            }
        }
        response.body = body;
    } else {
        response.setStatusCode(500);
        response.setHeader("Content-Type", "text/plain");
        response.body = "Status format error of response headersBuf.";
    }
    return response;
}

HttpResponse HttpResponse::getRespForRawMessage(const byteBuf& rawMessage) {
    //TODO:从原始消息中获取响应对象
    //HTTP/1.0 200 OK
    HttpResponse response;
    return response;
}

string HttpResponse::getVersion() const {
    return this->line.at("version");
}

void HttpResponse::setVersion(const string& version) {
    this->line.at("version") = version;
}

int HttpResponse::getStatusCode() const {
    return std::stoi(this->line.at("statusCode"));
}

void HttpResponse::setStatusCode(int statusCode, bool autoFillStatusName) {
    if (statusCode < 0)
        throw std::invalid_argument("statusCode不能小于0");
    line.at("statusCode") = std::to_string(statusCode);
    if (autoFillStatusName) {
        setStatusName(codeNameMap.at(statusCode));
    }
}

string HttpResponse::getStatusName() const {
    return line.at("statusName");
}

void HttpResponse::setStatusName(const string& statusName) {
    line.at("statusName") = statusName;
}

byteBuf HttpResponse::getLine() const {
    return getVersion() + ' ' + std::to_string(getStatusCode()) + ' ' + getStatusName();
}

void HttpResponse::setLine(const string& version, int statusCode, const string& statusName) {
    setVersion(version);
    setStatusCode(statusCode);
    setStatusName(statusName);
}

void HttpResponse::setHeader(const string& head, const string& value) {
    if (value != "")
        headers[head] = value;
    else
        throw std::runtime_error("header-value can't empty.");
}

string HttpResponse::getHeader(const string& head) const {
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
    retStr.erase(retStr.length() - 2);  //删除最后一行的\r\n
    return retStr;
}

void HttpResponse::setBody(const byteBuf& body) {
    this->body = body;
}

byteBuf HttpResponse::getBody() const {
    return body;
}

byteBuf HttpResponse::convertByteBuf() const {
    byteBuf retStr;
    retStr += getLine() + "\r\n";
    retStr += getHeaders() + "\r\n";
    retStr += "\r\n";
    retStr += getBody();
    return retStr;
}

void HttpResponse::sendData(int fd) const {
    if (fd < 0) throw std::range_error("fd不能小于0");
    auto byteBuf = this->convertByteBuf();
    Rio::writen(fd, byteBuf.data(), byteBuf.length());
}

void HttpResponse::deleteHeader(const string& head) {
    headers.erase(head);
}

void HttpResponse::deleteHeaders() {
    headers.clear();
}
