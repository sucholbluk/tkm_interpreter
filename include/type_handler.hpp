#ifndef TYPE_HANDLER_HPP
#define TYPE_HANDLER_HPP

#include "type.hpp"
#include "variable.hpp"

/**
 * @ingroup interpreter
 * @brief Modules of function for type handling.
 */
namespace TypeHandler {

Type deduce_type(value val);

Type get_composed_func_type(value left, value right);

Type get_bind_front_func_type(value bind_target, const arg_list& args);

bool are_the_same_type(value lhs, value rhs);

bool matches_return_type(const opt_vhold_or_val& ret_val, std::optional<Type> ret_type);

bool args_match_params(const arg_list& args, std::vector<VariableType> param_types);

bool arg_matches_param(vhold_or_val argument, VariableType param_type);

vhold_or_val opt_value_to_arg(const opt_vhold_or_val& maybe_val_or_holder);

value extract_value(const opt_vhold_or_val& maybe_val_or_holder);

value extract_value(const vhold_or_val& val_or_holder);

bool ret_type_matches_param_type(std::optional<Type> ret_type, VariableType param_type);

std::optional<value> as_type(Type type, value val);

std::optional<value> as_int(const value& val);

std::optional<value> as_float(const value& val);

std::optional<value> as_bool(const value& val);

std::optional<value> as_string(const value& val);

template <typename T>
bool value_type_is(const vhold_or_val& opt_v_or_vh) {
    return std::visit(
        []<typename U>(const U& v_or_vh) -> bool {
            if constexpr (std::same_as<VariableHolder, U>) {
                return std::holds_alternative<T>(v_or_vh.var->var_value);
            } else if constexpr (std::same_as<value, U>) {
                return std::holds_alternative<T>(v_or_vh);
            }
        },
        opt_v_or_vh);
}

template <typename T>
bool value_type_is(const opt_vhold_or_val& opt_v_or_vh) {
    if (not opt_v_or_vh) return false;
    return value_type_is<T>(opt_v_or_vh.value());
}

template <typename T>
T get_value_as(const value& val) {
    if (auto value = std::get_if<T>(&val)) {
        return *value;
    }
    throw std::logic_error("impl err");  // TODO: change
}

template <typename T>
T get_value_as(const vhold_or_val& argument) {
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
T get_value_as(opt_vhold_or_val opt_v_or_vh) {
    if (not opt_v_or_vh) {
        throw std::logic_error("impl err get value as with monostate");  // TODO: change
    }
    return get_value_as<T>(opt_v_or_vh.value());
}

}  // namespace TypeHandler

#endif  // TYPE_HANDLER_HPP