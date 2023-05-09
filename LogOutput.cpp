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

void LogOutput::log(const string& tag, FILE* fptr, char const* fmt, va_list vaList) {
    fprintf(fptr, "[%s] %s\t", getNowTimeStr().c_str(), tag.c_str());
    //[2022/03/31/ 12:31:13.314] INFO
    vfprintf(fptr, fmt, vaList);
    fprintf(fptr, "\n");
    fflush(fptr);
}

void LogOutput::d(const char* fmt, ...) {
    va_list vaList;
    va_start(vaList, fmt);
    log("DEBUG", logFilePtr, fmt, vaList);
    va_end(vaList);
}

void LogOutput::i(const char* fmt, ...) {
    va_list vaList;
    va_start(vaList, fmt);
    log("INFO", logFilePtr, fmt, vaList);
    va_end(vaList);
}

void LogOutput::w(const char* fmt, ...) {
    va_list vaList;
    va_start(vaList, fmt);
    log("WARNING", logFilePtr, fmt, vaList);
    va_end(vaList);
}

void LogOutput::e(const char* fmt, ...) {
    va_list vaList;
    va_start(vaList, fmt);
    log("ERROR", logFilePtr, fmt, vaList);
    va_end(vaList);
}


