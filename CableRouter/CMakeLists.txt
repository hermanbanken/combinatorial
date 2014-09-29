cmake_minimum_required(VERSION 2.8.4)
project(CableRouter)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
set(SOURCE_FILES main.cpp reader.cpp simplifier.cpp structures.cpp)
add_executable(CableRouter ${SOURCE_FILES})