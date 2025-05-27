#include "interpreter.hpp"

#include "builtint_functions.hpp"
#include "global_function.hpp"
#include "program.hpp"
#include "statement.hpp"

void Interpreter::visit(const Program& program) {
    std::ranges::for_each(program.function_definitions,
                          [this](const auto& function) { _register_function(*function); });
}

void Interpreter::_register_function(const FunctionDefinition& function) {}