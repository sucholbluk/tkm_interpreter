#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
#include <stack>

#include "call_frame.hpp"
#include "callable.hpp"
#include "variable.hpp"

class Environment {
   public:
    Environment();
    void register_function(const FunctionDefinition& function);

    void declare_variable(std::string identifier, VariableHolder var_holder);
    void declare_variable(std::string identifier, value val);

    void calling_function();
    void exiting_function();

    void entering_block();
    void exiting_block();

    bool var_in_current_scope(const std::string& identifier);
    std::optional<VariableHolder> get_by_identifier(const std::string& identifier);

   private:
    std::unordered_map<std::string, std::shared_ptr<Callable>> _functions;
    std::stack<CallFrame> _call_frames;
};
#endif  // ENVIRONMENT_HPP
