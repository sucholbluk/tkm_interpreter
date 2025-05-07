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

ExpectedArrowException ::ExpectedArrowException(const Position& position)
    : ParserException(std::format("Expected '->' for return type specification at: {}", position.get_position_str())) {}

ExpectedTypeSpecException::ExpectedTypeSpecException(const Position& position)
    : ParserException(std::format("Expected none or type at: {}", position.get_position_str())) {}

ExpectedTypeException::ExpectedTypeException(const Position& position)
    : ParserException(std::format("Expected type at: {}", position.get_position_str())) {}

ExpectedSemicolException::ExpectedSemicolException(const Position& position)
    : ParserException(std::format("Expected ';' at: {}", position.get_position_str())) {}

ExpectedTypedIdentifierException::ExpectedTypedIdentifierException(const Position& position)
    : ParserException(std::format("Expected identifier with specified type at: {}", position.get_position_str())) {}