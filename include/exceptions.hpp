#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#endif  // EXCEPTIONS_HPP
#include <stdexcept>
#include <string>

#include "constants.hpp"
#include "position.hpp"
#include "token_type.hpp"

class ImplementationError : public std::logic_error {
   public:
    explicit ImplementationError(const std::string& message)
        : std::logic_error(message) {}
};

class LexerException : public std::runtime_error {
   public:
    explicit LexerException(const std::string& message)
        : std::runtime_error(message) {}
};

class FileOpenException : public std::runtime_error {
   public:
    explicit FileOpenException(const std::string& filename)
        : std::runtime_error("Failed to open file: " + filename) {}
};

// if the written code is correct it should never be thrown
class InvalidTokenValueError : public ImplementationError {
   public:
    explicit InvalidTokenValueError(const TokenType type)
        : ImplementationError(std::string{"Invalid token value for"} + type._to_string()) {}
};

// if the written code is correct it should never be thrown
class InvalidGetTokenValueError : public ImplementationError {
   public:
    explicit InvalidGetTokenValueError(const std::string& message)
        : ImplementationError("Invalid get_value in token: " + message) {}
};

class UnexpectedCharacterException : public LexerException {
   public:
    explicit UnexpectedCharacterException(const Position& position, char character)
        : LexerException(std::string("Unexpected char:") + character + " at: " + position.get_position_str()) {}
};

class ParseIntOverflowException : public LexerException {
   public:
    explicit ParseIntOverflowException(const Position& position)
        : LexerException("Value too large at : " + position.get_position_str()) {}
};

class ParseFractionRangeExceededException : public LexerException {
   public:
    explicit ParseFractionRangeExceededException(const Position& position)
        : LexerException(std::string("Exceeded max fraction precision parsing value at: ") + position.get_position_str()) {}
};

class UnfinishedStringException : public LexerException {
   public:
    explicit UnfinishedStringException(const Position& position)
        : LexerException("String literal build interrupted by newline at: " + position.get_position_str()) {}
};

class IdentifierTooLongException : public LexerException {
   public:
    explicit IdentifierTooLongException(const Position position)
        : LexerException(std::string{"Identifier at: "} + position.get_position_str() +
                         +", exceeds max identifier len: " + std::to_string(MAX_IDENTIFIER_LEN)) {}
};