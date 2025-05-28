#include "builtint_functions.hpp"

BuiltinFunction::BuiltinFunction(Type type, function_impl& impl) : _type{type}, _impl{impl} {}

namespace Builtins {
const Type _print_type{FunctionTypeInfo{std::vector<VariableType>{VariableType{Type{TypeKind::STRING}}}, std::nullopt}};

function_impl _print_impl = [](Interpreter& interpreter, arg_list args) {

};

const std::array<BuiltinFunctionInfo, 1> builtin_function_infos{{
    {
        "print",
        _print_type,
    },
}};

}  // namespace Builtins