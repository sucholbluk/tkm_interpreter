#ifndef BIND_FRONT_FUNCTION_HPP
#define BIND_FRONT_FUNCTION_HPP
#include "callable.hpp"

/**
 * @ingroup interpreter
 * @brief Callable object that binds arguments to the front of another callable.
 *
 */
class BindFrontFunction : public Callable {
   public:
    /**
     * @brief BindFrontFunction Constructor.
     * @param type The type of the callable - after binding arguments.
     * @param bind_target The target callable to bind arguments to.
     * @param bind_args The arguments to bind to the front.
     */
    BindFrontFunction(Type type, sp_callable bind_target, arg_list bind_args);
    /**
     * @brief Returns the type of the callable object.
     * @return The type of the callable.
     */
    Type get_type() const override;
    /**
     * @brief Calls the target callable with bound arguments followed by call_args.
     * @param interpreter Reference to the interpreter executing the call.
     * @param call_args Arguments passed during the call.
     */
    void call(Interpreter& interpreter, arg_list call_args) override;

    /**
     * @brief Virtual destructor.
     */
    virtual ~BindFrontFunction() = default;

   private:
    Type _type;
    sp_callable _target_func;
    arg_list _bound_args;
};
#endif  // BIND_FRONT_FUNCTION_HPP