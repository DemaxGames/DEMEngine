cmake_minimum_required(VERSION 3.31.5)
project(demengine)
include_directories("../../include")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(CMAKE_BUILD_TYPE "Release")

add_compile_options(-g)
set(SRC_FILES
    Engine/Camera.cpp
    Engine/Image.cpp
    Engine/Mesh.cpp
    Engine/Object.cpp
    InputSystem/Input.cpp
    math/dot.cpp
    math/mat2.cpp
    math/mat3.cpp
    math/mat4.cpp
    math/normal.cpp
    math/normalize.cpp
    math/uid.cpp
    math/vec2.cpp
    math/vec3.cpp
    math/vec4.cpp
    Logger/Logger.cpp
    Platform/stb.cpp
    Renderer/ElementBuffer.cpp
    Renderer/GLImage.cpp
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
