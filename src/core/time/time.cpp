#include "core/time/time.h"

#include "glfw/glfw3.h"

float _time;
float deltaTime;

namespace dem{

void Time::Update(){
    float newTime = (float)glfwGetTime();
    deltaTime = newTime - _time;
    _time = newTime;
}

float Time::GetDeltaTime(){
    return deltaTime;
}
}