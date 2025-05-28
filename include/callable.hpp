#ifndef CALLABLE_HPP
#define CALLABLE_HPP
#include "expression.hpp"
#include "variable.hpp"

class Interpreter;
/**
 * @ingroup interpreter
 * @brief Base for all that can be called.
 */
class Callable {
   public:
    ~Callable() = default;
    virtual void call(Interpreter& interpreter, arg_list call_args) = 0;
    virtual Type get_type() const = 0;
};

std::ostream& operator<<(std::ostream& os, const Callable& callable);
std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Callable>& ptr);
#endif  // CALLABLE_HPP
