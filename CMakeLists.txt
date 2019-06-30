cmake_minimum_required (VERSION 2.6)
project (vcglib)

if (BUILD_SHARED_LIBS)
else()
    set(BUILD_SHARED_LIBS on)
endif()

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" ) 

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(apps)
