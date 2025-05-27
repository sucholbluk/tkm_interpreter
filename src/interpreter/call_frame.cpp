#include "call_frame.hpp"

void CallFrame::push_scope() {
    _scopes.push_front(Scope{});
}

void CallFrame::pop_scope() {
    _scopes.pop_front();
}

void CallFrame::add_variable(std::string identifier, sp_variable variable) {
    _scopes.front().add_variable(identifier, variable);
}

sp_variable CallFrame::find_variable(const std::string& identifier) {
    for (auto& scope : _scopes) {
        if (auto var = scope.get_variable(identifier)) return var;
    }
    return nullptr;
}

bool CallFrame::is_in_current_scope(const std::string& identifier) {
    return _scopes.front().contains_variable(identifier);
}