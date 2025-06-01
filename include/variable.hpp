#ifndef VARIABLE_HPP
#define VARIABLE_HPP
#include <variant>

#include "type.hpp"

class Callable;
using sp_callable = std::shared_ptr<Callable>;

using value = std::variant<int, double, bool, std::string, sp_callable>;

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

    template <typename T>
    T get_value_as() const;
};

// to represent variable value
// argument can be variable reference or rvalue -> to let variables be modified inside other functions
using vhold_or_val = std::variant<VariableHolder, value>;
using arg_list = std::vector<vhold_or_val>;
// contains value, reference to variable or nothing
using opt_vhold_or_val = std::optional<vhold_or_val>;

template <typename T>
T VariableHolder::get_value_as() const {
    if (auto value = std::get_if<T>(&var->var_value)) {
        return *value;
    }
    throw std::logic_error("impl err");  // TODO: change
}

std::ostream& operator<<(std::ostream& os, const VariableHolder& var_hold);
std::ostream& operator<<(std::ostream& os, const value& v);
std::ostream& operator<<(std::ostream& os, const vhold_or_val& a);
std::ostream& operator<<(std::ostream& os, const arg_list& args);
std::ostream& operator<<(std::ostream& os, const std::vector<VariableType>& v);
#endif  // VARIABLE_HPP