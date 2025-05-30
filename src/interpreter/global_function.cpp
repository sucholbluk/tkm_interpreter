#include "global_function.hpp"

#include "interpreter.hpp"
#include "statement.hpp"
#include "type_handler.hpp"

GlobalFunction::GlobalFunction(const FunctionDefinition& function) : _function{function} {
    std::cout << "Function created: " << _function.signature->identifier << std::endl;
}

Type GlobalFunction::get_type() const {
    return _function.signature->type;
}

void GlobalFunction::call(Interpreter& inter, arg_list arguments) {
    std::string id{_function.signature->identifier};
    auto& params{_function.signature->params};

    // initializing args
    for (size_t i = 0; i < arguments.size(); ++i) {
        std::visit(
            [&]<typename T>(const T& val_or_vh) {
                if constexpr (std::same_as<VariableHolder, T>) {
                    VariableHolder var_holder{val_or_vh.var, params[i]->type.is_mutable};
                    inter._env.declare_variable(params[i]->name, var_holder);
                } else if constexpr (std::same_as<value, T>) {
                    sp_variable var = std::make_shared<Variable>(params[i]->type, val_or_vh);
                    inter._env.declare_variable(params[i]->name, VariableHolder{var});
                }
            },
            arguments[i]);
    }

    // function body execution
    _function.body->accept(inter);
}

namespace MainProperties {
const Type type = Type{FunctionTypeInfo({}, Type{TypeKind::INT})};
}