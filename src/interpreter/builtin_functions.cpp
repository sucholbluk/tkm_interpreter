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

// comment for _impls interpreter already checked if arg_list matches taken params - here we have:
// string value or variable holder(values passed as references) of string type

function_impl _print_impl = [](Interpreter& interpreter, arg_list args) -> std::optional<value> {
    std::cout << TypeHandler::get_value_as<std::string>(args[0]) << std::endl;
    return std::nullopt;
};

const Type _round_type{FunctionTypeInfo{std::vector<VariableType>{
                                            {VariableType{Type{TypeKind::FLOAT}}},
                                            {VariableType{Type{TypeKind::INT}}},
                                        },
                                        Type{TypeKind::FLOAT}}};

function_impl _round_impl = [](Interpreter& interpreter, arg_list args) -> std::optional<value> {
    double to_roud{TypeHandler::get_value_as<double>(args[0])};
    int precision{TypeHandler::get_value_as<int>(args[1])};

    double factor = std::pow(10.0, precision);
    return (std::round(to_roud * factor)) / factor;
};

const Type _input_type{FunctionTypeInfo{{}, Type{TypeKind::STRING}}};

function_impl _input_impl = [](Interpreter& interpreter, arg_list args) -> std::optional<value> {
    std::string line;
    std::getline(std::cin, line);
    return line;
};
const Type _is_int_type{FunctionTypeInfo{std::vector<VariableType>{
                                             {VariableType{Type{TypeKind::STRING}}},
                                         },
                                         Type{TypeKind::BOOL}}};

function_impl _is_int_impl = [](Interpreter& interpreter, arg_list args) -> std::optional<value> {
    return TypeHandler::as_int(TypeHandler::extract_value(args[0])).has_value();
};

const Type _is_float_type{FunctionTypeInfo{std::vector<VariableType>{
                                               {VariableType{Type{TypeKind::STRING}}},
                                           },
                                           Type{TypeKind::BOOL}}};

function_impl _is_float_impl = [](Interpreter& interpreter, arg_list args) -> std::optional<value> {
    return TypeHandler::as_float(TypeHandler::extract_value(args[0])).has_value();
};

const Type _lower_type{FunctionTypeInfo{std::vector<VariableType>{
                                            {VariableType{Type{TypeKind::STRING}}},
                                        },
                                        Type{TypeKind::STRING}}};

function_impl _lower_impl = [](Interpreter& interpreter, arg_list args) -> std::optional<value> {
    std::string s = TypeHandler::get_value_as<std::string>(args[0]);
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
};

const Type _upper_type{FunctionTypeInfo{std::vector<VariableType>{
                                            {VariableType{Type{TypeKind::STRING}}},
                                        },
                                        Type{TypeKind::STRING}}};

function_impl _upper_impl = [](Interpreter& interpreter, arg_list args) -> std::optional<value> {
    std::string s = TypeHandler::get_value_as<std::string>(args[0]);
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
    return s;
};

const Type _capitalized_type{FunctionTypeInfo{std::vector<VariableType>{
                                                  {VariableType{Type{TypeKind::STRING}}},
                                              },
                                              Type{TypeKind::STRING}}};

function_impl _capitalized_impl = [](Interpreter& interpreter, arg_list args) -> std::optional<value> {
    std::string s = TypeHandler::get_value_as<std::string>(args[0]);
    if (!s.empty()) {
        s[0] = std::toupper(static_cast<unsigned char>(s[0]));
        std::transform(s.begin() + 1, s.end(), s.begin() + 1, [](unsigned char c) { return std::tolower(c); });
    }
    return s;
};

const std::array<BuiltinFunctionInfo, 8> builtin_function_infos{
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
    BuiltinFunctionInfo{
        "input",
        _input_type,
        _input_impl,
    },
    BuiltinFunctionInfo{
        "is_int",
        _is_int_type,
        _is_int_impl,
    },
    BuiltinFunctionInfo{
        "is_float",
        _is_float_type,
        _is_float_impl,
    },
    BuiltinFunctionInfo{
        "lower",
        _lower_type,
        _lower_impl,
    },
    BuiltinFunctionInfo{
        "upper",
        _upper_type,
        _upper_impl,
    },
    BuiltinFunctionInfo{
        "capitalized",
        _capitalized_type,
        _capitalized_impl,
    },
};
}  // namespace Builtins