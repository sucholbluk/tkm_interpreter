#ifndef BUILTIN_FUNCTIONS_HPP
#define BUILTIN_FUNCTIONS_HPP
#include <array>
#include <functional>

#include "callable.hpp"

using function_impl = std::function<std::optional<value>(Interpreter&, arg_list)>;
/**
 * @ingroup interpreter
 * @brief Buit-in print function.
 */
class BuiltinFunction : public Callable {
   public:
    BuiltinFunction(Type type, function_impl impl);
    void call(Interpreter& interpreter, arg_list call_args) override;
    Type get_type() const override;
    ~BuiltinFunction() = default;

   private:
    Type _type;
    function_impl _impl;
};

struct BuiltinFunctionInfo {
    std::string identifier;
    Type type;
    function_impl impl;
};

namespace Builtins {
extern const std::array<BuiltinFunctionInfo, 2> builtin_function_infos;
}  // namespace Builtins

#endif  // BUILTIN_FUNCTIONS_HPP