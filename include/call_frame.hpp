#ifndef CALL_FRAME_HPP
#define CALL_FRAME_HPP
#include <deque>

#include "scope.hpp"
#include "type.hpp"

/**
 * @ingroup interpreter
 * @brief Call frame representation. Consists of its scopes.
 */
class CallFrame {
   public:
    /**
     * @brief Constructs a CallFrame with an optional return type.
     *        Initializes the frame with a single (parameter) scope.
     *
     * Scope is added by default to hold function arguments.
     *
     * @param return_type The expected return type of the function, or std::nullopt.
     *
     */
    CallFrame(std::optional<Type> return_type) : _scopes{Scope{}}, _return_type{return_type} {}

    /**
     * @brief Pushes a new scope onto the scope stack.
     */
    void push_scope();

    /**
     * @brief Pops the top scope from the scope stack.
     */
    void pop_scope();

    /**
     * @brief Adds a variable to the current scope.
     * @param identifier The variable name.
     * @param variable The variable holder.
     */
    void add_variable(std::string identifier, VariableHolder variable);

    /**
     * @brief Searches for a variable by name in all scopes (from top to bottom).
     * @param identifier The variable name.
     * @return The variable holder if found, otherwise std::nullopt.
     */
    std::optional<VariableHolder> find_variable(const std::string& identifier);

    /**
     * @brief Checks if a variable exists in the current (top) scope.
     * @param identifier The variable name.
     * @return True if the variable exists in the current scope, false otherwise.
     *
     * Used mainly to decide if new value can be initialized
     */
    bool is_in_current_scope(const std::string& identifier);

    /**
     * @brief Returns the expected return type of current funtion - to which the call frame belongs
     * @return The return type, or std::nullopt if not set.
     */
    std::optional<Type> get_ret_type() const;

   private:
    std::deque<Scope> _scopes;
    std::optional<Type> _return_type;
};

#endif  // CALL_FRAME_HPP