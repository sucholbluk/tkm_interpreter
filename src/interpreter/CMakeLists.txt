add_library(interpreter STATIC interpreter.cpp global_function.cpp)

target_link_libraries(interpreter PUBLIC parser)

target_include_directories(interpreter PUBLIC ${CMAKE_SOURCE_DIR}/include)
