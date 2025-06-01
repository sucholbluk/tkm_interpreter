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
    void declare_variable(std::string identifier, VariableType var_type, value var_value);

    void calling_function(std::optional<Type> ret_type);
    void exiting_function();
    std::optional<Type> get_cur_func_ret_type() const;

    void add_scope();
    void pop_scope();

    bool var_in_current_scope(const std::string& identifier);
    std::optional<VariableHolder> get_by_identifier(const std::string& identifier);
    sp_callable get_global_function(const std::string& identifier);

   private:
    std::unordered_map<std::string, sp_callable> _functions;
    std::stack<CallFrame> _call_frames;
};
#endif  // ENVIRONMENT_HPP
