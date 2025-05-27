#ifndef BUILTIN_FUNCTIONS_HPP
#define BUILTIN_FUNCTIONS_HPP
#include <functional>

#include "callable.hpp"

using function_impl = std::function<void(Interpreter&, arg_list)>;
/**
 * @ingroup interpreter
 * @brief Buit-in print function.
 */
class BuiltinFunction : public Callable {
   public:
    BuiltinFunction(Type type, function_impl& impl);
    void call(Interpreter& interpreter, arg_list call_args) override{};
    Type get_type() const override;
    ~BuiltinFunction() = default;

   private:
    Type _type;
    function_impl& _impl;
};

namespace Builtins {
extern const Type print_type;
extern function_impl print_impl;

}  // namespace Builtins

#endif  // BUILTIN_FUNCTIONS_HPP