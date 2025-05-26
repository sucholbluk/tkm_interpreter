#include "interpreter.hpp"

#include "builtint_functions.hpp"
#include "global_function.hpp"
#include "program.hpp"
#include "statement.hpp"

Interpreter::Interpreter() {
    _functions["print"] = std::make_shared<PrintFunction>();
}

void Interpreter::visit(const Program& program) {
    std::ranges::for_each(program.function_definitions,
                          [this](const auto& function) { _register_function(*function); });
}

void Interpreter::_register_function(const FunctionDefinition& function) {
    std::string function_identifier{function.signature->identifier};
    if (_functions.contains(function_identifier)) {
        throw std::runtime_error("Function redefinition");
    }

    _functions[function_identifier] = std::make_shared<GlobalFunction>(function);
}