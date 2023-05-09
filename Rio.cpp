/*
* author:ajin
* create time:4/25/2022 18:23:06
*/

#include "Rio.hpp"

ssize_t Rio::rioRead(char* usrBuf, ssize_t n) {
    if (n < 0) throw std::runtime_error("n不能为负数");
    while (rio.unreadCount <= 0) {  /* Refill if buf is empty */
        rio.unreadCount = limitTimeToRead(rio.fd, rio.buf, sizeof(rio.buf), nullptr);
        if (rio.unreadCount == 0) {  /* EOF */
            return 0;
        } else
            rio.unreadPtr = rio.buf; /* Reset buffer ptr */
    }

    /* Copy min(bufLen, rio.unreadCount) bytes from internal buf to user buf */
    auto count = std::min<ssize_t>(n, rio.unreadCount);
    memcpy(usrBuf, rio.unreadPtr, count);
    rio.unreadPtr += count;
    rio.unreadCount -= count;
    return count;
}

ssize_t Rio::readn(int fd, void* usrBuf, ssize_t n) {
    if (n < 0) throw std::runtime_error("n不能为负数");
    if (fd < 0) throw std::runtime_error("fd不能为负数");
    ssize_t leftCount = n;
    ssize_t rc;
    char* bufPtr = (char*) usrBuf;

    while (leftCount > 0) {
        if ((rc = limitTimeToRead(fd, bufPtr, leftCount, nullptr)) < 0) {
            if (errno == EINTR) /* Interrupted by sig handler return */
                rc = 0;      /* and call rioRead() again */
            else
                throw std::runtime_error(std::strerror(errno));
        } else if (rc == 0)
            break;              /* EOF */
        leftCount -= rc;
        bufPtr += rc;
    }
    return (n - leftCount);         /* Return >= 0 */
}

ssize_t Rio::writen(int fd, void const* usrBuf, ssize_t n) {
    if (n < 0) throw std::runtime_error("n不能为负数");
    if (fd < 0) throw std::runtime_error("fd不能为负数");
    ssize_t leftCount = n;
    ssize_t rc;
    char* bufPtr = (char*) usrBuf;

    while (leftCount > 0) {
        if ((rc = write(fd, bufPtr, leftCount)) <= 0) {
            if (errno == EINTR)  /* Interrupted by sig handler return */
                rc = 0;    /* and call write() again */
            else
                throw std::runtime_error(std::strerror(errno));
        }
        leftCount -= rc;
        bufPtr += rc;
    }
    return n;
}

//read带计时器的包裹函数,默认阻塞3秒钟
ssize_t Rio::limitTimeToRead(int fd, void* usrBuf, ssize_t n, struct timeval* limit) {
    if (n < 0) throw std::runtime_error("n不能为负数");
    if (fd < 0) throw std::runtime_error("fd不能为负数");
    if (usrBuf == nullptr) std::runtime_error("usrBuf不能为nullptr");
    fd_set fdSet;
    FD_ZERO(&fdSet);
    FD_SET(fd, &fdSet);
    int rc = 0;
    if (limit == nullptr) {
        //TODO: 搞明白,为什么select每次都会阻塞完所有时间才返回.
        struct timeval limitTime = {3, 0};
        rc = select(fd + 1, &fdSet, nullptr, nullptr, &limitTime);  /* The default delay is five seconds */
    } else{
        rc = select(fd + 1, &fdSet, nullptr, nullptr, limit);
    }

    if (rc == 0) {
        return 0;
    } else if (rc < 0) {
        throw std::runtime_error(string(std::strerror(errno)));
    }
    ssize_t count = Read(fd, usrBuf, n);
    return count;
}

//Associate a descriptor with a rioRead buffer and reset buffer
Rio::Rio(int fd) {
    if (fd < 0) throw std::runtime_error("文件描述符不能为负数");
    rio.fd = fd;
    rio.unreadCount = 0;
    rio.unreadPtr = rio.buf;
}

//Robustly rioRead bufLen bytes (buffered)
ssize_t Rio::readnInBuf(void* usrBuf, ssize_t n) {
    if (n < 0) throw std::runtime_error("n不能为负数");
    ssize_t leftCount = n;
    ssize_t rc;
    char* bufPtr = (char*) usrBuf;

    while (leftCount > 0) {
        if ((rc = rioRead(bufPtr, leftCount)) == 0)
            break;
        leftCount -= rc;
        bufPtr += rc;
    }
    return (n - leftCount);         /* return >= 0 */
}

//Robustly rioRead a text lineBuf (buffered)
ssize_t Rio::readLineInBuf(void* usrBuf, ssize_t maxLen) {
    if (maxLen < 2) throw std::runtime_error("maxLen不能小于2,最短也为'\n\0'");
    int n = 0;
    char c = '\0';
    char* bufPtr = (char*) usrBuf;

    for (n = 1; n < maxLen; n++) {
        if (rioRead(&c, 1) == 1) {
            *bufPtr++ = c;
            if (c == '\n') {
                n++;
                break;
            }
        } else {
            if (n == 1) {
                return 0; /* EOF, no data rioRead */
            } else
                break;    /* EOF, some data was rioRead */
        }
    }
    *bufPtr = '\0';
    return n - 1;
}