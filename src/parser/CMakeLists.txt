add_library(parser STATIC 
            "type.cpp" 
            "program.cpp" 
            parser.cpp
            statement.cpp
            node.cpp
            typed_identifier.cpp
            printer.cpp
            expression.cpp
            verbose_parser.cpp
)

target_link_libraries(parser PUBLIC lexer)

target_include_directories(parser PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(parser PRIVATE spdlog fmt)