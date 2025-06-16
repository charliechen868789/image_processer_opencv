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

# Link executable to shared library
target_link_libraries(opencv_demo PRIVATE image_processor)
