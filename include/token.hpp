#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <optional>
#include <variant>

#include "exceptions.hpp"
#include "position.hpp"
#include "token_type.hpp"

using optional_token_value = std::optional<std::variant<int, double, std::string>>;

class Token {
    TokenType _type;
    Position _position;
    optional_token_value _value;

    void _validate_token() const;
    bool _type_matches_value() const;
    std::string _stringify_value() const;

   public:
    Token(TokenType type, Position position, optional_token_value value = std::nullopt);

    TokenType get_type() const;
    Position get_position() const;
    std::string print() const;

    template <typename T>
    T get_value_as() const;
};

template <typename T>
T Token::get_value_as() const {
    if (not _value.has_value())
        throw InvalidGetTokenValueException("token does not hold a value.");

    if (not std::holds_alternative<T>(*_value))
        throw InvalidGetTokenValueException("Token value does not match requested type.");

    return std::get<T>(*_value);
}

std::ostream& operator<<(std::ostream& os, const Token& token);

#endif  // TOKEN_HPP