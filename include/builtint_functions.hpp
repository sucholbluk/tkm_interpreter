#ifndef BUILTIN_FUNCTIONS_HPP
#define BUILTIN_FUNCTIONS_HPP
#include <array>
#include <functional>

#include "callable.hpp"

using function_impl = std::function<std::optional<value>(Interpreter&, arg_list)>;

/**
 * @ingroup interpreter
 * @brief Represents a builtin function
 *
 * Builtin functions do their task and store the result to the interpreter
 */
class BuiltinFunction : public Callable {
   public:
    /**
     * @brief BuiltinFunction Constructor
     * @param type The type of the builtin function
     * @param impl The function implementation
     */
    BuiltinFunction(Type type, function_impl impl);

    /**
     * @brief Calls the builtin function with the given arguments
     * @param interpreter Reference to the interpreter as result taker
     * @param call_args Arguments passed in a function call
     */
    void call(Interpreter& interpreter, arg_list call_args) override;

    /**
     * @brief Returns the type of the builtin function
     * @return The type of the function
     */
    Type get_type() const override;

    /**
     * @brief Virtual destructor.
     */
    virtual ~BuiltinFunction() = default;

   private:
    Type _type;
    function_impl _impl;
};

/**
 * @brief Structure holding information about a builtin function.
 */
struct BuiltinFunctionInfo {
    std::string identifier;
    Type type;
    function_impl impl;
};

/**
 * @brief Namespace for builtin functions.
 */
namespace Builtins {
/**
 * @brief Array containing information about all builtin functions.
 *
 * The environment uses it to initialize built-in functions
 */
extern const std::array<BuiltinFunctionInfo, 8> builtin_function_infos;
}  // namespace Builtins

#endif  // BUILTIN_FUNCTIONS_HPP