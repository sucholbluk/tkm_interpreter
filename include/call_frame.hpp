#ifndef CALL_FRAME_HPP
#define CALL_FRAME_HPP
#include <deque>

#include "scope.hpp"

/**
 * @ingroup interpreter
 * @brief Call frame representation. Consists of its scopes.
 */
class CallFrame {
   public:
    CallFrame() : _scopes{Scope{}} {}

    void push_scope();
    void pop_scope();
    void add_variable(std::string identifier, sp_variable variable);
    sp_variable find_variable(const std::string& identifier);
    bool is_in_current_scope(const std::string& identifier);

   private:
    std::deque<Scope> _scopes;
};

#endif  // CALL_FRAME_HPP