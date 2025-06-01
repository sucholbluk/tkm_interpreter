#include "builtint_functions.hpp"
#include "type_handler.hpp"

BuiltinFunction::BuiltinFunction(Type type, function_impl impl) : _type{type}, _impl{impl} {}

void BuiltinFunction::call(Interpreter& interpreter, arg_list call_args) {
    _impl(interpreter, call_args);
}

Type BuiltinFunction::get_type() const {
    return _type;
}

namespace Builtins {
const Type _print_type{
    FunctionTypeInfo{std::vector<VariableType>{{VariableType{Type{TypeKind::STRING}}}}, std::nullopt}};

function_impl _print_impl = [](Interpreter& interpreter, arg_list args) {
    // interpreter already checked if arg_list matches taken params - here we have:
    // string value or variable holder(values passed as references) of string type
    std::cout << TypeHandler::get_value_as<std::string>(args[0]) << std::endl;
};

const std::array<BuiltinFunctionInfo, 1> builtin_function_infos{{
    {
        "print",
        _print_type,
        _print_impl,
    },
}};

}  // namespace Builtins