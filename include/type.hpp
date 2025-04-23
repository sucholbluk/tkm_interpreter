#ifndef TYPE_HPP
#define TYPE_HPP
#include <memory>
#include <variant>
#include <vector>

enum class BasicType {
    INT,
    FLOAT,
    BOOL,
    STRING,
    NONE,
};

struct FunctionType;

using Type = std::variant<BasicType, std::shared_ptr<FunctionType>>;

struct FunctionType {
    std::vector<Type> param_types;
    Type return_type;
};

std::string type_to_string(const Type& type_info);

bool operator==(const FunctionType& lhs, const FunctionType& rhs);
bool operator==(const Type& lhs, const Type& rhs);
bool operator!=(const FunctionType& lhs, const FunctionType& rhs);

#endif  // TYPE_HPP