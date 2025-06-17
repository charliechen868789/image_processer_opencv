# shared-lib/qrcode_finder/CMakeLists.txt

add_library(qrcode_finder SHARED
    qrcode_finder.cpp
)

target_include_directories(qrcode_finder PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qrcode_finder PUBLIC ${OpenCV_LIBS})
