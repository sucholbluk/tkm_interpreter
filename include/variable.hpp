#ifndef VARIABLE_HPP
#define VARIABLE_HPP
#include <variant>

#include "type.hpp"

class Callable;
using value = std::variant<int, double, bool, std::string, std::shared_ptr<Callable>>;

/**
 * @ingroup interpreter
 * @brief Variable representation - type and value.
 */
struct Variable {
    VariableType type;
    value var_value;
};

using sp_variable = std::shared_ptr<Variable>;

/**
 * @ingroup interpreter
 * @brief Variable holder - variables can be mutable in some scope and const in other.
 */
struct VariableHolder {
    VariableHolder() = default;
    VariableHolder(sp_variable var);
    VariableHolder(sp_variable var, bool can_change_var);
    Type get_type() const;
    sp_variable var;
    bool can_change_var;
};

// to represent variable value
// argument can be variable reference or rvalue -> to let variables be modified inside other functions
using arg = std::variant<VariableHolder, value>;
using arg_list = std::vector<arg>;

std::ostream& operator<<(std::ostream& os, const VariableHolder& var_hold);
std::ostream& operator<<(std::ostream& os, const value& v);
std::ostream& operator<<(std::ostream& os, const arg& a);
std::ostream& operator<<(std::ostream& os, const std::vector<arg>& args);
std::ostream& operator<<(std::ostream& os, const std::vector<VariableType>& v);
#endif