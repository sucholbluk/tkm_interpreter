#ifndef OPER_HANDLER_HPP
#define OPER_HANDLER_HPP
#include "variable.hpp"

namespace OperHandler {
// BINARY
// arithmetic
value add(value left, value right);
value subtract(value left, value right);
value multiply(value left, value right);
value divide(value left, value right);
// comparison
bool check_eq(value left, value right);
bool check_neq(value left, value right);

bool check_gt(value left, value right);
bool check_lt(value left, value right);
bool check_gteq(value left, value right);
bool check_lteq(value left, value right);

// logical
bool logical_and(value left, value right);
bool logical_or(value left, value right);

// UNARY
value unary_minus(value val);
bool logical_not(value val);

};  // namespace OperHandler

#endif  // OPER_HANDLER_HPP