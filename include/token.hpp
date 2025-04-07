#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <variant>

#include "exceptions.hpp"
#include "position.hpp"
#include "token_type.hpp"

using optional_token_value = std::variant<std::monostate, int, double, std::string>;

class Token {
    TokenType _type;
    Position _position;
    optional_token_value _value;

    void _validate_token() const;
    bool _type_matches_value() const;
    std::string _stringify_value() const;

   public:
    Token(TokenType type, Position position);
    Token(TokenType type, Position position, optional_token_value value);

    TokenType get_type() const;
    Position get_position() const;
    std::string print() const;

    template <typename T>
    T get_value_as() const;
};

template <typename T>
T Token::get_value_as() const {
    if (not std::holds_alternative<T>(_value))
        throw InvalidGetTokenValueException("Token value does not match requested type.");

    return std::get<T>(_value);
}

std::ostream& operator<<(std::ostream& os, const Token& token);

#endif  // TOKEN_HPP