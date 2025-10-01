#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

namespace dem{
    
class Logger{
private:
    std::ofstream file;
    Logger();
public:
    static Logger* get();
    void log(const std::string& msg);
    void fileLog(const std::string& msg, const std::string& filepath);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};
}