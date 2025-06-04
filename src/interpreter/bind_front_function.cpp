#include "bind_front_function.hpp"

BindFrontFunction::BindFrontFunction(Type type, sp_callable bind_target, arg_list bind_args)
    : _type{type}, _target_func{bind_target}, _bound_args{bind_args} {}

void BindFrontFunction::call(Interpreter& interpreter, arg_list call_args) {
    arg_list all_args{_bound_args};
    all_args.insert(all_args.end(), call_args.begin(), call_args.end());
    _target_func->call(interpreter, all_args);
}

Type BindFrontFunction::get_type() const {
    return _type;
}