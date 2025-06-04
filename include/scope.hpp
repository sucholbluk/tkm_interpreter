#ifndef SCOPE_HPP
#define SCOPE_HPP
#include "variable.hpp"

/**
 * @ingroup interpreter
 * @brief Scope representation. Contains variables in scope.
 */
class Scope {
   public:
    std::optional<VariableHolder> get_variable(const std::string& identifier);
    // to check if variable declared in scope
    bool contains_variable(const std::string& identifier);
    void add_variable(std::string identifier, VariableHolder variable);

   private:
    std::unordered_map<std::string, VariableHolder> _variables;
};

#endif  // SCOPE_HPP