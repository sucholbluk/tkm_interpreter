add_library(interpreter STATIC 
    interpreter.cpp
    global_function.cpp
    builtin_functions.cpp
    type_handler.cpp
    scope.cpp
    call_frame.cpp
    "environment.cpp"
    variable.cpp
    callable.cpp
)

target_link_libraries(interpreter PUBLIC parser)

target_include_directories(interpreter PUBLIC ${CMAKE_SOURCE_DIR}/include)
