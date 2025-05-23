add_library(exceptions STATIC exceptions.cpp)

target_link_libraries(exceptions PUBLIC lexer source_handler)

target_include_directories(exceptions PUBLIC ${CMAKE_SOURCE_DIR}/include)
