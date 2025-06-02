cmake_minimum_required(VERSION 3.22)

project(tkm_interpreter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++ -lc++abi)

link_directories(/usr/local/lib)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

include_directories(include)

include(CTest)
enable_testing()
set(CMAKE_CTEST_COMMAND ctest)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(tkm_interpreter src/main.cpp)
target_link_libraries(tkm_interpreter PRIVATE
    core
    lexer
)