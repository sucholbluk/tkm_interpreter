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
    /**
     * @brief Calls the callable object with the given interpreter and arguments.
     * @param interpreter Reference to the interpreter executing the call.
     * @param call_args List of arguments passed to the callable - can be variables or values.
     */
    virtual void call(Interpreter& interpreter, arg_list call_args) = 0;
    /**
     * @brief Returns the type of the callable object.
     * @return The type of the callable.
     */
    virtual Type get_type() const = 0;
};
using sp_callable = std::shared_ptr<Callable>;

std::ostream& operator<<(std::ostream& os, const Callable& callable);
std::ostream& operator<<(std::ostream& os, const sp_callable& ptr);

#endif  // CALLABLE_HPP
