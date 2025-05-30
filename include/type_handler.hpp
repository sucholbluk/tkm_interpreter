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

bool matches_return_type(const std::variant<std::monostate, VariableHolder, value>& ret_val,
                         std::optional<Type> ret_type);

Type deduce_type(value val);

bool args_match_params(arg_list args, std::vector<VariableType> param_types);

bool arg_matches_param(arg argument, VariableType param_type);

arg maybe_value_to_arg(const std::variant<std::monostate, VariableHolder, value>& maybe_val_or_holder);

value extract_value(const std::variant<std::monostate, VariableHolder, value>& maybe_val_or_holder);

std::optional<value> as_type(Type primitive_type_kind, value val);

std::optional<value> as_int(const value& val);

std::optional<value> as_float(const value& val);

std::optional<value> as_bool(const value& val);

std::optional<value> as_string(const value& val);

template <typename T>
bool value_type_is(const std::variant<std::monostate, VariableHolder, value>& maybe_val_or_holder) {
    return std::visit(
        []<typename U>(const U& v_or_vh) -> bool {
            if constexpr (std::same_as<VariableHolder, U>) {
                return std::holds_alternative<T>(v_or_vh.var->var_value);
            } else if constexpr (std::same_as<value, U>) {
                return std::holds_alternative<T>(v_or_vh);
            }
            return false;
        },
        maybe_val_or_holder);
}

template <typename T>
T get_value_as(const value& val) {
    if (auto value = std::get_if<T>(&val)) {
        return *value;
    }
    throw std::logic_error("impl err");  // TODO: change
}

template <typename T>
T get_value_as(const std::variant<VariableHolder, value>& argument) {
    return std::visit(
        []<typename U>(const U& argument) -> T {
            if constexpr (std::same_as<VariableHolder, U>) {
                return argument.template get_value_as<T>();
            } else if constexpr (std::same_as<value, U>) {
                return get_value_as<T>(argument);
            }
        },
        argument);
}

template <typename T>
T get_value_as(std::variant<std::monostate, VariableHolder, value> maybe_val_or_holder) {
    return std::visit(
        []<typename U>(const U& v_or_vh) -> T {
            if constexpr (std::same_as<VariableHolder, U>) {
                return v_or_vh.template get_value_as<T>();
            } else if constexpr (std::same_as<value, U>) {
                return get_value_as<T>(v_or_vh);
            } else if constexpr (std::same_as<std::monostate, U>) {
                throw std::logic_error("impl err get value as with monostate");  // TODO: change
            }
        },
        maybe_val_or_holder);
}

}  // namespace TypeHandler

#endif  // TYPE_HANDLER_HPP