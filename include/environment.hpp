#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
#include <stack>

#include "call_frame.hpp"
#include "callable.hpp"
#include "variable.hpp"

/**
 * @ingroup interpreter
 * @brief Represents the runtime environment for the interpreter.
 *
 * Manages function definitions, variable scopes, and call frames.
 * It provides mechanisms for variable declaration, function registration, and scope management
 * during program execution.
 */
class Environment {
   public:
    /**
     * @brief Empty constructor
     */
    Environment();

    /**
     * @brief Registers a global function definition in the environment.
     * @param function The function definition to register.
     */
    void register_function(const FunctionDefinition& function);

    /**
     * @brief Declares a variable with a given identifier and variable holder.
     * @param identifier The variable name.
     * @param var_holder The variable holder.
     */
    void declare_variable(std::string identifier, VariableHolder var_holder);

    /**
     * @brief Declares a variable with a given identifier, type, and value.
     * @param identifier The variable name.
     * @param var_type The type of the variable.
     * @param var_value The value of the variable.
     *
     * Sometimes it was easier not to constructo the VariableHolder object in the interpreter.
     */
    void declare_variable(std::string identifier, VariableType var_type, value var_value);

    /**
     * @brief Pushes new CallFrame to the call frame stac. Used when calling function.
     * @param ret_type The expected return type std::nullopt == return type is none.
     */
    void calling_function(std::optional<Type> ret_type);

    /**
     * @brief Exits the current function - pops the call frame.
     */
    void exiting_function();

    /**
     * @brief Gets the return type of the current function.
     * @return The return type if std::nulloopt - then returns none
     */
    std::optional<Type> get_cur_func_ret_type() const;

    /**
     * @brief Adds a new scope to the current call frame.
     */
    void add_scope();

    /**
     * @brief Pops the top scope from the current call frame.
     */
    void pop_scope();

    /**
     * @brief Checks if a variable can be defined in the current scope.
     * @param identifier The variable name.
     * @return True if the variable can be defined, false otherwise.
     *
     * can never define variables with the same name as some global function
     */
    bool can_define(const std::string& identifier);

    /**
     * @brief Retrieves a variable by its identifier from the environment.
     * @param identifier The variable name.
     * @return The variable holder if found, otherwise std::nullopt.
     */
    std::optional<VariableHolder> get_by_identifier(const std::string& identifier);

    /**
     * @brief Retrieves a global function by its identifier.
     * @param identifier The function name.
     * @return Shared pointer to the callable if found.
     */
    sp_callable get_global_function(const std::string& identifier);

   private:
    std::unordered_map<std::string, sp_callable> _functions;
    std::stack<CallFrame> _call_frames;
};
#endif  // ENVIRONMENT_HPP
