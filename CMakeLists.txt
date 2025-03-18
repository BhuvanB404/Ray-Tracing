# spcifying minimum version of Cmake
cmake_minimum_required(VERSION 3.10)

#project
project(ray)

#c++ standard is ?
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add executable file
add_executable(ray ray.cpp)
