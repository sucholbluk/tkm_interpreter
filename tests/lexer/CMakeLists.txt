set(POSITION_TEST_SOURCES
    test_token.cpp
)

add_executable(lexer_tests ${POSITION_TEST_SOURCES})
target_link_libraries(lexer_tests
    Boost::unit_test_framework
    lexer
)
target_include_directories(lexer_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME lexer_tests COMMAND lexer_tests)