#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
#include <stack>

#include "call_frame.hpp"
#include "callable.hpp"
#include "variable.hpp"

class Environment {
   public:
    void register_function(const FunctionDefinition& function);
    void declare_variable();

    void calling_function();
    void exiting_function();

    void entering_block();
    void exiting_block();

    bool var_in_current_scope(std::string identifier);
    sp_variable get_by_identifier(std::string identifier);

   private:
    std::unordered_map<std::string, std::shared_ptr<Callable>> _global_functions;
    std::stack<CallFrame> _call_frames;
};
#endif  // ENVIRONMENT_HPP
