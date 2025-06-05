#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <variant>
#include <string>

#include "exceptions.hpp"
#include "position.hpp"
#include "token_type.hpp"

using optional_token_value = std::variant<std::monostate, int, double, bool, std::string>;

/**
 * @ingroup lexer
 * @brief Class representing Token
 *
 * There are 6 posible @ref Token Types that have value:
 * - T_LITERAL_INT - int
 * - T_LITERAL_FLOAT - double
 * - T_LITERAL_STRING - std::string
 * - T_LITERAL_BOOL - bool
 * - T_COMMENT - std::string
 * - T_IDENTIFIER - std::string
 */
using namespace tkm;
class Token {
   public:
    Token(TokenType type, Position position);
    Token(TokenType type, Position position, optional_token_value value);

    TokenType get_type() const noexcept;
    Position get_position() const noexcept;
    std::string repr() const;

    template <typename T>
    T get_value_as() const;

   private:
    TokenType _type;
    Position _position;
    optional_token_value _value;

    static void _validate_token(const TokenType& type, const optional_token_value& value);
    std::string _stringify_value() const;
};

/**
 * @brief Tries to return value of requested type.
 *
 * Usage must follow logic: check token type -> request the right type
 */
template <typename T>
T Token::get_value_as() const {
    if (auto value = std::get_if<T>(&_value)) {
        return *value;
    }
    throw InvalidGetTokenValueError(this->repr());
}

std::ostream& operator<<(std::ostream& os, const Token& token);

#endif  // TOKEN_HPP