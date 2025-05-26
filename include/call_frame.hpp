#ifndef CALL_FRAME_HPP
#define CALL_FRAME_HPP
#include <stack>

#include "scope.hpp"

/**
 * @ingroup interpreter
 * @brief Call frame representation. Consists of its scopes.
 */
class CallFrame {
   public:
    void push_scope(const Scope& scope);
    void pop_scope();
    void add_variable(std::string identifier, sp_variable variable);
    sp_variable find_variable(const std::string& identifier);
    bool is_in_current_scope(const std::string& identifier);

   private:
    std::stack<Scope> _scope_stack;
};

#endif  // CALL_FRAME_HPP