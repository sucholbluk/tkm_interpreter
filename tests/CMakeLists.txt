

add_subdirectory(position)
add_subdirectory(source_handler)
add_subdirectory(lexer)
add_subdirectory(parser)

set(TEST_SOURCES
    test_main.cpp
)
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_STATIC_LIBS ON) 
find_package(Boost 1.88.0 REQUIRED COMPONENTS unit_test_framework program_options)


link_directories(/usr/local/lib)
add_executable(tkm_tests ${TEST_SOURCES})
target_link_libraries(tkm_tests Boost::unit_test_framework)
target_include_directories(tkm_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME tkm_tests COMMAND tkm_tests)
