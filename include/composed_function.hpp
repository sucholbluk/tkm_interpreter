#ifndef COMPOSED_FUNCTION_HPP
#define COMPOSED_FUNCTION_HPP

#include "callable.hpp"

class ComposedFunction : public Callable {
   public:
    ComposedFunction(Type type, sp_callable first, sp_callable second);
    Type get_type() const override;
    void call(Interpreter& interpreter, arg_list call_args) override;

    virtual ~ComposedFunction() = default;

   private:
    Type _type;
    sp_callable _first_func;
    sp_callable _second_func;
};
#endif  // COMPOSED_FUNCTION_HPP