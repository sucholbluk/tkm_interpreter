set(SOURCE_HANDLER_TEST_SOURCES
    test_source_handler.cpp
)

add_executable(source_handler_unit_tests ${SOURCE_HANDLER_TEST_SOURCES})
target_link_libraries(source_handler_unit_tests
    Boost::unit_test_framework
    source_handler
)
target_include_directories(source_handler_unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME source_handler_unit_tests COMMAND source_handler_unit_tests)