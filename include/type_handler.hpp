#ifndef TYPE_HANDLER_HPP
#define TYPE_HANDLER_HPP

#include "type.hpp"
#include "variable.hpp"

/**
 * @ingroup interpreter
 * @brief Modules of function for type handling.
 */
namespace TypeHandler {

bool are_the_same_type(value lhs, value rhs);

bool matches_type(value val, VariableType type);

Type deduce_type(value val);

bool args_match_params(arg_list args, std::vector<VariableType> param_types);

bool arg_matches_param(arg argument, VariableType param_type);

}  // namespace TypeHandler

#endif  // TYPE_HANDLER_HPP