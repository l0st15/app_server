cmake_minimum_required(VERSION 3.29)
project(app_server)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR}/header)


add_executable(app_server cpp/main.cpp
        header/DBManager.h
        cpp/DBManager.cpp)
