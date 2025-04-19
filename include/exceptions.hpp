#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#endif  // EXCEPTIONS_HPP
#include <stdexcept>
#include <string>

#include "constants.hpp"
#include "position.hpp"
#include "token_type.hpp"
/*
 * @brief Base for other errors that originate in program's incorrect code.
 */
class ImplementationError : public std::logic_error {
   public:
    explicit ImplementationError(const std::string& message)
        : std::logic_error(message) {}
};
/*
 * @brief Base for errors in checked code.
 */
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

/*
 * @brief Token initialization called with logically wrong token type/value pair.
 */
class InvalidTokenValueError : public ImplementationError {
   public:
    explicit InvalidTokenValueError(const TokenType& type)
        : ImplementationError(std::string{"Invalid token value for"} + type_to_str(type)) {}
};

/*
 * @brief Trying to get the token of different value type
 */
class InvalidGetTokenValueError : public ImplementationError {
   public:
    explicit InvalidGetTokenValueError(const std::string& message)
        : ImplementationError("Invalid get_value in token: " + message) {}
};

/*
 * @brief Unexpected char discovered in checked code.
 */
class UnexpectedCharacterException : public LexerException {
   public:
    explicit UnexpectedCharacterException(const Position& position, char character)
        : LexerException(std::string("Unexpected char:") + character + " at: " + position.get_position_str()) {}
};

/*
 * @brief Parsing written value in coude would result in int overflow.atomic_noexcept
 */
class ParseIntOverflowException : public LexerException {
   public:
    explicit ParseIntOverflowException(const Position& position)
        : LexerException("Value too large at : " + position.get_position_str()) {}
};

/*
 * @brief Parsing fraction would result in unsigned long overflow
 */
class ParseFractionRangeExceededException : public LexerException {
   public:
    explicit ParseFractionRangeExceededException(const Position& position)
        : LexerException(std::string("Exceeded max fraction precision parsing value at: ") + position.get_position_str()) {}
};

/*
 * @brief String build was interrupted by newline/EOF.
 */
class UnfinishedStringException : public LexerException {
   public:
    explicit UnfinishedStringException(const Position& position)
        : LexerException("String literal build interrupted by newline at: " + position.get_position_str()) {}
};

/*
 * @breif Identifier length exceeds established max length.
 */
class IdentifierTooLongException : public LexerException {
   public:
    explicit IdentifierTooLongException(const Position& position)
        : LexerException(std::string{"Identifier at: "} + position.get_position_str() +
                         +", exceeds max identifier len: " + std::to_string(MAX_IDENTIFIER_LEN)) {}
};