#include "environment.hpp"

#include "builtint_functions.hpp"
#include "global_function.hpp"
#include "statement.hpp"

Environment::Environment() : _call_frames{} {
    // Initialize built-in functions
    std::ranges::for_each(Builtins::builtin_function_infos, [this](auto& builtin_info) {
        this->_functions[builtin_info.identifier] =
            std::make_shared<BuiltinFunction>(builtin_info.type, builtin_info.impl);
    });
}

void Environment::register_function(const FunctionDefinition& function) {
    std::string identifier{function.signature->identifier};
    if (_functions.contains(identifier)) {
        throw std::runtime_error("func redefinition");  // TODO - replace
    }
    _functions[identifier] = std::make_shared<GlobalFunction>(function);
}

void Environment::declare_variable(std::string identifier, VariableHolder var_holder) {
    _call_frames.top().add_variable(identifier, var_holder);
}

void Environment::declare_variable(std::string identifier, VariableType var_type, value var_value) {
    sp_variable var = std::make_shared<Variable>(var_type, var_value);
    declare_variable(identifier, VariableHolder{var});
};

void Environment::calling_function() {
    _call_frames.push(CallFrame{});
}

void Environment::exiting_function() {
    _call_frames.pop();
}

void Environment::entering_block() {
    _call_frames.top().push_scope();
}

void Environment::exiting_block() {
    _call_frames.top().pop_scope();
}

bool Environment::var_in_current_scope(const std::string& identifier) {
    return _call_frames.top().is_in_current_scope(identifier);
}

std::optional<VariableHolder> Environment::get_by_identifier(const std::string& identifier) {
    if (_call_frames.empty()) return std::nullopt;
    std::optional<VariableHolder> var_hold{std::nullopt};
    return _call_frames.top().find_variable(identifier);
}

std::shared_ptr<Callable> Environment::get_global_function(const std::string& identifier) {
    auto it = _functions.find(identifier);
    return it != _functions.end() ? it->second : nullptr;
}