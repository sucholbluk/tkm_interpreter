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
    // scope for function params
    CallFrame(std::optional<Type> return_type) : _scopes{Scope{}}, _return_type{return_type} {}

    void push_scope();
    void pop_scope();
    void add_variable(std::string identifier, VariableHolder variable);
    std::optional<VariableHolder> find_variable(const std::string& identifier);
    bool is_in_current_scope(const std::string& identifier);
    std::optional<Type> get_ret_type() const;

   private:
    std::deque<Scope> _scopes;
    std::optional<Type> _return_type;
};

#endif  // CALL_FRAME_HPP