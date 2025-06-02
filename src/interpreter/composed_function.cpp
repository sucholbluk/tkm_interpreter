#include "composed_function.hpp"

#include "interpreter.hpp"
#include "type_handler.hpp"

ComposedFunction::ComposedFunction(Type type, sp_callable first, sp_callable second)
    : _type{type}, _first_func{first}, _second_func{second} {}

Type ComposedFunction::get_type() const {
    return _type;
}

void ComposedFunction::call(Interpreter& interpreter, arg_list call_args) {
    interpreter._env.calling_function(_first_func->get_type().function_type_info->return_type);
    _first_func->call(interpreter, call_args);
    interpreter._env.exiting_function();

    arg_list second_func_args = {TypeHandler::opt_value_to_arg(interpreter._tmp_result)};
    interpreter._clear_tmp_result();
    interpreter._env.calling_function(_second_func->get_type().function_type_info->return_type);
    _second_func->call(interpreter, second_func_args);
    interpreter._env.exiting_function();
}