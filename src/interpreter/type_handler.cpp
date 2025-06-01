#include "type_handler.hpp"

#include <algorithm>
#include <ranges>

#include "callable.hpp"

namespace TypeHandler {
Type deduce_type(value val) {
    return std::visit(
        []<typename T>(const T& _val) -> Type {
            if constexpr (std::same_as<int, T>) {
                return Type{TypeKind::INT};
            } else if constexpr (std::same_as<double, T>) {
                return Type{TypeKind::FLOAT};
            } else if constexpr (std::same_as<bool, T>) {
                return Type{TypeKind::BOOL};
            } else if constexpr (std::same_as<std::string, T>) {
                return Type{TypeKind::STRING};
            } else if constexpr (std::same_as<sp_callable, T>) {
                return _val->get_type();
            }
        },
        val);
}

bool args_match_params(arg_list args, std::vector<VariableType> param_types) {
    if (args.size() != param_types.size()) return false;

    return std::ranges::all_of(std::views::iota(size_t{0}, args.size()),
                               [&](size_t i) { return arg_matches_param(args[i], param_types[i]); });
}

bool arg_matches_param(arg argument, VariableType param_type) {
    return std::visit(
        [param_type]<typename T>(const T& argument) -> bool {
            if constexpr (std::same_as<value, T>) {
                return deduce_type(argument) == param_type.type;
            } else if constexpr (std::same_as<VariableHolder, T>) {
                if (param_type.is_mutable and not argument.can_change_var) return false;

                // mutable variable can be passed as immutable variable - the other way no
                return argument.get_type() == param_type.type;
            }
        },
        argument);
}

arg maybe_value_to_arg(const mb_var_or_val& maybe_val_or_holder) {
    return std::visit(
        []<typename T>(const T& v_or_vh) -> arg {
            if constexpr (std::same_as<T, VariableHolder>) {
                return v_or_vh;
            } else if constexpr (std::same_as<T, value>) {
                return v_or_vh;
            } else if constexpr (std::same_as<T, std::monostate>) {
                throw std::logic_error("impl err - check if not monostate before calling");
            }
        },
        maybe_val_or_holder);
}

value extract_value(const mb_var_or_val& maybe_val_or_holder) {
    return std::visit(
        []<typename T>(const T& v_or_vh) -> value {
            if constexpr (std::same_as<T, VariableHolder>) {
                return v_or_vh.var->var_value;
            } else if constexpr (std::same_as<T, value>) {
                return v_or_vh;
            } else if constexpr (std::same_as<T, std::monostate>) {
                throw std::logic_error("impl err - should never be called with monostate");
            }
        },
        maybe_val_or_holder);
}

std::optional<value> as_type(Type type, value val) {
    switch (type.kind) {
        case TypeKind::INT:
            return as_int(val);
        case TypeKind::FLOAT:
            return as_float(val);
        case TypeKind::BOOL:
            return as_bool(val);
        case TypeKind::STRING:
            return as_string(val);
        default:
            return std::nullopt;
    }
}

std::optional<value> as_string(const value& val) {
    return std::visit(
        []<typename T>(const T& val) -> std::optional<value> {
            if constexpr (std::same_as<int, T>) {
                return std::format("{}", val);
            } else if constexpr (std::same_as<double, T>) {
                return std::format("{}", val);
            } else if constexpr (std::same_as<bool, T>) {
                return std::format("{}", val ? "true" : "false");
            } else if constexpr (std::same_as<std::string, T>) {
                return val;
            } else {
                return std::nullopt;
            }
        },
        val);
}

std::optional<value> as_int(const value& val) {
    return std::visit(
        []<typename T>(const T& val) -> std::optional<value> {
            if constexpr (std::same_as<int, T>) {
                return val;
            } else if constexpr (std::same_as<double, T>) {
                return (int)val;
            } else if constexpr (std::same_as<bool, T>) {
                return (int)val;
            } else if constexpr (std::same_as<std::string, T>) {
                try {
                    return std::stoi(val);
                } catch (const std::invalid_argument&) {
                    return std::nullopt;
                } catch (const std::out_of_range&) {
                    return std::nullopt;
                }
            } else {
                return std::nullopt;
            }
        },
        val);
}

std::optional<value> as_float(const value& val) {
    return std::visit(
        []<typename T>(const T& val) -> std::optional<value> {
            if constexpr (std::same_as<int, T>) {
                return (double)val;
            } else if constexpr (std::same_as<double, T>) {
                return val;
            } else if constexpr (std::same_as<bool, T>) {
                return (int)val;
            } else if constexpr (std::same_as<std::string, T>) {
                try {
                    return std::stod(val);
                } catch (const std::invalid_argument&) {
                    return std::nullopt;
                } catch (const std::out_of_range&) {
                    return std::nullopt;
                }
            } else {
                return std::nullopt;
            }
        },
        val);
}

std::optional<value> as_bool(const value& val) {
    return std::visit(
        []<typename T>(const T& val) -> std::optional<value> {
            if constexpr (std::same_as<int, T>) {
                return (bool)val;
            } else if constexpr (std::same_as<double, T>) {
                return (bool)val;
            } else if constexpr (std::same_as<bool, T>) {
                return val;
            } else if constexpr (std::same_as<std::string, T>) {
                return not val.empty();
            } else {
                return std::nullopt;
            }
        },
        val);
}

bool matches_return_type(const mb_var_or_val& ret_val, std::optional<Type> ret_type) {
    if (not ret_type) {  // returning none
        return std::holds_alternative<std::monostate>(ret_val);
    }
    return deduce_type(extract_value(ret_val)) == ret_type.value();
}

bool are_the_same_type(value lhs, value rhs) {
    return std::visit([]<typename T, typename U>(const T& left, const U& right) -> bool { return std::same_as<T, U>; },
                      lhs, rhs);
}

Type get_composed_func_type(value left, value right) {
    // it was already check if both left and right are the same type
    if (not std::holds_alternative<sp_callable>(left)) {
        throw std::runtime_error("Func comp only on functionns -- got ...");  // TODO
    }
    auto l_ftype_info{get_value_as<sp_callable>(left)->get_type().function_type_info};
    auto r_ftype_info{get_value_as<sp_callable>(right)->get_type().function_type_info};

    bool r_takes_1arg{r_ftype_info->param_types.size() == 1};

    if (not(r_takes_1arg and ret_type_matches_param_type(l_ftype_info->return_type, r_ftype_info->param_types[0]))) {
        throw std::runtime_error("invalid function types for function compositin {types}");
    }

    return Type{FunctionTypeInfo{l_ftype_info->param_types, r_ftype_info->return_type}};
}

bool ret_type_matches_param_type(std::optional<Type> ret_type, VariableType param_type) {
    if (not ret_type) return false;

    return ret_type.value() == param_type.type;
}

}  // namespace TypeHandler
