add_library(source_handler STATIC source_handler.cpp)

target_link_libraries(source_handler PUBLIC position exceptions)

target_include_directories(source_handler PUBLIC ${CMAKE_SOURCE_DIR}/include/source_handler)