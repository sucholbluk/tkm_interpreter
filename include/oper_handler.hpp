#ifndef OPER_HANDLER_HPP
#define OPER_HANDLER_HPP
#include "variable.hpp"

/**
 * @ingroup interpreter
 * @brief Operator handling utilities for arithmetic, comparison, logical, and function operations.
 */
namespace OperHandler {
/**
 * @brief Adds two values.
 * @param left Left operand.
 * @param right Right operand.
 * @return Result of addition.
 */
value add(value left, value right);

/**
 * @brief Subtracts right value from left value.
 * @param left Left operand.
 * @param right Right operand.
 * @return Result of subtraction.
 */
value subtract(value left, value right);

/**
 * @brief Multiplies two values.
 * @param left Left operand.
 * @param right Right operand.
 * @return Result of multiplication.
 */
value multiply(value left, value right);

/**
 * @brief Divides left value by right value.
 * @param left Left operand.
 * @param right Right operand.
 * @return Result of division.
 */
value divide(value left, value right);

/**
 * @brief Checks if two values are equal.
 * @param left Left operand.
 * @param right Right operand.
 * @return True if equal, false otherwise.
 */
bool check_eq(value left, value right);

/**
 * @brief Checks if two values are not equal.
 * @param left Left operand.
 * @param right Right operand.
 * @return True if not equal, false otherwise.
 */
bool check_neq(value left, value right);

/**
 * @brief Checks if left value is greater than right value.
 * @param left Left operand.
 * @param right Right operand.
 * @return True if left > right, false otherwise.
 */
bool check_gt(value left, value right);

/**
 * @brief Checks if left value is less than right value.
 * @param left Left operand.
 * @param right Right operand.
 * @return True if left < right, false otherwise.
 */
bool check_lt(value left, value right);

/**
 * @brief Checks if left value is greater than or equal to right value.
 * @param left Left operand.
 * @param right Right operand.
 * @return True if left >= right, false otherwise.
 */
bool check_gteq(value left, value right);

/**
 * @brief Checks if left value is less than or equal to right value.
 * @param left Left operand.
 * @param right Right operand.
 * @return True if left <= right, false otherwise.
 */
bool check_lteq(value left, value right);

/**
 * @brief Logical AND operation.
 * @param left Left operand.
 * @param right Right operand.
 * @return Result of logical AND.
 */
bool logical_and(value left, value right);

/**
 * @brief Logical OR operation.
 * @param left Left operand.
 * @param right Right operand.
 * @return Result of logical OR.
 */
bool logical_or(value left, value right);

/**
 * @brief Unary minus operation.
 * @param val Value to negate.
 * @return Negated value.
 */
value unary_minus(value val);

/**
 * @brief Logical NOT operation.
 * @param val Value to negate.
 * @return Result of logical NOT.
 */
bool logical_not(value val);

/**
 * @brief Composes two callables into a single callable.
 * @param left First callable value.
 * @param right Second callable value.
 * @return Shared pointer to composed callable.
 */
sp_callable compose_functions(value left, value right);

/**
 * @brief Binds arguments to the front of a callable function.
 * @param bind_target Target callable to bind arguments to.
 * @param args Arguments to bind.
 * @return Shared pointer to the new callable with bound arguments.
 */
sp_callable bind_front_function(sp_callable bind_target, arg_list args);

};  // namespace OperHandler

#endif  // OPER_HANDLER_HPP