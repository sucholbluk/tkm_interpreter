set(SOURCE_HANDLER_TEST_SOURCES
    test_source_handler.cpp
)
find_package(Boost 1.88.0 REQUIRED COMPONENTS unit_test_framework)
add_executable(source_handler_tests ${SOURCE_HANDLER_TEST_SOURCES})
target_link_libraries(source_handler_tests
    Boost::unit_test_framework
    source_handler
)
target_include_directories(source_handler_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME source_handler_tests COMMAND source_handler_tests)