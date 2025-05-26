#ifndef VARIABLE_HPP
#define VARIABLE_HPP
#include <variant>

#include "type.hpp"

class Callable;
using value = std::variant<int, float, bool, std::string, std::shared_ptr<Callable>>;

/**
 * @ingroup interpreter
 * @brief Variable representation - type and value.
 */
struct Variable {
    Variable(VariableType type, value var_value);
    VariableType type;
    value var_value;
};

// to represent variable value
using sp_variable = std::shared_ptr<Variable>;
// argument can be variable reference or rvalue -> to let variables be modified inside other functions
using arg = std::variant<sp_variable, value>;
using arg_list = std::vector<arg>;

#endif