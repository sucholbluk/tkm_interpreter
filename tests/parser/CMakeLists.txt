set(PARSER_TEST_SOURCES
    test_type.cpp
)
find_package(Boost 1.88.0 REQUIRED COMPONENTS unit_test_framework)
add_executable(parser_tests ${PARSER_TEST_SOURCES})
target_link_libraries(parser_tests
    Boost::unit_test_framework
    parser
)
target_include_directories(parser_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME parser_tests COMMAND parser_tests)