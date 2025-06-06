#ifndef GLOBAL_FUNCTION_HPP
#define GLOBAL_FUNCTION_HPP
#include "callable.hpp"

/**
 * @ingroup interpreter
 * @brief User defined function representation.
 */
class GlobalFunction : public Callable {
   public:
    /**
     * @brief GlobalFunction Constructor
     * @param function Reference to the function definition.
     */
    GlobalFunction(const FunctionDefinition& function);

    /**
     * @brief Calls the user-defined function with the given arguments.
     * @param interpreter Reference to the interpreter instance.
     * @param call_args List of arguments passed to the function.
     *
     * Call done by interpreter visiting the funciton.
     */
    void call(Interpreter& interpreter, arg_list call_args) override;

    /**
     * @brief Returns the type of the function.
     * @return Type of the function.
     */
    Type get_type() const override;

    /**
     * @brief Virtual Destructor.
     */
    virtual ~GlobalFunction() = default;

   private:
    const FunctionDefinition& _function;
};

/**
 * @brief Properties of obligatory main function
 */
namespace MainProperties {
/**
 * @brief Required main function type.
 */
extern const Type type;

/**
 * @brief Required main identifier
 */
extern const std::string main_identifier;
}  // namespace MainProperties

#endif  // GLOBAL_FUNCTION_HPP