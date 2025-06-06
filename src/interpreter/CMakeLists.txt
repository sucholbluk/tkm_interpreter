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
    oper_handler.cpp
    composed_function.cpp
    bind_front_function.cpp
)

target_link_libraries(interpreter PUBLIC parser exceptions)

target_include_directories(interpreter PUBLIC ${CMAKE_SOURCE_DIR}/include)
