#ifndef GLOBAL_FUNCTION_HPP
#define GLOBAL_FUNCTION_HPP
#include "callable.hpp"

/**
 * @ingroup interpreter
 * @brief User defined function representation.
 */
class GlobalFunction : public Callable {
   public:
    GlobalFunction(const FunctionDefinition& function);
    void call(Interpreter& interpreter, arg_list call_args) override;
    Type get_type() const override;
    ~GlobalFunction() = default;

   private:
    const FunctionDefinition& _function;
};

namespace MainProperties {
extern const Type type;
extern const std::string main_identifier;
}  // namespace MainProperties

#endif  // GLOBAL_FUNCTION_HPP