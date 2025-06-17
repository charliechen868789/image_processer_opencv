# CMakeLists.txt at project root

cmake_minimum_required(VERSION 3.10)
project(image_process)

set(CMAKE_CXX_STANDARD 14)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add library and source
add_subdirectory(shared-lib)

# Executable
add_executable(opencv_demo src/main.cpp)

# Add include directories for thread_pool and image_processor
target_include_directories(opencv_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/shared-lib
)

# Link executable to shared library
target_link_libraries(opencv_demo PRIVATE 
    image_processor
    thread_pool
    qrcode_finder
)
