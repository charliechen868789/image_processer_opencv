cmake_minimum_required(VERSION 3.10)
project(qrcode_finder)

add_library(qrcode_finder SHARED
    qrcode_finder.cpp
)

target_include_directories(qrcode_finder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(qrcode_finder PRIVATE QRCODE_FINDER_BUILD)

find_package(OpenCV REQUIRED)
target_link_libraries(qrcode_finder PUBLIC ${OpenCV_LIBS})
