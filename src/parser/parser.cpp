#include "parser.hpp"

Parser::Parser(std::unique_ptr<ILexer> lexer) : _lexer{std::move(lexer)}, _token{_lexer->get_next_token()} {};

std::unique_ptr<Program> Parser::parse_program() {
    Position position{_token.get_position()};
    up_statement_vec statements{};

    while (auto statement = _try_parse_statement()) {
        statements.push_back(std::move(statement));
    }

    _token_must_be(TokenType::T_EOF);

    return std::make_unique<Program>(position, std::move(statements));
}

/* -----------------------------------------------------------------------------*
 *                             PARSING STATEMENTS                               *
 *------------------------------------------------------------------------------*/

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

up_statement Parser::_try_parse_variable_declaration() {
    if (not _token_type_is(TokenType::T_LET))
        return nullptr;

    Position position{_token.get_position()};
    _get_next_token();

    up_typed_identifier typed_identifier = _try_parse_typed_identifier();

    if (not typed_identifier)
        throw std::invalid_argument("couldnt parse typed identifier");  // TODO: replace

    _token_must_be(TokenType::T_ASSIGN);
    _get_next_token();

    auto assigned_expression = _try_parse_expression();
    if (not assigned_expression)
        // dobra już widze że to zaczyna sie powtarzać, do upakowania w inną funkcje np _must_be() po prostu
        throw std::invalid_argument("required expression to assign");  // TODO: replace - eee chociaz może jak działam na unique ptr to nie warto

    _token_must_be(TokenType::T_SEMICOLON);
    return std::make_unique<VariableDeclarationStatement>(position, std::move(typed_identifier),
                                                          std::move(assigned_expression));
}

/* -----------------------------------------------------------------------------*
 *                             PARSING EXPRESSIONS                              *
 *------------------------------------------------------------------------------*/
// TODO

/* -----------------------------------------------------------------------------*
 *                               TYPED IDENTIFIER                               *
 *------------------------------------------------------------------------------*/

up_typed_identifier Parser::_try_parse_typed_identifier() {
    bool is_mutable{false};
    Position position{_token.get_position()};

    if (_token_type_is(TokenType::T_MUT)) {
        is_mutable = true;
        _get_next_token();
        _token_must_be(TokenType::T_IDENTIFIER);  // już zjedliśmy token, teraz wymagamy identyfikatora
    } else if (not _token_type_is(TokenType::T_IDENTIFIER)) {
        return nullptr;
    }

    std::string identifier{_token.get_value_as<std::string>()};

    std::optional<Type> type = _try_parse_type();
    if (not type.has_value())
        throw std::invalid_argument("couldnt parse typed identifier");  // TODO: replace

    return std::make_unique<TypedIdentifier>(position, identifier, VariableType{type.value(), is_mutable});
}

/* -----------------------------------------------------------------------------*
 *                                   TYPE                                       *
 *------------------------------------------------------------------------------*/
std::optional<Type> Parser::_try_parse_type() {
    switch (_token.get_type()) {
        // int
        case TokenType::T_INT:
            _get_next_token();
            return Type{TypeKind::INT};
        // float
        case TokenType::T_FLOAT:
            _get_next_token();
            return Type{TypeKind::FLOAT};
        // string
        case TokenType::T_STRING:
            _get_next_token();
            return Type{TypeKind::STRING};
        // bool
        case TokenType::T_BOOL:
            _get_next_token();
            return Type{TypeKind::BOOL};
        // example: function<mut int,float:none>
        case TokenType::T_FUNCTION:
            _get_next_token();
        // not a type
        default:
            return std::nullopt;
    }
}
// examples: <int:string>, <float,float:float>, <function<string,mut string:none>,string:function<mut string:none>
FunctionTypeInfo Parser::_try_parse_function_type_info() {
    _token_must_be(TokenType::T_LESS);
    _get_next_token();

    std::vector<VariableType> params{};  // if none, then params vector is empty
    if (not _token_type_is(TokenType::T_NONE)) {
        std::optional<VariableType> param{};
        do {
            param = _try_parse_function_param_type();

            if (not param.has_value())
                throw std::invalid_argument("expected type");  // TODO: replace

            params.push_back(std::move(*param));
        } while (_token_type_is(TokenType::T_COMMA));
    }
    _token_must_be(TokenType::T_COLON);
    _get_next_token();

    std::optional<Type> return_type{};

    if (_token_type_is(TokenType::T_NONE)) {
        _get_next_token();
    } else {
        return_type = _try_parse_type();
        if (not return_type.has_value())
            throw std::invalid_argument("expected return type");
    }

    _token_must_be(TokenType::T_GREATER);
    _get_next_token();

    return FunctionTypeInfo{params, return_type};
}

std::optional<VariableType> Parser::_try_parse_function_param_type() {
    bool is_mutable{false};
    if (_token_type_is(TokenType::T_MUT)) {
        is_mutable = true;
        _get_next_token();
    }
    std::optional<Type> type{_try_parse_type()};

    if (not type.has_value()) {
        if (is_mutable)                                                  // przejedliśmy mut a nie dostaliśmy typu -> błąd
            throw std::invalid_argument("couldnt parse variable type");  // TODO: replace
        return std::nullopt;                                             // nie było mut - po prostu nie sparsowaliśmy typu
    }

    return VariableType{type.value(), is_mutable};
}

/* -----------------------------------------------------------------------------*
 *                                  OTHER                                       *
 *------------------------------------------------------------------------------*/

void Parser::_get_next_token() {
    _token = _lexer->get_next_token();
}

void Parser::_token_must_be(TokenType token_type) const {
    if (not _token_type_is(token_type))
        throw std::invalid_argument("required token of type ...");  // TODO: create custom exception for ts
}

bool Parser::_token_type_is(TokenType token_type) const {
    return _token.get_type() == token_type;
}