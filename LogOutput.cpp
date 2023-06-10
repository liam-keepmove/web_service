/*
* author:ajin
* create time:4/24/2022 14:23:20
*/

#include "LogOutput.hpp"

LogOutput::LogOutput() {
    logFilePtr = stderr;
}

LogOutput::LogOutput(const string& logFileName) {
    logFilePtr = fopen(logFileName.c_str(), "a");
    if (logFilePtr == nullptr) {
        throw std::bad_alloc();
    }
}

LogOutput::LogOutput(FILE* filePtr) {
    if (filePtr != nullptr)
        logFilePtr = filePtr;
    else
        throw std::invalid_argument("filePtr不能为空");
}

void LogOutput::reDirect(const string& logFileName) {
    logFilePtr = fopen(logFileName.c_str(), "a");
    if (logFilePtr == nullptr) {
        throw std::bad_alloc();
    }
}

void LogOutput::reDirect(FILE* filePtr) {
    if (filePtr != nullptr)
        logFilePtr = filePtr;
    else
        throw std::invalid_argument("filePtr不能为空");
}

void LogOutput::logf(const string& tag, FILE* fptr, char const* fmt, va_list vaList) {
    if (fptr == nullptr)
        throw std::invalid_argument("文件指针不能为null");
    if (fmt == nullptr)
        throw std::invalid_argument("fmt指针不能为null");
    //[2022/03/31/ 12:31:13.314] INFO:
    fprintf(fptr, "[%s] %s:", getNowTimeStr().c_str(), tag.c_str());
    vfprintf(fptr, fmt, vaList);
    fprintf(fptr, "\n");
    fflush(fptr);
}

void LogOutput::log(const string& tag, FILE* fptr, const char* str) {
    if (fptr == nullptr)
        throw std::invalid_argument("文件指针不能为null");
    if (str == nullptr)
        throw std::invalid_argument("str指针不能为null");
    fprintf(fptr, "[%s] %s:", getNowTimeStr().c_str(), tag.c_str());
    fputs(str,fptr);
    fputs("\n",fptr);
    fflush(fptr);
}

void LogOutput::dfmt(const char* fmt, ...) {
    va_list vaList;
    va_start(vaList, fmt);
    logf("DEBUG", logFilePtr, fmt, vaList);
    va_end(vaList);
}

void LogOutput::ifmt(const char* fmt, ...) {
    va_list vaList;
    va_start(vaList, fmt);
    logf("INFO", logFilePtr, fmt, vaList);
    va_end(vaList);
}

void LogOutput::wfmt(const char* fmt, ...) {
    va_list vaList;
    va_start(vaList, fmt);
    logf("WARNING", logFilePtr, fmt, vaList);
    va_end(vaList);
}

void LogOutput::efmt(const char* fmt, ...) {
    va_list vaList;
    va_start(vaList, fmt);
    logf("ERROR", logFilePtr, fmt, vaList);
    va_end(vaList);
}

void LogOutput::i(const char* str) {
    log("INFO", logFilePtr, str);
}

void LogOutput::d(const char* str) {
    log("DEBUG", logFilePtr, str);
}

void LogOutput::w(const char* str) {
    log("WARNING", logFilePtr, str);
}

void LogOutput::e(const char* str) {
    log("ERROR", logFilePtr, str);
}
