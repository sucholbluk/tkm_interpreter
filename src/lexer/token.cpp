#include "token.hpp"

#include <format>
#include <set>
#include <typeindex>

using namespace tkm;
Token::Token(TokenType type, Position position) : Token(type, position, std::monostate{}) {}

Token::Token(TokenType type, Position position, optional_token_value value) {
    _validate_token(type, value);
    _type = type;
    _position = position;
    _value = value;
}

TokenType Token::get_type() const noexcept {
    return _type;
}

Position Token::get_position() const noexcept {
    return _position;
}

std::string Token::repr() const {
    std::string repr{std::string("Token(") + type_to_str(_type) + "," + _position.repr()};
    if (not std::holds_alternative<std::monostate>(_value)) {
        repr += "," + _stringify_value();
    }
    return repr + ")";
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.repr();
    return os;
}

std::string Token::_stringify_value() const {
    return std::visit(
        []<typename T>(const T& value) -> std::string {
            if constexpr (std::same_as<std::string, T>) {
                return std::format(R"("{}")", value);
            } else if constexpr (std::same_as<std::monostate, T>) {
                throw ImplementationError("error in Token::stringify_value after validation");
            } else {
                return std::to_string(value);
            }
        },
        _value);
}

void Token::_validate_token(const TokenType& type, const optional_token_value& value) {
    switch (type) {
        case TokenType::T_LITERAL_INT:
            if (std::holds_alternative<int>(value)) return;
            break;
        case TokenType::T_LITERAL_FLOAT:
            if (std::holds_alternative<double>(value)) return;
            break;
        case TokenType::T_LITERAL_BOOL:
            if (std::holds_alternative<bool>(value)) return;
            break;
        case TokenType::T_IDENTIFIER:
        case TokenType::T_COMMENT:
        case TokenType::T_LITERAL_STRING:
            if (std::holds_alternative<std::string>(value)) return;
            break;
        default:
            if (std::holds_alternative<std::monostate>(value)) return;
    }

    throw InvalidTokenValueError(type);
}