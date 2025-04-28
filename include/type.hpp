#ifndef TYPE_HPP
#define TYPE_HPP
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

enum class TypeKind {
    INT,
    FLOAT,
    BOOL,
    STRING,
    FUNCTION,
};

struct FunctionTypeInfo;

struct Type {
    TypeKind kind;
    std::shared_ptr<FunctionTypeInfo> function_type_info = nullptr;

    Type() = default;
    explicit Type(TypeKind kind);
    explicit Type(FunctionTypeInfo function_type_info);

    std::string to_str() const;
};

struct ParamType {
    Type type;
    bool is_mutable = false;

    explicit ParamType(Type type, bool is_mutable = false);

    std::string to_str() const;
};

// empty param_types vector means that function is of type function<none:XXX>
// non-existant return_type means the function is of type function<XXX:none>
struct FunctionTypeInfo {
    std::vector<ParamType> param_types;
    std::optional<Type> return_type;

    std::string to_str() const;
};

std::string type_kind_to_string(const TypeKind& type_kind);

bool operator==(const Type& lhs, const Type& rhs);
bool operator==(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs);
bool operator==(const ParamType& lhs, const ParamType& rhs);

bool operator!=(const Type& lhs, const Type& rhs);
bool operator!=(const FunctionTypeInfo& lhs, const FunctionTypeInfo& rhs);
bool operator!=(const ParamType& lhs, const ParamType& rhs);

std::ostream& operator<<(std::ostream& os, const Type& type);
std::ostream& operator<<(std::ostream& os, const ParamType& param_type);
std::ostream& operator<<(std::ostream& os, const FunctionTypeInfo& func_type_info);
std::ostream& operator<<(std::ostream& os, const TypeKind& type_kind);

#endif  // TYPE_HPP