#include "builtint_functions.hpp"

BuiltinFunction::BuiltinFunction(Type type, function_impl& impl) : _type{type}, _impl{impl} {}

namespace Builtins {
const Type _print_type{FunctionTypeInfo{std::vector<VariableType>{VariableType{Type{TypeKind::STRING}}}, std::nullopt}};

// function_impl _print_impl = [](Interpreter& interpreter, arg_list args) {
//     // interpreter will check if arg_list matches taken params - here we have:
//     // string value or variable holder(values passed as references) of string type
//     arg argument{args[0]};  // if somehow is an error with logic and it passes without arg i want the error here
//     std::visit(
//         []<typename T>(const T& argument) {
//             if constexpr (std::same_as<value, T>) {
//                 std::cout << std::get<std::string>(argument) << std::endl;
//             } else if constexpr (std::same_as<VariableHolder>)
//         },
//         argument);
// };

// const std::array<BuiltinFunctionInfo, 1> builtin_function_infos{{
//     {
//         "print",
//         _print_type,
//         _print_impl,
//     },
// }};

}  // namespace Builtins