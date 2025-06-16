# shared-lib/image_processor/CMakeLists.txt

add_library(image_processor SHARED
    edge_detection.cpp
)

target_include_directories(image_processor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(image_processor PUBLIC ${OpenCV_LIBS})
