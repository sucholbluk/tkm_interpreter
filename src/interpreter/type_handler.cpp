#include "type_handler.hpp"

#include <algorithm>
#include <ranges>

#include "callable.hpp"
#include "exceptions.hpp"

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

bool args_match_params(const arg_list& args, std::vector<VariableType> param_types) {
    if (args.size() != param_types.size()) return false;

    return std::ranges::all_of(std::views::iota(size_t{0}, args.size()),
                               [&](size_t i) { return arg_matches_param(args[i], param_types[i]); });
}

bool arg_matches_param(vhold_or_val argument, VariableType param_type) {
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

vhold_or_val opt_value_to_arg(const opt_vhold_or_val& opt_v_or_vh) {
    if (not opt_v_or_vh) {
        throw ImplementationError("opt_value_to_arg should never be called before value check");
    }
    return opt_v_or_vh.value();
}

value extract_value(const opt_vhold_or_val& opt_v_or_vh) {
    if (not opt_v_or_vh) {
        throw ImplementationError("opt_value_to_arg should never be called before value check");
    }
    return extract_value(opt_v_or_vh.value());
}

value extract_value(const vhold_or_val& v_or_vh) {
    return std::visit(
        []<typename T>(const T& v_or_vh) -> value {
            if constexpr (std::same_as<T, VariableHolder>) {
                return v_or_vh.var->var_value;
            } else if constexpr (std::same_as<T, value>) {
                return v_or_vh;
            }
        },
        v_or_vh);
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
                    size_t idx;
                    int i = std::stoi(val, &idx);
                    if (idx != val.length()) return std::nullopt;
                    return i;
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
                return (double)val;
            } else if constexpr (std::same_as<std::string, T>) {
                try {
                    size_t idx;
                    double d = std::stod(val, &idx);
                    if (idx != val.length()) return std::nullopt;
                    return d;
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

bool matches_return_type(const opt_vhold_or_val& ret_val, std::optional<Type> ret_type) {
    if (not ret_val) {  // returning none
        return not ret_type.has_value();
    }
    return ret_type.has_value() and deduce_type(extract_value(ret_val)) == ret_type.value();
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

Type get_bind_front_func_type(sp_callable bind_target, const arg_list& args) {
    auto ftype_info{bind_target->get_type().function_type_info};
    auto param_types{ftype_info->param_types};

    if (args.size() > param_types.size()) {
        throw TooManyArgsToBindException(args.size(), param_types.size());
    }
    auto bound_params{std::vector<VariableType>(param_types.begin(), param_types.begin() + args.size())};

    if (not args_match_params(args, bound_params)) {
        throw ArgTypesNotMatchingException("Bind front", get_types_string(args), get_types_string(bound_params));
    }
    std::vector<VariableType> new_params{};
    if (args.size() < param_types.size()) {
        new_params = std::vector<VariableType>(param_types.begin() + args.size(), param_types.end());
    }
    return Type{FunctionTypeInfo{new_params, ftype_info->return_type}};
}

bool ret_type_matches_param_type(std::optional<Type> ret_type, VariableType param_type) {
    if (not ret_type) return false;

    return ret_type.value() == param_type.type;
}

std::string get_type_string(std::optional<Type> opt_type) {
    if (not opt_type) return "none";

    return opt_type.value().to_str();
}

std::string get_type_string(const opt_vhold_or_val& opt_v_or_vh) {
    if (not opt_v_or_vh) return "none";

    return deduce_type(extract_value(opt_v_or_vh)).to_str();
}

std::string get_type_string(const vhold_or_val& v_or_vh) {
    return std::visit(
        []<typename T>(const T& v_or_vh) -> std::string {
            if constexpr (std::same_as<VariableHolder, T>) {
                return std::string{v_or_vh.can_change_var ? "mut" : ""} + v_or_vh.get_type().to_str();
            } else if constexpr (std::same_as<value, T>) {
                return get_type_string(deduce_type(v_or_vh));
            }
        },
        v_or_vh);
}

std::string get_types_string(const arg_list& args) {
    std::string types_str{"("};
    bool first = true;
    for (const auto& arg : args) {
        if (!first) types_str += ", ";
        types_str += get_type_string(arg);
        first = false;
    }
    return types_str + ")";
}

std::string get_types_string(const std::vector<VariableType> params) {
    std::string types_str{"("};
    bool first = true;
    for (const auto& param : params) {
        if (!first) types_str += ", ";
        types_str += param.to_str();
        first = false;
    }
    return types_str + ")";
}

}  // namespace TypeHandler
