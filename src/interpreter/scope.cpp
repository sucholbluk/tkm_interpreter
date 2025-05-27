#include "scope.hpp"

sp_variable Scope::get_variable(const std::string& identifier) {
    auto it = _variables.find(identifier);
    return it != _variables.end() ? it->second : nullptr;
}

bool Scope::contains_variable(const std::string& identifier) {
    return _variables.contains(identifier);
}

void Scope::add_variable(std::string identifier, sp_variable variable) {
    _variables[identifier] = variable;
}