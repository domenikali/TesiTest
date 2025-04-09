#include "logger.hpp"
#include <cstdio>
#include <ctime>
#include <cstring>

Logger::Logger(const std::string &filename){
    filename_ = filename;
    openFile();
    if(openFile()){
        exit(EXIT_FAILURE);
    }
}

Logger::~Logger() {
    closeFile();
}

bool Logger::openFile() {
    //open file and append
    fp_ = fopen(filename_.c_str(), "a");
    if (fp_ == nullptr) {
        perror("Failed to open log file");
        return true;
    }
    return false;
}

void Logger::closeFile() {
    if (fp_) {
        fclose(fp_);
        fp_ = nullptr;
    }
}

std::string Logger::getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buffer);
}

void Logger::init() {
    if (fp_ == nullptr) {
        if (!openFile()) return;
    }
    std::string datetime = getCurrentDateTime();
    std::string header = "\n--- Log Initialized at " + datetime + " ---\n";
    fputs(header.c_str(), fp_);
    fflush(fp_);
}

std::string Logger::logLevelToString(LogLevel level) {
    switch(level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string &message) {
    if (fp_ == nullptr) {
        if (!openFile()) return;
    }
    std::string levelStr = logLevelToString(level);
    std::string logEntry =" [" + levelStr + "] " + message + "\n";
    fputs(logEntry.c_str(), fp_);
    fflush(fp_);
}
