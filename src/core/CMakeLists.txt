cmake_minimum_required(VERSION 3.31.5)
project(demengine)
include_directories("../../include")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(SRC_FILES
    Logger/Logger.cpp
    Renderer/GLProgram.cpp
    Renderer/Renderer.cpp
    Renderer/Shader.cpp
    Renderer/VertexArray.cpp 
    Renderer/VertexBuffer.cpp 
    glad.c
    WinDLL.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_link_directories(${PROJECT_NAME} PRIVATE ../../lib/)
target_link_libraries(${PROJECT_NAME} glfw3)
