#include <cmath>

#include "builtint_functions.hpp"
#include "interpreter.hpp"
#include "type_handler.hpp"

BuiltinFunction::BuiltinFunction(Type type, function_impl impl) : _type{type}, _impl{impl} {}

void BuiltinFunction::call(Interpreter& interpreter, arg_list call_args) {
    auto opt_val = _impl(interpreter, call_args);
    if (opt_val)
        interpreter._tmp_result = opt_val.value();
    else
        interpreter._tmp_result = std::nullopt;
}

Type BuiltinFunction::get_type() const {
    return _type;
}

namespace Builtins {
const Type _print_type{
    FunctionTypeInfo{std::vector<VariableType>{{VariableType{Type{TypeKind::STRING}}}}, std::nullopt}};

function_impl _print_impl = [](Interpreter& interpreter, arg_list args) -> std::optional<value> {
    // interpreter already checked if arg_list matches taken params - here we have:
    // string value or variable holder(values passed as references) of string type
    std::cout << TypeHandler::get_value_as<std::string>(args[0]) << std::endl;
    return std::nullopt;
};

const Type _round_type{FunctionTypeInfo{std::vector<VariableType>{
                                            {VariableType{Type{TypeKind::FLOAT}}},
                                            {VariableType{Type{TypeKind::INT}}},
                                        },
                                        Type{TypeKind::FLOAT}}};

function_impl _round_impl = [](Interpreter& interpreter, arg_list args) -> std::optional<value> {
    // interpreter already checked if arg_list matches taken params - here we have:
    // string value or variable holder(values passed as references) of string type
    double to_roud{TypeHandler::get_value_as<double>(args[0])};
    int precision{TypeHandler::get_value_as<int>(args[1])};

    double factor = std::pow(10.0, precision);
    return (std::round(to_roud * factor)) / factor;
};

const std::array<BuiltinFunctionInfo, 2> builtin_function_infos{
    BuiltinFunctionInfo{
        "print",
        _print_type,
        _print_impl,
    },
    BuiltinFunctionInfo{
        "round",
        _round_type,
        _round_impl,
    },
};
}  // namespace Builtins