cmake_minimum_required(VERSION 3.31.5)
project(demengine)
include_directories("../../include")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(SRC_FILES
    Engine/Camera.cpp
    Engine/Object.cpp
    InputSystem/Input.cpp
    math/dot.cpp
    math/mat2.cpp
    math/mat3.cpp
    math/mat4.cpp
    math/normalize.cpp
    math/vec2.cpp
    math/vec3.cpp
    math/vec4.cpp
    Logger/Logger.cpp
    Renderer/GLProgram.cpp
    Renderer/Renderer.cpp
    Renderer/Shader.cpp
    Renderer/VertexArray.cpp 
    Renderer/VertexBuffer.cpp 
    time/time.cpp
    glad.c
    WinDLL.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_link_directories(${PROJECT_NAME} PRIVATE ../../lib/)
target_link_libraries(${PROJECT_NAME} glfw3)
