set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

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
    kfilterdev.cpp
    kfilterdev.h
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
)

if(WIN32)
    enable_language(RC)
    list(APPEND SOURCES karchive.rc)
endif()

add_library(karchive SHARED ${SOURCES})

if(MSVC)
    target_compile_options(karchive PRIVATE /utf-8)
endif()
target_compile_definitions(karchive PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    KARCHIVE_LIBRARY
)
if(WIN32)
    target_compile_definitions(karchive PRIVATE
        WIN32_LEAN_AND_MEAN
        _CRT_SECURE_NO_WARNINGS
    )
endif()
target_link_libraries(karchive PRIVATE Qt::Core zlib bz2 LibLZMA)
target_include_directories(karchive PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
)
