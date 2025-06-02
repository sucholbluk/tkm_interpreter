#include "exceptions.hpp"

ImplementationError::ImplementationError(const std::string& message) : std::logic_error(message) {}
/* -----------------------------------------------------------------------------*
 *                                LEXER ERRORS                                  *
 *------------------------------------------------------------------------------*/

LexerException::LexerException(const std::string& message) : std::runtime_error(message) {}

FileOpenException::FileOpenException(const std::string& filename)
    : std::runtime_error("Failed to open file: " + filename) {}

InvalidTokenValueError::InvalidTokenValueError(const TokenType& type)
    : ImplementationError(std::string{"Invalid token value for"} + type_to_str(type)) {}

InvalidGetTokenValueError::InvalidGetTokenValueError(const std::string& message)
    : ImplementationError("Invalid get_value in token: " + message) {}

UnexpectedCharacterException ::UnexpectedCharacterException(const Position& position, char character)
    : LexerException(std::string("Unexpected char:") + character + " at: " + position.get_position_str()) {}

ParseIntOverflowException ::ParseIntOverflowException(const Position& position)
    : LexerException("Value too large at : " + position.get_position_str()) {}

ParseFractionRangeExceededException ::ParseFractionRangeExceededException(const Position& position)
    : LexerException(std::string("Exceeded max fraction precision parsing value at: ") + position.get_position_str()) {}

UnfinishedStringException ::UnfinishedStringException(const Position& position)
    : LexerException("String literal build interrupted by newline at: " + position.get_position_str()) {}

IdentifierTooLongException ::IdentifierTooLongException(const Position& position)
    : LexerException(std::string{"Identifier at: "} + position.get_position_str() +
                     +", exceeds max identifier len: " + std::to_string(MAX_IDENTIFIER_LEN)) {}

/* -----------------------------------------------------------------------------*
 *                               PARSER ERRORS                                  *
 *------------------------------------------------------------------------------*/
ParserException ::ParserException(const std::string& message) : std::runtime_error(message) {}

ExpectedFuncOrEOFException ::ExpectedFuncOrEOFException(const Position& position)
    : ParserException(std::format("Unexpected input at: {}\nExpected function definition or EOF (there may be code "
                                  "outside function definitions)",
                                  position.get_position_str())) {}

ExpectedFunctionBodyException ::ExpectedFunctionBodyException(const std::string& function_name,
                                                              const Position& position)
    : ParserException(
          std::format("Expected body for function: {} at: {}", function_name, position.get_position_str())) {}

ExpectedFuncIdentException ::ExpectedFuncIdentException(const Position& position)
    : ParserException(std::format("Expected function identifier at: {}", position.get_position_str())) {}

ExpectedArgListException ::ExpectedArgListException(const Position& position)
    : ParserException(std::format("Expected argument list at: {}", position.get_position_str())) {}

ExpectedTypeSpecException::ExpectedTypeSpecException(const Position& position)
    : ParserException(std::format("Expected none or type at: {}", position.get_position_str())) {}

ExpectedTypeException::ExpectedTypeException(const Position& position)
    : ParserException(std::format("Expected type at: {}", position.get_position_str())) {}

ExpectedSemicolException::ExpectedSemicolException(const Position& position)
    : ParserException(std::format("Expected ';' at: {}", position.get_position_str())) {}

ExpectedTypedIdentifierException::ExpectedTypedIdentifierException(const Position& position)
    : ParserException(std::format("Expected identifier with specified type at: {}", position.get_position_str())) {}

ExpectedAssignmentException::ExpectedAssignmentException(const Position& position)
    : ParserException(std::format("Expected assignment operator '=' at: {}", position.get_position_str())) {}
// nieprzetestowane
ExpectedTypeForTypeCastException::ExpectedTypeForTypeCastException(const Position& position)
    : ParserException(std::format("Expected type for type cast at: {}", position.get_position_str())) {}

ExpectedExprAfterOrException::ExpectedExprAfterOrException(const Position& position)
    : ParserException(std::format("Expected expression after 'or' operator at: {}", position.get_position_str())) {}

ExpectedExprAfterAndException::ExpectedExprAfterAndException(const Position& position)
    : ParserException(std::format("Expected expression after 'and' operator at: {}", position.get_position_str())) {}

ExpectedExprAfterEqualityException::ExpectedExprAfterEqualityException(const Position& position)
    : ParserException(
          std::format("Expected expression after equality operator (==, !=) at: {}", position.get_position_str())) {}

ExpectedExprAfterComparisonException::ExpectedExprAfterComparisonException(const Position& position)
    : ParserException(std::format("Expected expression after comparison operator (<, <=, >, >=) at: {}",
                                  position.get_position_str())) {}

ExpectedExprAfterAdditiveException::ExpectedExprAfterAdditiveException(const Position& position)
    : ParserException(
          std::format("Expected expression after additive operator (+, -) at: {}", position.get_position_str())) {}

ExpectedExprAfterMultiplicativeException::ExpectedExprAfterMultiplicativeException(const Position& position)
    : ParserException(std::format("Expected expression after multiplicative operator (*, /) at: {}",
                                  position.get_position_str())) {}

ExpectedExprAfterUnaryException::ExpectedExprAfterUnaryException(const Position& position)
    : ParserException(
          std::format("Expected expression after unary operator (not, -): {}", position.get_position_str())) {}

ExpectedExprAfterFuncCompException::ExpectedExprAfterFuncCompException(const Position& position)
    : ParserException(
          std::format("Expected expression after function composition operator at: {}", position.get_position_str())) {}

ExpectedBindFrontOperatorException::ExpectedBindFrontOperatorException(const Position& position)
    : ParserException(std::format("Expected bind_front operator at: {}", position.get_position_str())) {}

ExpectedBindFrontTargetException::ExpectedBindFrontTargetException(const Position& position)
    : ParserException(std::format("Expected bind target at: {}", position.get_position_str())) {}

ExpectedIfConditionException::ExpectedIfConditionException(const Position& position)
    : ParserException(std::format("Expected condition(must be in parens) at: {}", position.get_position_str())) {}

ExpectedConditionalStatementBodyException::ExpectedConditionalStatementBodyException(const Position& position)
    : ParserException(std::format("Expected body for conditional statement at: {}", position.get_position_str())) {}

ExpectedLoopVarDeclException::ExpectedLoopVarDeclException(const Position& position)
    : ParserException(std::format("Expected loop variable declaration in at: {}", position.get_position_str())) {}

ExpectedLoopConditionException::ExpectedLoopConditionException(const Position& position)
    : ParserException(std::format("Expected condition in loop statement at: {}", position.get_position_str())) {}

ExpectedLoopBodyException::ExpectedLoopBodyException(const Position& position)
    : ParserException(std::format("Expected body for loop statement at: {}", position.get_position_str())) {}

ExpectedExprException::ExpectedExprException(const Position& position)
    : ParserException(std::format("Expected expression at: {}", position.get_position_str())) {}

ExpectedLoopVarUpdateException::ExpectedLoopVarUpdateException(const Position& position)
    : ParserException(std::format("Expected assignment as loop variable update at: {}", position.get_position_str())) {}

InvalidAssignTargetException::InvalidAssignTargetException(const Position& position)
    : ParserException(std::format("Invalid assignment target at: {}", position.get_position_str())) {}

InvalidFunctionParamTypeException::InvalidFunctionParamTypeException(const Position& position)
    : ParserException(std::format("Invalid type for param in function type spec at: {}", position.get_position_str())) {
}

ExpectedArrowException ::ExpectedArrowException(const Position& position)
    : ParserException(std::format("Expected '->' for return type specification at: {}", position.get_position_str())) {}

ExpectedRBraceException::ExpectedRBraceException(const Position& position)
    : ParserException(std::string("Expected '}' at: ") + position.get_position_str()) {}

ExpectedLParenException::ExpectedLParenException(const Position& position)
    : ParserException(std::format("Expected '(' at: {}", position.get_position_str())) {}

ExpectedRParenException::ExpectedRParenException(const Position& position)
    : ParserException(std::format("Expected ')' at: {}", position.get_position_str())) {}

ExpectedColException::ExpectedColException(const Position& position)
    : ParserException(std::format("Expected ':' at: {}", position.get_position_str())) {}

ExpectedLessException::ExpectedLessException(const Position& position)
    : ParserException(std::format("Expected '<' for function param spec opening at: {}", position.get_position_str())) {
}

ExpectedGreaterException::ExpectedGreaterException(const Position& position)
    : ParserException(std::format("Expected '>' for function param spec closing at: {}", position.get_position_str())) {
}

ExpectedIdentifierException::ExpectedIdentifierException(const Position& position)
    : ParserException(std::format("Expected identifier at: {}", position.get_position_str())) {}

/* -----------------------------------------------------------------------------*
 *                             INTERPRETER ERRORS                               *
 *------------------------------------------------------------------------------*/
InterpreterException::InterpreterException(const std::string& msg) : std::runtime_error(msg) {}

ReturnTypeMismatchException::ReturnTypeMismatchException(const std::string& expected_type_str,
                                                         const std::string& got_type_str, const Position& pos)
    : InterpreterException(std::format("Return type mismatch. Expected type: {}, got: {} at: {}", expected_type_str,
                                       got_type_str, pos.get_position_str())) {}

AssignTypeMismatchException::AssignTypeMismatchException(const std::string& expected_type_str,
                                                         const std::string& got_type_str, const Position& pos)
    : InterpreterException(std::format("Type mismatch trying to assign a value. Expected type: {}, got: {} at: {}",
                                       expected_type_str, got_type_str, pos.get_position_str())) {}

CannotCastException::CannotCastException(const std::string& source_type_str, const std::string& dest_type_str,
                                         const Position& pos)
    : InterpreterException(std::format("Cannot perform type cast from: {} to: {} at: {}", source_type_str,
                                       dest_type_str, pos.get_position_str())) {}