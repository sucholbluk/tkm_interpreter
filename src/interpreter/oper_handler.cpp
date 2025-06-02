#include "oper_handler.hpp"

#include <cstdlib>
#include <limits>

#include "bind_front_function.hpp"
#include "composed_function.hpp"
#include "exceptions.hpp"
#include "type_handler.hpp"

namespace OperHandler {

value add(value left, value right) {
    return std::visit(
        [right]<typename T>(const T& left) -> value {
            if constexpr (std::same_as<int, T>) {
                int rhs = TypeHandler::get_value_as<int>(right);
                if (std::abs((double)left + (double)rhs) > std::numeric_limits<int>::max()) {
                    throw IntOverflowException();
                }
                return left + rhs;
            } else if constexpr (std::same_as<double, T>) {
                return left + TypeHandler::get_value_as<double>(right);
            } else if constexpr (std::same_as<std::string, T>) {
                return left + TypeHandler::get_value_as<std::string>(right);
            } else {
                throw CantPerformOperationException(expr_kind_to_str(ExprKind::ADDITION),
                                                    TypeHandler::deduce_type(left).to_str());
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
                    throw IntOverflowException();
                }
                return left - rhs;
            } else if constexpr (std::same_as<double, T>) {
                return left - TypeHandler::get_value_as<double>(right);
            } else {
                throw CantPerformOperationException(expr_kind_to_str(ExprKind::SUBTRACTION),
                                                    TypeHandler::deduce_type(left).to_str());
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
                    throw IntOverflowException();
                }
                return left * rhs;
            } else if constexpr (std::same_as<double, T>) {
                return left * TypeHandler::get_value_as<double>(right);
            } else {
                throw CantPerformOperationException(expr_kind_to_str(ExprKind::MULTIPICATION),
                                                    TypeHandler::deduce_type(left).to_str());
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
                    throw DivByZeroException();
                }
                return left / rhs;
            } else if constexpr (std::same_as<double, T>) {
                auto rhs{TypeHandler::get_value_as<double>(right)};
                if (rhs == 0) {
                    throw DivByZeroException();
                }
                return left / rhs;
            } else {
                throw CantPerformOperationException(expr_kind_to_str(ExprKind::DIVISION),
                                                    TypeHandler::deduce_type(left).to_str());
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
            } else {  // all comparison operators are implemented usind == and < - so the debug info is limited
                // could havae also be done with just < operator, but i decided not to
                throw CantPerformOperationException("Comparison", TypeHandler::deduce_type(left).to_str());
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
            } else {
                throw CantPerformOperationException("Comparison", TypeHandler::deduce_type(left).to_str());
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
    throw CantPerformOperationException(expr_kind_to_str(ExprKind::LOGICAL_AND),
                                        TypeHandler::deduce_type(left).to_str());
}

bool logical_or(value left, value right) {
    if (auto lhs = std::get_if<bool>(&left)) {
        return *lhs or std::get<bool>(right);
    }
    throw CantPerformOperationException(expr_kind_to_str(ExprKind::LOGICAL_OR),
                                        TypeHandler::deduce_type(left).to_str());
}

// UNARY

bool logical_not(value val) {
    if (auto bool_val = std::get_if<bool>(&val)) {
        return not *bool_val;
    }
    throw CantPerformOperationException(expr_kind_to_str(ExprKind::LOGICAL_NOT),
                                        TypeHandler::deduce_type(val).to_str());
}

value unary_minus(value val) {
    return std::visit(
        []<typename T>(const T& val) -> value {
            if constexpr (std::same_as<int, T>) {
                return -val;
            } else if constexpr (std::same_as<double, T>) {
                return -val;
            } else {
                throw CantPerformOperationException(expr_kind_to_str(ExprKind::LOGICAL_NOT),
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

sp_callable bind_front_function(sp_callable bind_target, arg_list args) {
    // save vars passed by reference as their values
    arg_list value_args{};
    std::ranges::for_each(args, [&](auto argument) { value_args.push_back(TypeHandler::extract_value(argument)); });
    Type bfront_type{TypeHandler::get_bind_front_func_type(bind_target, args)};
    return std::make_shared<BindFrontFunction>(bfront_type, bind_target, value_args);
}
}  // namespace OperHandler