add_library(lexer STATIC lexer.cpp token.cpp token_type.cpp logging_lexer.cpp)

target_link_libraries(lexer PUBLIC source_handler exceptions)

target_include_directories(lexer PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(lexer PRIVATE spdlog::spdlog)