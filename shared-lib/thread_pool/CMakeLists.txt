# shared-lib/thread_pool/CMakeLists.txt

add_library(thread_pool SHARED
    thread_pool.cpp
)

target_include_directories(thread_pool PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
