#include "call_frame.hpp"

void CallFrame::push_scope() {
    _scopes.push_front(Scope{});
}

void CallFrame::pop_scope() {
    _scopes.pop_front();
}

void CallFrame::add_variable(std::string identifier, VariableHolder variable) {
    _scopes.front().add_variable(identifier, variable);
}

std::optional<VariableHolder> CallFrame::find_variable(const std::string& identifier) {
    for (auto& scope : _scopes) {
        if (auto var = scope.get_variable(identifier)) return var;
    }
    return std::nullopt;
}

bool CallFrame::is_in_current_scope(const std::string& identifier) {
    return _scopes.front().contains_variable(identifier);
}

std::optional<Type> CallFrame::get_ret_type() const {
    return _return_type;
}