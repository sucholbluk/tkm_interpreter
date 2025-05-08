add_library(core STATIC safe_exec.cpp cli_app.cpp)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/include)
find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(core PRIVATE spdlog fmt Boost::program_options lexer parser)