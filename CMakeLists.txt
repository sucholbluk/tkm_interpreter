cmake_minimum_required(VERSION 3.22)

project(TKM_Compiler)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

include(CTest)
enable_testing()
set(CMAKE_CTEST_COMMAND ctest)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(TKM_Compiler src/main.cpp)