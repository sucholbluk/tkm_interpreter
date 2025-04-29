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

    _token_must_be(TokenType::T_EOF);

    return std::make_unique<Program>(position, std::move(statements));
}

up_statement Parser::_try_parse_statement() {
    up_statement statement;
    if ((statement = _try_parse_continue_statement())) return statement;

    if ((statement = _try_parse_break_statement())) return statement;

    if ((statement = _try_parse_return_statement())) return statement;

    if ((statement = _try_parse_variable_declaration())) return statement;

    if ((statement = _try_parse_code_block())) return statement;

    if ((statement = _try_parse_if_statement())) return statement;

    if ((statement = _try_parse_for_loop())) return statement;

    if ((statement = _try_parse_function_definition())) return statement;

    if ((statement = _try_parse_assignment_or_expression_statement())) return statement;

    return nullptr;
}

up_statement Parser::_try_parse_continue_statement() {
    if (not _token_type_is(TokenType::T_CONTINUE))
        return nullptr;
    Position position{_token.get_position()};
    _get_next_token();

    _token_must_be(TokenType::T_SEMICOLON);

    return std::make_unique<ContinueStatement>(position);
}

up_statement Parser::_try_parse_break_statement() {
    if (not _token_type_is(TokenType::T_BREAK))
        return nullptr;
    Position position{_token.get_position()};
    _get_next_token();

    _token_must_be(TokenType::T_SEMICOLON);

    return std::make_unique<BreakStatement>(position);
}

up_statement Parser::_try_parse_return_statement() {
    if (not _token_type_is(TokenType::T_RETURN))
        return nullptr;

    Position position{_token.get_position()};
    _get_next_token();
    auto expression = _try_parse_expression();

    _token_must_be(TokenType::T_SEMICOLON);

    _get_next_token();
    return std::make_unique<ReturnStatement>(position, std::move(expression));
}

void Parser::_token_must_be(TokenType token_type) const {
    if (not _token_type_is(token_type))
        throw std::invalid_argument("required token of type ...");  // TODO: create custom exception for ts
}

bool Parser::_token_type_is(TokenType token_type) const {
    return _token.get_type() == token_type;
}