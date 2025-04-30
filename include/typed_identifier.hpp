#ifndef TYPED_IDENTIFIER_HPP
#define TYPED_IDENTIFIER_HPP
#include <string>

#include "node.hpp"
#include "type.hpp"

struct TypedIdentifier : public Node {
    std::string name;
    VariableType type;
    explicit TypedIdentifier(const Position& position, std::string name, VariableType type);
};

using up_typed_identifier = std::unique_ptr<TypedIdentifier>;
#endif  // TYPED_IDENTIFIER_HPP