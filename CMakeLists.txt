cmake_minimum_required(VERSION 3.18)

project(KArchive LANGUAGES C CXX)

if(WIN32)
    set(CMAKE_DEBUG_POSTFIX d)
else()
    set(CMAKE_DEBUG_POSTFIX _debug)
endif()

add_subdirectory(zlib)
add_subdirectory(bzip2)
add_subdirectory(xz)
add_subdirectory(karchive)
