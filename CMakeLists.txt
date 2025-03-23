# spcifying minimum version of Cmake
cmake_minimum_required(VERSION 3.10)

#project
project(ray)

#c++ standard is ?
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#specifiuing source files
set(SOURCES
	src/ray.cpp
	src/color.h
	src/vec3.h
	src/ray.h)


# add executable file
add_executable(ray ${SOURCES})


