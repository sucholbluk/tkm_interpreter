#include "oper_handler.hpp"

#include <cstdlib>
#include <limits>

#include "bind_front_function.hpp"
#include "composed_function.hpp"
#include "type_handler.hpp"

namespace OperHandler {

value add(value left, value right) {
    return std::visit(
        [right]<typename T>(const T& left) -> value {
            if constexpr (std::same_as<int, T>) {
                int rhs = TypeHandler::get_value_as<int>(right);
                if (std::abs((double)left + (double)rhs) > std::numeric_limits<int>::max()) {
                    throw std::runtime_error("int overflow");
                }
                return left + rhs;
            } else if constexpr (std::same_as<double, T>) {
                return left + TypeHandler::get_value_as<double>(right);
            } else if constexpr (std::same_as<std::string, T>) {
                return left + TypeHandler::get_value_as<std::string>(right);
            } else if constexpr (std::same_as<bool, T>) {
                throw std::runtime_error("cant perform addition on bools");
            } else if constexpr (std::same_as<sp_callable, T>) {
                throw std::runtime_error("cant perform addition on function types {with types}");
            }
        },
        left);
}

value subtract(value left, value right) {
    return std::visit(
        [right]<typename T>(const T& left) -> value {
            if constexpr (std::same_as<int, T>) {
                int rhs = TypeHandler::get_value_as<int>(right);
                if (std::abs((double)left - (double)rhs) > std::numeric_limits<int>::max()) {
                    throw std::runtime_error("int overflow");
                }
                return left - rhs;
            } else if constexpr (std::same_as<double, T>) {
                return left - TypeHandler::get_value_as<double>(right);
            } else if constexpr (std::same_as<std::string, T>) {
                throw std::runtime_error("cant perform subtraction on strings");
            } else if constexpr (std::same_as<bool, T>) {
                throw std::runtime_error("cant perform subtraction on bools");
            } else if constexpr (std::same_as<sp_callable, T>) {
                throw std::runtime_error("cant perform subtraction on functions {with types}");
            }
        },
        left);
}

value multiply(value left, value right) {
    return std::visit(
        [right]<typename T>(const T& left) -> value {
            if constexpr (std::same_as<int, T>) {
                int rhs = TypeHandler::get_value_as<int>(right);
                if (std::abs((double)left * (double)rhs) > std::numeric_limits<int>::max()) {
                    throw std::runtime_error("int overflow");
                }
                return left * rhs;
            } else if constexpr (std::same_as<double, T>) {
                return left * TypeHandler::get_value_as<double>(right);
            } else if constexpr (std::same_as<std::string, T>) {
                throw std::runtime_error("cant perform multiplication on strings");
            } else if constexpr (std::same_as<bool, T>) {
                throw std::runtime_error("cant perform multiplication on bools");
            } else if constexpr (std::same_as<sp_callable, T>) {
                throw std::runtime_error("cant perform multiplication on functions {with types}");
            }
        },
        left);
}

value divide(value left, value right) {
    return std::visit(
        [right]<typename T>(const T& left) -> value {
            if constexpr (std::same_as<int, T>) {
                int rhs = TypeHandler::get_value_as<int>(right);
                if (rhs == 0) {
                    throw std::runtime_error("division by zero err");
                }
                return left / rhs;
            } else if constexpr (std::same_as<double, T>) {
                auto rhs{TypeHandler::get_value_as<double>(right)};
                if (rhs == 0) {
                    throw std::runtime_error("divistion by zero err");
                }
                return left / rhs;
            } else if constexpr (std::same_as<std::string, T>) {
                throw std::runtime_error("cant perform divistion on strings");
            } else if constexpr (std::same_as<bool, T>) {
                throw std::runtime_error("cant perform divistion on bools");
            } else if constexpr (std::same_as<sp_callable, T>) {
                throw std::runtime_error("cant perform divistion on functions {with types}");
            }
        },
        left);
}

bool check_eq(value left, value right) {
    return std::visit(
        [right]<typename T>(const T& left) -> bool {
            if constexpr (std::same_as<int, T>) {
                return left == TypeHandler::get_value_as<int>(right);
            } else if constexpr (std::same_as<double, T>) {
                return left == TypeHandler::get_value_as<double>(right);
            } else if constexpr (std::same_as<std::string, T>) {
                return left == TypeHandler::get_value_as<std::string>(right);
            } else if constexpr (std::same_as<bool, T>) {
                return left == TypeHandler::get_value_as<bool>(right);
            } else if constexpr (std::same_as<sp_callable, T>) {
                throw std::runtime_error("cant perform comparison operation on functions {with types}");
            }
        },
        left);
}

bool check_neq(value left, value right) {
    return not check_eq(left, right);
}

bool check_lt(value left, value right) {
    return std::visit(
        [right]<typename T>(const T& left) -> bool {
            if constexpr (std::same_as<int, T>) {
                return left < TypeHandler::get_value_as<int>(right);
            } else if constexpr (std::same_as<double, T>) {
                return left < TypeHandler::get_value_as<double>(right);
            } else if constexpr (std::same_as<std::string, T>) {
                return left < TypeHandler::get_value_as<std::string>(right);
            } else if constexpr (std::same_as<bool, T>) {
                return left < TypeHandler::get_value_as<bool>(right);
            } else if constexpr (std::same_as<sp_callable, T>) {
                throw std::runtime_error("cant perform comparison operation on functions {with types}");
            }
        },
        left);
}

bool check_gt(value left, value right) {
    return check_lt(right, left);
}

bool check_gteq(value left, value right) {
    return not check_lt(left, right);
}

bool check_lteq(value left, value right) {
    return not check_gt(left, right);
}

bool logical_and(value left, value right) {
    if (auto lhs = std::get_if<bool>(&left)) {
        return *lhs and std::get<bool>(right);
    }
    throw std::runtime_error("logical and cannot operate on {type}");
}

bool logical_or(value left, value right) {
    if (auto lhs = std::get_if<bool>(&left)) {
        return *lhs or std::get<bool>(right);
    }
    throw std::runtime_error("logical or cannot operate on {type}");
}

// UNARY

bool logical_not(value val) {
    if (auto bool_val = std::get_if<bool>(&val)) {
        return not *bool_val;
    }
    throw std::runtime_error(std::string("cant call logical not with: ") + TypeHandler::deduce_type(val).to_str());
}

value unary_minus(value val) {
    return std::visit(
        []<typename T>(const T& val) -> value {
            if constexpr (std::same_as<int, T>) {
                return -val;
            } else if constexpr (std::same_as<double, T>) {
                return -val;
            } else {
                throw std::runtime_error(std::string{"unary minus doestn operate on: "} +
                                         TypeHandler::deduce_type(val).to_str());
            }
        },
        val);
}

sp_callable compose_functions(value left, value right) {
    Type type{TypeHandler::get_composed_func_type(left, right)};
    // if it passed through getting type, the values are functions and are correct
    return std::make_shared<ComposedFunction>(type, TypeHandler::get_value_as<sp_callable>(left),
                                              TypeHandler::get_value_as<sp_callable>(right));
}

sp_callable bind_front_function(value bind_target, arg_list args) {
    // save vars passed by reference as their values
    arg_list value_args{};
    std::ranges::for_each(args, [&](auto argument) { value_args.push_back(TypeHandler::extract_value(argument)); });
    Type bfront_type{TypeHandler::get_bind_front_func_type(bind_target, args)};
    return std::make_shared<BindFrontFunction>(bfront_type, TypeHandler::get_value_as<sp_callable>(bind_target),
                                               value_args);
}
}  // namespace OperHandler