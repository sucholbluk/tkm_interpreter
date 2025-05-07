#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#endif  // EXCEPTIONS_HPP
#include <format>
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
    explicit ImplementationError(const std::string& message);
};
/* -----------------------------------------------------------------------------*
 *                                LEXER ERRORS                                  *
 *------------------------------------------------------------------------------*/

/*
 * @brief Base for errors in checked code.
 */
class LexerException : public std::runtime_error {
   public:
    explicit LexerException(const std::string& message);
};

class FileOpenException : public std::runtime_error {
   public:
    explicit FileOpenException(const std::string& filename);
};

/*
 * @brief Token initialization called with logically wrong token type/value pair.
 */
class InvalidTokenValueError : public ImplementationError {
   public:
    explicit InvalidTokenValueError(const TokenType& type);
};

/*
 * @brief Trying to get the token of different value type
 */
class InvalidGetTokenValueError : public ImplementationError {
   public:
    explicit InvalidGetTokenValueError(const std::string& message);
};

/*
 * @brief Unexpected char discovered in checked code.
 */
class UnexpectedCharacterException : public LexerException {
   public:
    explicit UnexpectedCharacterException(const Position& position, char character);
};

/*
 * @brief Parsing written value in coude would result in int overflow.atomic_noexcept
 */
class ParseIntOverflowException : public LexerException {
   public:
    explicit ParseIntOverflowException(const Position& position);
};

/*
 * @brief Parsing fraction would result in unsigned long overflow
 */
class ParseFractionRangeExceededException : public LexerException {
   public:
    explicit ParseFractionRangeExceededException(const Position& position);
};

/*
 * @brief String build was interrupted by newline/EOF.
 */
class UnfinishedStringException : public LexerException {
   public:
    explicit UnfinishedStringException(const Position& position);
};

/*
 * @breif Identifier length exceeds established max length.
 */
class IdentifierTooLongException : public LexerException {
   public:
    explicit IdentifierTooLongException(const Position& position);
};
/* -----------------------------------------------------------------------------*
 *                               PARSER ERRORS                                  *
 *------------------------------------------------------------------------------*/
class ParserException : public std::runtime_error {
   public:
    explicit ParserException(const std::string& message);
};

class ExpectedFuncOrEOFException : public ParserException {
   public:
    explicit ExpectedFuncOrEOFException(const Position& position);
};

class ExpectedFunctionBodyException : public ParserException {
   public:
    explicit ExpectedFunctionBodyException(const std::string& function_name, const Position& position);
};

class ExpectedFuncIdentException : public ParserException {
   public:
    explicit ExpectedFuncIdentException(const Position& position);
};

class ExpectedArgListException : public ParserException {
   public:
    explicit ExpectedArgListException(const Position& position);
};

class ExpectedArrowException : public ParserException {
   public:
    explicit ExpectedArrowException(const Position& position);
};

class ExpectedTypeSpecException : public ParserException {
   public:
    explicit ExpectedTypeSpecException(const Position& position);
};

class ExpectedTypeException : public ParserException {
   public:
    explicit ExpectedTypeException(const Position& position);
};

class ExpectedSemicolException : public ParserException {
   public:
    explicit ExpectedSemicolException(const Position& position);
};

class ExpectedTypedIdentifierException : public ParserException {
   public:
    explicit ExpectedTypedIdentifierException(const Position& position);
};
