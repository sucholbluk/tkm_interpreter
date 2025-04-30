#include "typed_identifier.hpp"

TypedIdentifier::TypedIdentifier(const Position& position, std::string name, VariableType type)
    : Node{position}, name{name}, type{type} {}