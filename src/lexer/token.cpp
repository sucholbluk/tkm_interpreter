#include "token.hpp"

#include <set>
#include <typeindex>

Token::Token(TokenType type, Position position, optional_token_value value)
    : _type{type}, _position{position}, _value{value} {
    _validate_token();
}

TokenType Token::get_type() const {
    return _type;
}

Position Token::get_position() const {
    return _position;
}

std::string Token::print() const {
    std::string repr{std::string("Token(") + _type._to_string() + "," + _position.print()};
    if (_value.has_value()) {
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

    switch (_type) {
        case TokenType::T_LITERAL_INT:
            throw InvalidTokenValueException(std::string("expected integer value for ") + _type._to_string() + ".");
        case TokenType::T_LITERAL_FLOAT:
            throw InvalidTokenValueException(std::string("expected floating value for ") + _type._to_string() + ".");
        case TokenType::T_LITERAL_STRING:
        case TokenType::T_COMMENT:
        case TokenType::T_IDENTIFIER:
            throw InvalidTokenValueException(std::string("expected std::string value for") + _type._to_string() + ".");
        default:
            throw InvalidTokenValueException(std::string("no value expected for ") + _type._to_string() + ".");
    };
}

bool Token::_type_matches_value() const {
    switch (_type) {
        case TokenType::T_LITERAL_INT:
            return _value.has_value() and std::holds_alternative<int>(*_value);
        case TokenType::T_LITERAL_FLOAT:
            return _value.has_value() and std::holds_alternative<double>(*_value);
        case TokenType::T_IDENTIFIER:
        case TokenType::T_COMMENT:
        case TokenType::T_LITERAL_STRING:
            return _value.has_value() and std::holds_alternative<std::string>(*_value);
        default:
            return not _value.has_value();
    }
}

std::string Token::_stringify_value() const {
    switch (_type) {
        case TokenType::T_LITERAL_INT:
            return std::to_string(std::get<int>(*_value));
        case TokenType::T_LITERAL_FLOAT:
            return std::to_string(std::get<double>(*_value));
        default:
            return std::string{"\""} + std::get<std::string>(*_value) + "\"";
    }
}