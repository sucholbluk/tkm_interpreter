#ifndef BUILTIN_FUNCTIONS_HPP
#define BUILTIN_FUNCTIONS_HPP
#include "callable.hpp"

/**
 * @ingroup interpreter
 * @brief Buit-in print function.
 */
class PrintFunction : public Callable {
   public:
    void call(Interpreter& interpreter, arg_list call_args) override{};
    ~PrintFunction() = default;
};
#endif  // BUILTIN_FUNCTIONS_HPP