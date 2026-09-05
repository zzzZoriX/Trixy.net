cmake_minimum_required(VERSION 3.15)

project(trixy-net)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(trixy-net ./src/main.cpp)