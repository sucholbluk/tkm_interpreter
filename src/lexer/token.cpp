#include "token.hpp"

#include <set>
#include <typeindex>

Token::Token(TokenType type, Position position) : Token(type, position, std::monostate{}) {}

Token::Token(TokenType type, Position position, optional_token_value value)
    : _type{type}, _position{position}, _value{value} {
    _validate_token();
}

TokenType Token::get_type() const noexcept {
    return _type;
}

Position Token::get_position() const noexcept {
    return _position;
}

std::string Token::print() const {
    std::string repr{std::string("Token(") + _type._to_string() + "," + _position.print()};
    if (not std::holds_alternative<std::monostate>(_value)) {
        repr += "," + _stringify_value();
    }
    return repr + ")";
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.print();
    return os;
}

void Token::_validate_token() const {
    if (_type_matches_value())
        return;

    throw InvalidTokenValueException(_type);
}

bool Token::_type_matches_value() const noexcept {
    switch (_type) {
        case TokenType::T_LITERAL_INT:
            return std::holds_alternative<int>(_value);
        case TokenType::T_LITERAL_FLOAT:
            return std::holds_alternative<double>(_value);
        case TokenType::T_LITERAL_BOOL:
            return std::holds_alternative<bool>(_value);
        case TokenType::T_IDENTIFIER:
        case TokenType::T_COMMENT:
        case TokenType::T_LITERAL_STRING:
            return std::holds_alternative<std::string>(_value);
        default:
            return std::holds_alternative<std::monostate>(_value);
    }
}

std::string Token::_stringify_value() const {
    return std::visit(
        [](const auto& value) -> std::string {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int>)
                return std::to_string(value);
            else if constexpr (std::is_same_v<T, double>)
                return std::to_string(value);
            else if constexpr (std::is_same_v<T, std::string>)
                return "\"" + value + "\"";
            else if constexpr (std::is_same_v<T, bool>)
                return std::to_string(value);
            else
                throw std::logic_error("It's after validation - should never be called.");
        },
        _value);
}