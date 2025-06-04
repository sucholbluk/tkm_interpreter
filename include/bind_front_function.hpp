#ifndef BIND_FRONT_FUNCTION_HPP
#define BIND_FRONT_FUNCTION_HPP
#include "callable.hpp"

class BindFrontFunction : public Callable {
   public:
    BindFrontFunction(Type type, sp_callable bind_target, arg_list bind_args);
    Type get_type() const override;
    void call(Interpreter& interpreter, arg_list call_args) override;

    virtual ~BindFrontFunction() = default;

   private:
    Type _type;
    sp_callable _target_func;
    arg_list _bound_args;
};
#endif  // BIND_FRONT_FUNCTION_HPP