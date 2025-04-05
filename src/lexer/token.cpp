#include "token.hpp"

#include <set>
#include <typeindex>

Token::Token(TokenType type, Position position, optional_token_value value)
    : _type{type}, _position{position}, _value{value} {
    _validate_token();
}

void Token::_validate_token() const {
    if (_type_matches_value())
        return;

    switch (_type) {
        case TokenType::T_LITERAL_INT:
            throw InvalidTokenValueException("expected int value for TokenType::T_LITERAL_INT.");
        case TokenType::T_LITERAL_FLOAT:
            throw InvalidTokenValueException("expected float value for TokenType::T_LITERAL_FLOAT.");
        case TokenType::T_LITERAL_STRING:
            throw InvalidTokenValueException("expected std::string value for TokenType::T_LITERAL_STRING.");
        case TokenType::T_IDENTIFIER:
            throw InvalidTokenValueException("expected std::string value for TokenType::T_IDENTIFIER.");
        default:
            throw InvalidTokenValueException("non value token type recieved value.");
    };
}

bool Token::_type_matches_value() const {
    switch (_type) {
        case TokenType::T_LITERAL_INT:
            return _value.has_value() and std::holds_alternative<int>(*_value);
        case TokenType::T_LITERAL_FLOAT:
            return _value.has_value() and std::holds_alternative<float>(*_value);
        case TokenType::T_IDENTIFIER:
        case TokenType::T_LITERAL_STRING:
            return _value.has_value() and std::holds_alternative<std::string>(*_value);
        default:
            return not _value.has_value();
    }
}