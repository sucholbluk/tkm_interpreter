#include "type.hpp"

#include <sstream>
#include <typeindex>

std::string _basic_type_to_string(const BasicType& type) {
    switch (type) {
        case BasicType::INT:
            return "int";
        case BasicType::FLOAT:
            return "float";
        case BasicType::STRING:
            return "string";
        case BasicType::BOOL:
            return "bool";
        case BasicType::NONE:
            return "none";
    }
}

std::string type_to_string(const Type& type) {
    return std::visit(
        []<typename T>(const T& _type) -> std::string {
            if constexpr (std::same_as<std::shared_ptr<FunctionType>, T>) {
                std::stringstream stream;
                stream << "function<";
                for (auto it = _type->param_types.begin(); it != _type->param_types.end(); ++it) {
                    stream << type_to_string(*it);
                    if (it != _type->param_types.end() - 1)
                        stream << ",";
                }
                stream << ":";
                stream << type_to_string(_type->return_type);
                stream << ">";
                return stream.str();
            } else {
                return _basic_type_to_string(_type);
            }
        },
        type);
}

bool operator==(const Type& lhs, const Type& rhs) {
    return std::visit(
        []<typename T, typename U>(const T& left, const U& right) {
            if constexpr (not std::same_as<T, U>)
                return false;

            if constexpr (std::same_as<T, BasicType>)
                return left == right;

            if constexpr (std::same_as<T, std::shared_ptr<FunctionType>> and std::same_as<U, std::shared_ptr<FunctionType>>) {
                if (not left or not right)
                    return left == right;
                return *left == *right;
            }
            return false;
        },
        lhs, rhs);
}

bool operator!=(const Type& lhs, const Type& rhs) {
    return not(lhs == rhs);
}

bool operator==(const FunctionType& lhs, const FunctionType& rhs) {
    return lhs.param_types == rhs.param_types and lhs.return_type == rhs.return_type;
}

bool operator!=(const FunctionType& lhs, const FunctionType& rhs) {
    return not(lhs == rhs);
}