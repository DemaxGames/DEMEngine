#include "core/time/time.h"

#include "glfw/glfw3.h"

float time;
float deltaTime;

namespace dem{

void Time::Update(){
    float newTime = (float)glfwGetTime();
    deltaTime = newTime - time;
    time = newTime;
}

float Time::GetDeltaTime(){
    return deltaTime;
}
}