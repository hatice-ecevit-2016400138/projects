
cmake_minimum_required(VERSION 2.8.12.2)
project(project4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    function.cpp
    function.h
    graph.cpp
    graph.h
    main.cpp
    variable.cpp
    variable.h)

add_executable(project4 ${SOURCE_FILES})
