#include "parser.hpp"

Parser::Parser(std::unique_ptr<ILexer> lexer) : _lexer{std::move(lexer)}, _token{_lexer->get_next_token()} {};

void Parser::_get_next_token() {
    _token = _lexer->get_next_token();
}

std::unique_ptr<Program> Parser::parse_program() {
    Position position{_token.get_position()};
    up_statement_vec statements{};

    while (auto statement = _try_parse_statement()) {
        statements.push_back(std::move(statement));
    }

    if (_token.get_type() != TokenType::T_EOF)
        throw std::runtime_error("expected end of text");

    return std::make_unique<Program>(position, std::move(statements));
}

up_statement Parser::_try_parse_statement() {
    up_statement statement;
    if ((statement = _try_parse_variable_declaration())) return statement;

    if ((statement = _try_parse_code_block())) return statement;

    if ((statement = _try_parse_if_statement())) return statement;

    if ((statement = _try_parse_for_loop())) return statement;

    if ((statement = _try_parse_return_statement())) return statement;

    if ((statement = _try_parse_break_statement())) return statement;

    if ((statement = _try_parse_continue_statement())) return statement;

    if ((statement = _try_parse_function_definition())) return statement;

    if ((statement = _try_parse_assignment_or_expression_statement())) return statement;

    return nullptr;
}

up_statement Parser::_try_parse_return_statement() {
    if (_token.get_type() != TokenType::T_RETURN)
        return nullptr;

    Position position{_token.get_position()};
    this->_get_next_token();
    auto expression = _try_parse_expression();

    if (_token.get_type() != TokenType::T_SEMICOLON)
        throw std::runtime_error(std::string{"semicolon required at:"} + position.get_position_str());

    this->_get_next_token();
    return std::make_unique<ReturnStatement>(position, std::move(expression));
}