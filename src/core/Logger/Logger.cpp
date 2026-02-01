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
    std::time_t time_raw;
    std::time(&time_raw);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    fileLog(time + msg, "Log.txt");
    std::cout << time << msg << '\n';
    return;
}

void Logger::log(const std::string& msg, const float& f){
    char time[80];
    std::time_t time_raw = std::time(nullptr);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    fileLog(time + msg + std::to_string(f), "Log.txt");
    std::cout << time + msg + std::to_string(f) << '\n';
}

void Logger::log(const std::string& msg, const int a){
    char time[80];
    std::time_t time_raw = std::time(nullptr);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    fileLog(time + msg + std::to_string(a), "Log.txt");
    std::cout << time + msg + std::to_string(a) << '\n';
}

void Logger::log(const std::string& msg, const unsigned int a){
    char time[80];
    std::time_t time_raw = std::time(nullptr);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    fileLog(time + msg + std::to_string(a), "Log.txt");
    std::cout << time + msg + std::to_string(a) << '\n';
}


void Logger::log(const std::string& msg, const math::vec2& vec){
    char time[80];
    std::time_t time_raw = std::time(nullptr);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    std::string vec_str = "{";
    vec_str += std::to_string(vec[0]);
    vec_str += "; ";
    vec_str += std::to_string(vec[1]);
    vec_str += "}";
    fileLog(time + msg + vec_str, "Log.txt");
    std::cout << time + msg + vec_str<< '\n';
}

void Logger::log(const std::string& msg, const math::vec3& vec){
    char time[80];
    std::time_t time_raw = std::time(nullptr);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    std::string vec_str = "{";
    vec_str += std::to_string(vec[0]);
    vec_str += "; ";
    vec_str += std::to_string(vec[1]);
    vec_str += "; ";
    vec_str += std::to_string(vec[2]);
    vec_str += "}";
    fileLog(time + msg + vec_str, "Log.txt");
    std::cout << time + msg + vec_str << '\n';
}

void Logger::log(const std::string& msg, const math::vec4& vec){
    char time[80];
    std::time_t time_raw = std::time(nullptr);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    std::string vec_str = "{";
    vec_str += std::to_string(vec[0]);
    vec_str += "; ";
    vec_str += std::to_string(vec[1]);
    vec_str += "; ";
    vec_str += std::to_string(vec[2]);
    vec_str += "; ";
    vec_str += std::to_string(vec[3]);
    vec_str += "}";
    fileLog(time + msg + vec_str, "Log.txt");
    std::cout << time + msg + vec_str << '\n';
}

void Logger::log(const std::string& msg, const math::mat2& mat){
    char time[80];
    std::time_t time_raw = std::time(nullptr);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    std::string mat_str = "\n{";
    mat_str += std::to_string(mat[0][0]);
    mat_str += "; ";
    mat_str += std::to_string(mat[1][0]);
    mat_str += "}\n{";
    mat_str += std::to_string(mat[0][1]);
    mat_str += "; ";
    mat_str += std::to_string(mat[1][1]);
    mat_str += "}";
    fileLog(time + msg + mat_str, "Log.txt");
    std::cout << time + msg + mat_str << '\n';
}

void Logger::log(const std::string& msg, const math::mat3& mat){
    char time[80];
    std::time_t time_raw = std::time(nullptr);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    std::string mat_str = "\n{";
    mat_str += std::to_string(mat[0][0]);
    mat_str += "; ";
    mat_str += std::to_string(mat[1][0]);
    mat_str += "; ";
    mat_str += std::to_string(mat[2][0]);
    mat_str += "}\n{";
    mat_str += std::to_string(mat[0][1]);
    mat_str += "; ";
    mat_str += std::to_string(mat[1][1]);
    mat_str += "; ";
    mat_str += std::to_string(mat[2][1]);
    mat_str += "}\n{";
    mat_str += std::to_string(mat[0][2]);
    mat_str += "; ";
    mat_str += std::to_string(mat[1][2]);
    mat_str += "; ";
    mat_str += std::to_string(mat[2][2]);
    mat_str += "}";
    fileLog(time + msg + mat_str, "Log.txt");
    std::cout << time + msg + mat_str << '\n';
}

void Logger::log(const std::string& msg, const math::mat4& mat){
    char time[80];
    std::time_t time_raw = std::time(nullptr);
    std::strftime(time, 80, "[%H:%M:%S]", std::localtime(&time_raw));
    std::string mat_str = "\n{";
    mat_str += std::to_string(mat[0][0]);
    mat_str += "; ";
    mat_str += std::to_string(mat[1][0]);
    mat_str += "; ";
    mat_str += std::to_string(mat[2][0]);
    mat_str += "; ";
    mat_str += std::to_string(mat[3][0]);
    mat_str += "}\n{";
    mat_str += std::to_string(mat[0][1]);
    mat_str += "; ";
    mat_str += std::to_string(mat[1][1]);
    mat_str += "; ";
    mat_str += std::to_string(mat[2][1]);
    mat_str += "; ";
    mat_str += std::to_string(mat[3][1]);
    mat_str += "}\n{";
    mat_str += std::to_string(mat[0][2]);
    mat_str += "; ";
    mat_str += std::to_string(mat[1][2]);
    mat_str += "; ";
    mat_str += std::to_string(mat[2][2]);
    mat_str += "; ";
    mat_str += std::to_string(mat[3][2]);
    mat_str += "}\n{";
    mat_str += std::to_string(mat[0][3]);
    mat_str += "; ";
    mat_str += std::to_string(mat[1][3]);
    mat_str += "; ";
    mat_str += std::to_string(mat[2][3]);
    mat_str += "; ";
    mat_str += std::to_string(mat[3][3]);
    mat_str += "}";
    fileLog(time + msg + mat_str, "Log.txt");
    std::cout << time + msg + mat_str << '\n';
}

};