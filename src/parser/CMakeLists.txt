add_library(parser STATIC "type.cpp")

target_link_libraries(lexer PUBLIC source_handler)

target_include_directories(lexer PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(lexer PRIVATE spdlog fmt)