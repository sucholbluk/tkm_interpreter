#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#endif  // EXCEPTIONS_HPP
#include <stdexcept>
#include <string>

#include "constants.hpp"
#include "position.hpp"
#include "token_type.hpp"

// class ProgramImplementationError : public std::logic_error

// if the written code is correct it should never be thrown
class InvalidTokenValueException : public std::logic_error {
   public:
    explicit InvalidTokenValueException(const TokenType type)
        : std::logic_error(std::string{"Invalid token value for"} + type._to_string()) {}
};

// if the written code is correct it should never be thrown
class InvalidGetTokenValueException : public std::logic_error {
   public:
    explicit InvalidGetTokenValueException(const std::string& message)
        : std::logic_error("Invalid get_value in token: " + message) {}
};

class UnexpectedCharacterException : public std::runtime_error {
   public:
    explicit UnexpectedCharacterException(const std::string& message)
        : std::runtime_error("Unexpected char at: " + message) {}
};

class IntValueOverflowException : public std::runtime_error {
   public:
    explicit IntValueOverflowException(const std::string& message)
        : std::runtime_error("Value too large at : " + message) {}
};

class UnfinishedStringException : public std::runtime_error {
   public:
    explicit UnfinishedStringException(const std::string& message)
        : std::runtime_error("String literal build interrupted by newline at: " + message) {}
};

class IdentifierTooLongException : public std::runtime_error {
   public:
    explicit IdentifierTooLongException(const Position position)
        : std::runtime_error(std::string{"Identifier at: "} + position.get_position_str() +
                             +", exceeds max identifier len: " + std::to_string(MAX_IDENTIFIER_LEN)) {}
};