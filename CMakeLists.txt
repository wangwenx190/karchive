cmake_minimum_required(VERSION 3.18)

project(KArchive LANGUAGES C CXX)

add_subdirectory(zlib)
add_subdirectory(bzip2)
add_subdirectory(xz)
add_subdirectory(karchive)
