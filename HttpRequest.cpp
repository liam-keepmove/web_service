/*
* author:ajin
* create time:4/19/2022 17:19:20
*/

#include "HttpRequest.hpp"

HttpRequest HttpRequest::getRequestForRawBuf(const string& buf) {
    HttpRequest request;
    std::regex requestRegex(
            R"(^(\S+) (\S+) (HTTP\/\d\.\d)\r\n([\S+: .+\r\n]*)\r\n([^$]*)$)");  //GET /index.html?a=1&b=2 HTTP/1.1
    std::regex headerRegex(R"((\S+): (.+)\r\n)");
    std::smatch strMatch;
    byteBuf headersBuf;
    if (!std::regex_match(buf, strMatch, requestRegex)) {
        throw std::invalid_argument("HTTP原始报文不符合规定格式.");
    } else {
        request.line["method"] = strMatch.str(1);
        request.line["uri"] = strMatch.str(2);
        request.line["version"] = strMatch.str(3);
        headersBuf = strMatch.str(4);
        request.body = strMatch.str(5);
    }

    auto begin = std::sregex_iterator(headersBuf.begin(), headersBuf.end(), headerRegex);
    auto end = std::sregex_iterator();
    for (auto i = begin; i != end; ++i) {
        std::smatch result = *i;
        request.headers[result.str(1)] = result.str(2);
    }
    return request;
}

string HttpRequest::getMethod() const {
    return line.at("method");
}

string HttpRequest::getUri() const {
    return line.at("uri");
}

string HttpRequest::getVersion() const {
    return line.at("version");
}

string HttpRequest::getPath() const {
    string uri = getUri();
    return uri.substr(0, uri.find('?'));
}

string HttpRequest::getParam() const {
    string method = getMethod();
    if (method == "GET") {
        string uri = getUri();
        auto pos = uri.rfind('?');
        if (pos == string::npos)
            return "";
        else
            return uri.substr(pos + 1);
    } else if (method == "POST") {
        // TODO: 获取post访问的参数
    }
    return "";
}

string HttpRequest::getHeader(const string& head) const {
    auto it = headers.find(head);
    if (it == headers.end())
        return "";
    else
        return it->second;
}

string HttpRequest::getBody() const {
    return body;
}

string HttpRequest::getExt() const {
    auto path = getPath();
    auto pos = path.rfind('.');
    if (pos == string::npos)
        return "";
    else
        return path.substr(pos + 1);
}

void HttpRequest::setMethod(const string& method) {
    line["method"] = method;
}

void HttpRequest::setUri(const string& uri) {
    line["uri"] = uri;
}

void HttpRequest::setVersion(const string& version) {
    line["version"] = version;
}

void HttpRequest::setPath(const string& path) {
    string uri = getUri();
    auto pos = uri.find('?');
    if (pos == string::npos)
        setUri(path);
    else
        setUri(path + uri.substr(pos));
}

void HttpRequest::setExt(const string& ext) {
    string path = getPath();
    path = path.substr(0, path.rfind('.')) + '.' + ext;
    setPath(path);
}

void HttpRequest::setParam(const string& paramStr) {
    string method = getMethod();
    if (method == "GET") {
        string uri = getUri();
        uri = uri.substr(0, uri.rfind('?')) + '?' + paramStr;
        setUri(uri);
    } else if (method == "POST") {

    }
}

void HttpRequest::setHeader(const string& k, const string& v) {
    headers[k] = v;
}

void HttpRequest::setBody(const HttpRequest::byteBuf& body) {
    this->body = body;
}

void HttpRequest::fillData(int fd) {
    char buf[PAGE_SIZE_AVE] = {};
    byteBuf fdBuf;
    int n = sizeof(buf);
    std::cerr<<"请求数据：";
    while (n == sizeof(buf)) {
        n = Rio::readn(fd, buf, sizeof(buf));
        for(auto ch:buf) {
            printf("%c",ch);
        }
        fdBuf += byteBuf(buf,n);  //防止0截断.
    }
    std::cerr<<std::endl;
//    std::cerr << "请求数据Buf:" << fdBuf << std::endl;
    HttpRequest request = getRequestForRawBuf(fdBuf);
    this->setLine(request.getLine());
    this->setHeaders(request.getHeaders());
    this->setBody(request.getBody());

}

map<string, string> HttpRequest::getHeaders() const {
    return headers;
}

void HttpRequest::setHeaders(const map<string, string>& headers) {
    this->headers = headers;
}

map<string, string> HttpRequest::getLine() const {
    return line;
}

void HttpRequest::setLine(map<string, string> line) {
    this->line = line;
}
