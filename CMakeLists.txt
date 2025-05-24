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
	src/vec3.cpp
	src/ray.h
	src/interval.h
	src/objects.h
	src/hit_objects.h
	src/material.h
	)


# add executable file
add_executable(ray ${SOURCES})


