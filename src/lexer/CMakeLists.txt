add_library(lexer STATIC lexer.cpp token.cpp)

target_link_libraries(lexer PUBLIC source_handler)

target_include_directories(lexer PUBLIC ${CMAKE_SOURCE_DIR}/include)