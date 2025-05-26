#include "global_function.hpp"

#include "statement.hpp"

GlobalFunction::GlobalFunction(const FunctionDefinition& function) : _function{function} {
    std::cout << "Function created: " << _function.signature->identifier << std::endl;
}