#ifndef TYPED_IDENTIFIER_HPP
#define TYPED_IDENTIFIER_HPP
#include <string>

#include "node.hpp"
#include "type.hpp"

/**
 * @ingroup parser
 * @brief EBNF TypedIdentifier representation.
 */
struct TypedIdentifier : public Node {
    std::string name;
    VariableType type;
    explicit TypedIdentifier(const Position& position, std::string name, VariableType type);

    void accept(Visitor& visitor) const override;
};

using up_typed_identifier = std::unique_ptr<TypedIdentifier>;
using up_typed_ident_vec = std::vector<up_typed_identifier>;
#endif  // TYPED_IDENTIFIER_HPP