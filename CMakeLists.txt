cmake_minimum_required (VERSION 2.6)
project (vcglib)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" ) 

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(apps)
