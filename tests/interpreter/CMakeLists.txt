set(INTERPRETER_TEST_SOURCES
    test_interpreter.cpp
    test_type_handler.cpp
    test_scope.cpp
)

find_package(Boost 1.88.0 REQUIRED COMPONENTS unit_test_framework)
add_executable(interpreter_tests ${INTERPRETER_TEST_SOURCES})
target_link_libraries(interpreter_tests
    Boost::unit_test_framework
    interpreter
)
target_include_directories(lexer_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME interpreter_tests COMMAND interpreter_tests)