#ifndef COMPOSED_FUNCTION_HPP
#define COMPOSED_FUNCTION_HPP

#include "callable.hpp"

/**
 * @ingroup interpreter
 * @brief Represents a function composed of two callable objects.
 *
 * ComposedFunction chains two callable objects, where the result of the first
 * is passed as the input to the second.
 */
class ComposedFunction : public Callable {
   public:
    /**
     * @brief Constructs a ComposedFunction.
     * @param type The type of the composed function.
     * @param first The first callable to be executed.
     * @param second The second callable to be executed with the result of the first.
     */
    ComposedFunction(Type type, sp_callable first, sp_callable second);

    /**
     * @brief Returns the type of the composed function.
     * @return The type of the function.
     */
    Type get_type() const override;

    /**
     * @brief Calls the composed function with the given arguments.
     *        Executes the first callable, then passes its result to the second callable.
     * @param interpreter Reference to the interpreter executing the call.
     * @param call_args Arguments passed to the composed function.
     */
    void call(Interpreter& interpreter, arg_list call_args) override;

    /**
     * @brief Virtual destructor.
     */
    virtual ~ComposedFunction() = default;

   private:
    Type _type;
    sp_callable _first_func;
    sp_callable _second_func;
};
#endif  // COMPOSED_FUNCTION_HPP