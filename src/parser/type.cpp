#include "type.hpp"

#include "exceptions.hpp"

/* -----------------------------------------------------------------------------*
 *                                      TYPE                                    *
 *------------------------------------------------------------------------------*/

Type::Type(TypeKind type_kind) {
    if (type_kind == TypeKind::FUNCTION)
        throw std::invalid_argument("function cannot be initialized without FunctionTypeInfo");
    kind = type_kind;
}

Type::Type(FunctionTypeInfo fun_type_info)
    : kind{TypeKind::FUNCTION}, function_type_info{std::make_shared<FunctionTypeInfo>(std::move(fun_type_info))} {}

std::string Type::to_str() const {
    std::string type_str{type_kind_to_string(kind)};

    if (kind == TypeKind::FUNCTION) {
        type_str += function_type_info->to_str();
    }
    return type_str;
}

bool operator==(const Type& lhs, const Type& rhs) {
    if (lhs.kind != rhs.kind) return false;

    if (lhs.kind != TypeKind::FUNCTION) return true;

    return *lhs.function_type_info == *rhs.function_type_info;
}

bool operator!=(const Type& lhs, const Type& rhs) {
    return not(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Type& type) {
    os << type.to_str();
    return os;
}

/* -----------------------------------------------------------------------------*
 *                             FUNCTION_TYPE_INFO                               *
 *------------------------------------------------------------------------------*/

std::string FunctionTypeInfo::to_str() const {
    std::string fun_type_info_str{"<"};
    if (param_types.empty()) {
        fun_type_info_str += "none";
    } else {
        for (auto it = param_types.begin(); it != param_types.end(); ++it) {
            fun_type_info_str += it->to_str();
            if (it != param_types.end() - 1) {
                fun_type_info_str += ",";
            }
        }
    }

    fun_type_info_str += ":";
    if (return_type) {
        fun_type_info_str += return_type.value().to_str();
    } else {
        fun_type_info_str += "none";
    }

    return fun_type_info_str + ">";
}

bool operator==(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs) {
    return lhs.param_types == rhs.param_types and lhs.return_type == rhs.return_type;
}

bool operator!=(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs) {
    return not(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const FunctionTypeInfo& func_type_info) {
    os << func_type_info.to_str();
    return os;
}

/* -----------------------------------------------------------------------------*
 *                                 PARAM_TYPE                                   *
 *------------------------------------------------------------------------------*/

VariableType::VariableType(Type type, bool is_mutable) {
    if (type.kind == TypeKind::FUNCTION and is_mutable)
        throw std::invalid_argument("function type parameter cannot be mutable");

    this->type = type;
    this->is_mutable = is_mutable;
}

std::string VariableType::to_str() const {
    std::string param_type_str{};
    if (is_mutable) {
        param_type_str += "mut ";
    }
    return param_type_str + type.to_str();
}

bool operator==(const VariableType& lhs, const VariableType& rhs) {
    return lhs.is_mutable == rhs.is_mutable and lhs.type == rhs.type;
}

bool operator!=(const VariableType& lhs, const VariableType& rhs) {
    return not(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const VariableType& param_type) {
    os << param_type.to_str();
    return os;
}

/* -----------------------------------------------------------------------------*
 *                                 TYPE_KIND                                    *
 *------------------------------------------------------------------------------*/

std::string type_kind_to_string(const TypeKind& type_kind) {
    switch (type_kind) {
        case TypeKind::INT:
            return "int";
        case TypeKind::FLOAT:
            return "float";
        case TypeKind::BOOL:
            return "bool";
        case TypeKind::STRING:
            return "string";
        case TypeKind::FUNCTION:
            return "function";
        default:
            throw ImplementationError("TypeKind didnt match???");
    }
}

std::ostream& operator<<(std::ostream& os, const TypeKind& type_kind) {
    os << type_kind_to_string(type_kind);
    return os;
}
