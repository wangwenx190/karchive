cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(KArchive LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    if(WIN32)
        set(CMAKE_DEBUG_POSTFIX d)
    else()
        set(CMAKE_DEBUG_POSTFIX _debug)
    endif()
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()

if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(zstd REQUIRED)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(SOURCES
    karchive_global.h
    k7zip.cpp
    k7zip.h
    kar.cpp
    kar.h
    karchive.cpp
    karchive.h
    karchivedirectory.h
    karchiveentry.h
    karchivefile.h
    karchive_p.h
    kbzip2filter.cpp
    kbzip2filter.h
    kcompressiondevice.cpp
    kcompressiondevice.h
    kcompressiondevice_p.h
    kfilterbase.cpp
    kfilterbase.h
    kgzipfilter.cpp
    kgzipfilter.h
    klimitediodevice.cpp
    klimitediodevice_p.h
    knonefilter.cpp
    knonefilter.h
    krcc.cpp
    krcc.h
    ktar.cpp
    ktar.h
    kxzfilter.cpp
    kxzfilter.h
    kzip.cpp
    kzip.h
    kzipfileentry.h
    kzstdfilter.cpp
    kzstdfilter.h
)

if(WIN32)
    enable_language(RC)
    list(APPEND SOURCES karchive.rc)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    QT_NO_KEYWORDS
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x060100
    KARCHIVE_BUILD_LIBRARY
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    ZLIB::ZLIB
    BZip2::BZip2
    LibLZMA::LibLZMA
    zstd::libzstd_shared
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32_LEAN_AND_MEAN
        _CRT_SECURE_NO_WARNINGS
        UNICODE
        _UNICODE
    )
endif()
