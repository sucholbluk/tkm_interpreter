#include "global_function.hpp"

#include "interpreter.hpp"
#include "statement.hpp"

GlobalFunction::GlobalFunction(const FunctionDefinition& function) : _function{function} {
    std::cout << "Function created: " << _function.signature->identifier << std::endl;
}

Type GlobalFunction::get_type() const {
    return _function.signature->type;
}

void GlobalFunction::call(Interpreter& interpreter, arg_list arguments) {
    // TODO - init variables based on params
    _function.body->accept(interpreter);
}

namespace MainProperties {
const Type type = Type{FunctionTypeInfo({}, Type{TypeKind::INT})};
}