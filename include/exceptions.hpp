#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <format>
#include <stdexcept>
#include <string>

#include "constants.hpp"
#include "position.hpp"
#include "token_type.hpp"
/**
 * @defgroup exceptions Exceptions
 * @brief Exceptions used in the project.
 * @{
 */

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

class ExpectedTypeSpecException : public ParserException {
   public:
    explicit ExpectedTypeSpecException(const Position& position);
};

class ExpectedTypeException : public ParserException {
   public:
    explicit ExpectedTypeException(const Position& position);
};

class ExpectedTypedIdentifierException : public ParserException {
   public:
    explicit ExpectedTypedIdentifierException(const Position& position);
};

class ExpectedAssignmentException : public ParserException {
   public:
    explicit ExpectedAssignmentException(const Position& position);
};

class ExpectedTypeForTypeCastException : public ParserException {
   public:
    explicit ExpectedTypeForTypeCastException(const Position& position);
};

class ExpectedExprAfterOrException : public ParserException {
   public:
    explicit ExpectedExprAfterOrException(const Position& position);
};

class ExpectedExprAfterAndException : public ParserException {
   public:
    explicit ExpectedExprAfterAndException(const Position& position);
};

class ExpectedExprAfterEqualityException : public ParserException {
   public:
    explicit ExpectedExprAfterEqualityException(const Position& position);
};

class ExpectedExprAfterComparisonException : public ParserException {
   public:
    explicit ExpectedExprAfterComparisonException(const Position& position);
};

class ExpectedExprAfterAdditiveException : public ParserException {
   public:
    explicit ExpectedExprAfterAdditiveException(const Position& position);
};

class ExpectedExprAfterMultiplicativeException : public ParserException {
   public:
    explicit ExpectedExprAfterMultiplicativeException(const Position& position);
};

class ExpectedExprAfterUnaryException : public ParserException {
   public:
    explicit ExpectedExprAfterUnaryException(const Position& position);
};

class ExpectedExprAfterFuncCompException : public ParserException {
   public:
    explicit ExpectedExprAfterFuncCompException(const Position& position);
};

class ExpectedBindFrontOperatorException : public ParserException {
   public:
    explicit ExpectedBindFrontOperatorException(const Position& position);
};

class ExpectedBindFrontTargetException : public ParserException {
   public:
    explicit ExpectedBindFrontTargetException(const Position& position);
};

class ExpectedIfConditionException : public ParserException {
   public:
    explicit ExpectedIfConditionException(const Position& position);
};

class ExpectedConditionalStatementBodyException : public ParserException {
   public:
    explicit ExpectedConditionalStatementBodyException(const Position& position);
};

class ExpectedLoopVarDeclException : public ParserException {
   public:
    explicit ExpectedLoopVarDeclException(const Position& position);
};

class ExpectedLoopConditionException : public ParserException {
   public:
    explicit ExpectedLoopConditionException(const Position& position);
};

class ExpectedLoopBodyException : public ParserException {
   public:
    explicit ExpectedLoopBodyException(const Position& position);
};

class ExpectedExprException : public ParserException {
   public:
    explicit ExpectedExprException(const Position& position);
};

class ExpectedLoopVarUpdateException : public ParserException {
   public:
    explicit ExpectedLoopVarUpdateException(const Position& position);
};

class InvalidAssignTargetException : public ParserException {
   public:
    explicit InvalidAssignTargetException(const Position& position);
};

class InvalidFunctionParamTypeException : public ParserException {
   public:
    explicit InvalidFunctionParamTypeException(const Position& position);
};

class ExpectedArrowException : public ParserException {
   public:
    explicit ExpectedArrowException(const Position& position);
};

class ExpectedSemicolException : public ParserException {
   public:
    explicit ExpectedSemicolException(const Position& position);
};

class ExpectedRBraceException : public ParserException {
   public:
    explicit ExpectedRBraceException(const Position& position);
};

class ExpectedLParenException : public ParserException {
   public:
    explicit ExpectedLParenException(const Position& position);
};

class ExpectedRParenException : public ParserException {
   public:
    explicit ExpectedRParenException(const Position& position);
};

class ExpectedColException : public ParserException {
   public:
    explicit ExpectedColException(const Position& position);
};

class ExpectedLessException : public ParserException {
   public:
    explicit ExpectedLessException(const Position& position);
};

class ExpectedGreaterException : public ParserException {
   public:
    explicit ExpectedGreaterException(const Position& position);
};

class ExpectedIdentifierException : public ParserException {
   public:
    explicit ExpectedIdentifierException(const Position& position);
};

template <typename ExceptionT>
[[noreturn]] void rethrow_with_position(const ExceptionT& e, const Position& position) {
    throw ExceptionT(std::string(e.what()) + " at: " + position.get_position_str());
}

/** @} */
#endif  // EXCEPTIONS_HPP