add_library(core STATIC safe_exec.cpp program_.cpp)

target_include_directories(lexer PUBLIC ${CMAKE_SOURCE_DIR}/include)
find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(core PRIVATE spdlog fmt Boost::program_options)