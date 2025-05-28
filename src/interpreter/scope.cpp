#include "scope.hpp"

std::optional<VariableHolder> Scope::get_variable(const std::string& identifier) {
    auto it = _variables.find(identifier);
    return it != _variables.end() ? std::make_optional(it->second) : std::nullopt;
}

bool Scope::contains_variable(const std::string& identifier) {
    return _variables.contains(identifier);
}

void Scope::add_variable(std::string identifier, VariableHolder variable) {
    _variables[identifier] = variable;
}