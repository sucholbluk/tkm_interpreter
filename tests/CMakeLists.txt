
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_subdirectory(position)
add_subdirectory(source_handler)
add_subdirectory(lexer)

set(TEST_SOURCES
    test_main.cpp
)

find_package(Boost REQUIRED)

add_executable(tkm_tests ${TEST_SOURCES})
target_link_libraries(tkm_tests Boost::unit_test_framework)
target_include_directories(tkm_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME tkm_tests COMMAND tkm_tests)
