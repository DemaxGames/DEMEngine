#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "core/math/vec.h"
#include "core/math/mat.h"

namespace dem{
    
class Logger{
private:
    std::ofstream file;
    Logger();
public:
    static Logger* get();
    void log(const std::string& msg);
    void log(const std::string& msg, const float& f);
    void log(const std::string& msg, const int a);
    void log(const std::string& msg, const unsigned int a);
    void log(const std::string& msg, const math::vec2& vec);
    void log(const std::string& msg, const math::vec3& vec);
    void log(const std::string& msg, const math::vec4& vec);
    void log(const std::string& msg, const math::mat2& mat);
    void log(const std::string& msg, const math::mat3& mat);
    void log(const std::string& msg, const math::mat4& mat);
    void fileLog(const std::string& msg, const std::string& filepath);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};
}