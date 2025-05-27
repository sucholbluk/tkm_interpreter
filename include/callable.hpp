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

inline std::ostream& operator<<(std::ostream& os, const Callable& callable) {
    os << std::format("Callable - {}]", callable.get_type().to_str());
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Callable>& ptr) {
    if (ptr)
        os << std::format("[sp to Callable - {}]", ptr->get_type().to_str());
    else
        os << "[sp to Callable - nullptr]";
    return os;
}
#endif  // CALLABLE_HPP
