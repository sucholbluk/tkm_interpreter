#include "global_function.hpp"

#include "statement.hpp"

GlobalFunction::GlobalFunction(const FunctionDefinition& function) : _function{function} {
    std::cout << "Function created: " << _function.signature->identifier << std::endl;
}

Type GlobalFunction::get_type() const {
    return _function.signature->type;
}

namespace MainProperties {
const Type type = Type{FunctionTypeInfo({}, Type{TypeKind::INT})};
}