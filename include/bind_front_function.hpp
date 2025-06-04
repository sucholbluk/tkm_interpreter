#ifndef BIND_FRONT_FUNCTION_HPP
#define BIND_FRONT_FUNCTION_HPP
#include "callable.hpp"

class BindFrontFunction : public Callable {
   public:
    BindFrontFunction(Type type, sp_callable bind_target, arg_list bind_args);
    Type get_type() const override;
    void call(Interpreter& interpreter, arg_list call_args) override;

    ~BindFrontFunction() = default;

   private:
    Type _type;
    arg_list _bound_args;
    sp_callable _target_func;
};
#endif  // BIND_FRONT_FUNCTION_HPP