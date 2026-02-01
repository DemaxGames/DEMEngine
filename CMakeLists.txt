cmake_minimum_required(VERSION 3.31.5)
project(test)
include_directories("include")

set(CMAKE_BUILD_TYPE "Release")

add_compile_options(-g)
add_executable(${PROJECT_NAME} src/app/main.cpp)

target_link_directories(${PROJECT_NAME} PRIVATE build/core/)
target_link_libraries(${PROJECT_NAME} demengine)

target_link_directories(${PROJECT_NAME} PRIVATE lib/)
target_link_libraries(${PROJECT_NAME} glfw3)