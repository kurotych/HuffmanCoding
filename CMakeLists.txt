cmake_minimum_required(VERSION 2.8)

project(HuffmanCoding)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -g")
add_executable(${PROJECT_NAME} "main.cpp")
