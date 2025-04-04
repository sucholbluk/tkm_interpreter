
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_subdirectory(position)

set(TEST_SOURCES
    test_main.cpp
)

add_executable(TKM_Tests ${TEST_SOURCES})
target_link_libraries(TKM_Tests Boost::unit_test_framework)
target_include_directories(TKM_Tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME TKM_Tests COMMAND TKM_Tests)
