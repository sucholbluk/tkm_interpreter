#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP
#include "variable.hpp"
/**
 * @ingroup interpreter
 * @brief Modules of function for type checking.
 */
namespace TypeChecker {

bool are_the_same_type(value lhs, value rhs);

bool matches_type(value val, VariableType type);

}  // namespace TypeChecker

#endif  // TYPE_CHECKER_HPP