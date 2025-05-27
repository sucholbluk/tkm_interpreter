#ifndef SCOPE_HPP
#define SCOPE_HPP
#include "variable.hpp"

/**
 * @ingroup interpreter
 * @brief Scope representation. Contains variables in scope.
 */
class Scope {
   public:
    sp_variable get_variable(std::string identifier);
    // to check if variable declared in scope
    bool contains_variable(std::string identifier);
    void add_variable(std::string identifier, sp_variable variable);

   private:
    std::unordered_map<std::string, sp_variable> _variables;
};

#endif  // SCOPE_HPP