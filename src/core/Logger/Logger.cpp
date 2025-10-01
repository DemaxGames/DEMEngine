#include "core/Logger/Logger.h"

namespace dem{

Logger::Logger(){
    Logger::file = std::ofstream("log.txt");
    Logger::log("Initialized Logger");
}

Logger* Logger::get(){
    static Logger instance;
    return &instance;
}

void Logger::fileLog(const std::string& msg, const std::string& filepath){
    file << msg << '\n';
    file.flush();
}

void Logger::log(const std::string& msg){
    char time[80];
    std::time_t time_raw = std::time(nullptr);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    fileLog(time + msg, "Log.txt");
    std::cout << time << msg << '\n';
    return;
}
};