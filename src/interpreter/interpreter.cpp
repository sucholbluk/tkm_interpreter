#include "interpreter.hpp"

#include "builtint_functions.hpp"
#include "global_function.hpp"
#include "program.hpp"
#include "statement.hpp"

void Interpreter::visit(const Program& program) {
    std::ranges::for_each(program.function_definitions,
                          [this](const auto& function) { _env.register_function(*function); });
    _execute_main();
}

void Interpreter::_execute_main() {
    if (auto main = _env.get_global_function("main")) {
        if (not(main->get_type() == MainProperties::type)) {
            throw std::runtime_error("invalid main function definition");
        }
        main->call(*this, {});

    } else {
        throw std::runtime_error("No main function to execute");
    }
}