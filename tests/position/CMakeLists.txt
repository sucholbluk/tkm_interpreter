set(POSITION_TEST_SOURCES
    test_position.cpp
)

add_executable(Position_UnitTests ${POSITION_TEST_SOURCES})
target_link_libraries(Position_UnitTests Boost::unit_test_framework)
target_include_directories(Position_UnitTests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME Position_UnitTests COMMAND Position_UnitTests)