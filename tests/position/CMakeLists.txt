set(POSITION_TEST_SOURCES
    test_position.cpp
)
find_package(Boost 1.88.0 REQUIRED COMPONENTS unit_test_framework)
add_executable(position_unit_tests ${POSITION_TEST_SOURCES})
target_link_libraries(position_unit_tests
    Boost::unit_test_framework
    position
)
target_include_directories(position_unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME position_unit_tests COMMAND position_unit_tests)