#ifndef LOG_H
#define LOG_H

#include <string>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger(const std::string &filename);
    
    ~Logger();
    
    void init();
    
    void log(LogLevel level, const std::string &message);

private:
    std::string filename_;
    FILE *fp_;
    bool openFile();
    
    void closeFile();
    std::string getCurrentDateTime();
    std::string logLevelToString(LogLevel level);
};

#endif 