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
// continue;
up_statement Parser::_try_parse_continue_statement() {
    if (not _token_type_is(TokenType::T_CONTINUE)) return nullptr;
    Position position{_token.get_position()};
    _get_next_token();

    _token_must_be(TokenType::T_SEMICOLON);
    _get_next_token();

    return std::make_unique<ContinueStatement>(position);
}
// break;
up_statement Parser::_try_parse_break_statement() {
    if (not _token_type_is(TokenType::T_BREAK)) return nullptr;
    Position position{_token.get_position()};
    _get_next_token();

    _token_must_be(TokenType::T_SEMICOLON);
    _get_next_token();

    return std::make_unique<BreakStatement>(position);
}
// return [ expression ]; examples: return 4 + 8, return is_sth or foo(a, b, b+5) ;
up_statement Parser::_try_parse_return_statement() {
    if (not _token_type_is(TokenType::T_RETURN)) return nullptr;

    Position position{_token.get_position()};
    _get_next_token();
    auto expression = _try_parse_expression();

    _token_must_be(TokenType::T_SEMICOLON);
    _get_next_token();

    _get_next_token();
    return std::make_unique<ReturnStatement>(position, std::move(expression));
}
// let [ mut ] identifier: type = expression;
// examples: let mut i: int = 4;, let foo: function<int,int:bool> = foo2();
up_statement Parser::_try_parse_variable_declaration() {
    if (not _token_type_is(TokenType::T_LET)) return nullptr;

    Position position{_token.get_position()};
    _get_next_token();

    up_typed_identifier typed_identifier = _try_parse_typed_identifier();

    if (not typed_identifier) throw std::invalid_argument("couldnt parse typed identifier");  // TODO: replace

    _token_must_be(TokenType::T_ASSIGN);
    _get_next_token();

    auto assigned_expression{_try_parse_expression()};
    if (not assigned_expression)
        // dobra już widze że to zaczyna sie powtarzać, do upakowania w inną funkcje np _must_be() po prostu
        throw std::invalid_argument("required expression to assign");  // TODO: replace - eee chociaz może jak działam
                                                                       // na unique ptr to nie warto - do przemyslenia

    _token_must_be(TokenType::T_SEMICOLON);
    _get_next_token();
    return std::make_unique<VariableDeclarationStatement>(position, std::move(typed_identifier),
                                                          std::move(assigned_expression));
}

up_statement Parser::_try_parse_code_block() {
    return nullptr;
}
up_statement Parser::_try_parse_if_statement() {
    return nullptr;
}
up_statement Parser::_try_parse_for_loop() {
    return nullptr;
}
up_statement Parser::_try_parse_function_definition() {
    return nullptr;
}
up_statement Parser::_try_parse_assignment_or_expression_statement() {
    return nullptr;
}
up_statement Parser::_try_parse_expression_statement() {
    return nullptr;
}

/* -----------------------------------------------------------------------------*
 *                             PARSING EXPRESSIONS                              *
 *------------------------------------------------------------------------------*/
// TODO

up_expression Parser::_try_parse_expression() {
    return _try_parse_logical_or();
}
up_expression Parser::_try_parse_logical_or() {
    up_expression left{_try_parse_logical_and()};
    if (not left) {
        return nullptr;
    }

    while (_token_type_is(TokenType::T_OR)) {
        _get_next_token();
        up_expression right{_try_parse_logical_and()};
        if (not right) {
            throw std::runtime_error("missing expression after or");
        }  // TODO: custom exept

        left = std::make_unique<BinaryExpression>(left->position, ExpressionKind::LOGICAL_OR, std::move(left),
                                                  std::move(right));
    }
    return left;
}

up_expression Parser::_try_parse_logical_and() {
    up_expression left{_try_parse_equality_expression()};
    if (not left) {
        return nullptr;
    }

    while (_token_type_is(TokenType::T_AND)) {
        _get_next_token();
        up_expression right{_try_parse_equality_expression()};
        if (not right) {
            throw std::runtime_error("missing expression after and");
        }  // TODO: custom exept

        left = std::make_unique<BinaryExpression>(left->position, ExpressionKind::LOGICAL_AND, std::move(left),
                                                  std::move(right));
    }
    return left;
}

up_expression Parser::_try_parse_equality_expression() {
    up_expression left{_try_parse_comparison_expression()};
    if (not left) {
        return nullptr;
    }

    if (_token_type_is(TokenType::T_EQUAL) or _token_type_is(TokenType::T_NOT_EQUAL)) {
        ExpressionKind kind = _token_type_is(TokenType::T_EQUAL) ? ExpressionKind::EQUAL : ExpressionKind::NOT_EQUAL;
        _get_next_token();
        up_expression right{_try_parse_comparison_expression()};
        if (not right) {
            throw std::runtime_error("missing expression after eq/neq");
        }  // TODO: custom exept

        left = std::make_unique<BinaryExpression>(left->position, kind, std::move(left), std::move(right));
    }
    return left;
}

up_expression Parser::_try_parse_comparison_expression() {
    up_expression left{_try_parse_additive_expression()};
    if (not left) {
        return nullptr;
    }

    if (Parser::_comparison_tokens_to_expr_kind.contains(_token.get_type())) {
        ExpressionKind kind = Parser::_comparison_tokens_to_expr_kind.at(_token.get_type());
        _get_next_token();
        up_expression right{_try_parse_additive_expression()};
        if (not right) {
            throw std::runtime_error("missing expression after comparison op");
        }  // TODO: custom exept

        left = std::make_unique<BinaryExpression>(left->position, kind, std::move(left), std::move(right));
    }
    return left;
}

up_expression Parser::_try_parse_additive_expression() {
    up_expression left{_try_parse_multiplicative_expression()};
    if (not left) {
        return nullptr;
    }

    while (_token_type_is(TokenType::T_PLUS) or _token_type_is(TokenType::T_MINUS)) {
        ExpressionKind kind{_token_type_is(TokenType::T_PLUS) ? ExpressionKind::ADDITION : ExpressionKind::SUBTRACTION};
        _get_next_token();
        up_expression right{_try_parse_multiplicative_expression()};

        if (not right) {
            throw std::runtime_error("missing expression after additive oper");
        }  // TODO: custom exept

        left = std::make_unique<BinaryExpression>(left->position, kind, std::move(left), std::move(right));
    }
    return left;
}

up_expression Parser::_try_parse_multiplicative_expression() {
    up_expression left{_try_parse_type_cast()};
    if (not left) {
        return nullptr;
    }

    while (_token_type_is(TokenType::T_MULTIPLY) or _token_type_is(TokenType::T_DIVIDE)) {
        ExpressionKind kind =
            _token_type_is(TokenType::T_MULTIPLY) ? ExpressionKind::MULTIPICATION : ExpressionKind::DIVISION;

        _get_next_token();
        up_expression right{_try_parse_type_cast()};

        if (not right) {
            throw std::runtime_error("missing expression after multiplicative oper");
        }  // TODO: custom exept

        left = std::make_unique<BinaryExpression>(left->position, kind, std::move(left), std::move(right));
    }
    return left;
}

up_expression Parser::_try_parse_type_cast() {
    up_expression expr{_try_parse_unary_expression()};
    if (not expr) {
        return nullptr;
    }

    if (_token_type_is(TokenType::T_AS)) {
        _get_next_token();
        std::optional<Type> type{_try_parse_type()};
        if (not type.has_value()) {
            throw std::runtime_error("expected type after as");  // TODO
        }
        expr = std::make_unique<TypeCastExpression>(expr->position, std::move(expr), type.value());
    }
    return expr;
}

up_expression Parser::_try_parse_unary_expression() {
    if (not(_token_type_is(TokenType::T_NOT) or _token_type_is(TokenType::T_MINUS))) {
        return _try_parse_function_composition();  // to nie unary - delegujemy dalej
    }

    Position position{_token.get_position()};
    ExpressionKind kind = _token_type_is(TokenType::T_NOT) ? ExpressionKind::UNARY_MINUS : ExpressionKind::LOGICAL_NOT;

    _get_next_token();

    up_expression expr = _try_parse_function_composition();
    if (not expr) {
        throw std::runtime_error("unary operator must be followed with adequate expression");  // TODO
    }

    return std::make_unique<UnaryExpression>(position, kind, std::move(expr));
}

up_expression Parser::_try_parse_function_composition() {
    up_expression left{_try_parse_bind_front_or_function_call()};
    if (not left) {
        return nullptr;
    }
    while (_token_type_is(TokenType::T_FUNC_COMPOSITION)) {
        _get_next_token();
        up_expression right{_try_parse_bind_front_or_function_call()};
        if (not right) {
            throw std::runtime_error("func comp must be followed with adequate expression");  // TODO
        }
        left = std::make_unique<BinaryExpression>(left->position, ExpressionKind::FUNCTION_COMPOSITION, std::move(left),
                                                  std::move(right));
    }
    return left;
}

// EBNF bind_front = function_call | ( arg_list, bindf, function_call);
// examples:
//      bind front:
//          (4, 3) >> foo
//          (foo()) >> xff()
//          () >> foo - gramatically correct, but does nothing
//      function call: handles function call after ()
//          (foo())()
//          (foo(4, 3))()() - function foo returns function, that returns function and that function is called
//      parentesized expression:
//          (foo())
//          (4 + 8)
//          (is_foo and x == y)
up_expression Parser::_try_parse_bind_front_or_function_call() {
    Position position{_token.get_position()};  // po try_parse_argument_list już przejedzony obecny token

    std::optional<up_expression_vec> opt_argument_list = _try_parse_argument_list();
    if (not opt_argument_list.has_value()) {
        return _try_parse_function_call();
    }
    up_expression_vec argument_list{std::move(opt_argument_list.value())};

    if (not _token_type_is(TokenType::T_BIND_FRONT)) {
        if (argument_list.size() != 1) {
            throw std::runtime_error("missing >> after argument list");
        }
        return _try_parse_function_call(std::make_unique<ParenExpression>(position, std::move(argument_list[0])));
    }

    _get_next_token();
    up_expression target{_try_parse_function_call()};
    if (not target) {
        throw std::runtime_error("missing function target in bind front");
    }

    return std::make_unique<BindFront>(position, std::move(argument_list), std::move(target));
}

up_expression Parser::_try_parse_assigned_expression() {
    return nullptr;
}

/* -----------------------------------------------------------------------------*
 *                            PARSE_ARGUMENT_LIST                               *
 *------------------------------------------------------------------------------*/
std::optional<up_expression_vec> Parser::_try_parse_argument_list() {
    if (not _token_type_is(TokenType::T_L_PAREN)) {
        return std::nullopt;
    }
    up_expression_vec arguments{};
    Position position(_token.get_position());
    _get_next_token();

    up_expression argument = _try_parse_expression();
    if (not argument) {
        _token_must_be(TokenType::T_R_PAREN);
        _get_next_token();
        return arguments;
    }

    arguments.push_back(std::move(argument));

    while (_token_type_is(TokenType::T_COMMA)) {
        _get_next_token();
        argument = _try_parse_expression();
        if (not argument) {
            throw std::runtime_error("required argument after , in arg list");  // TODO
        }
        arguments.push_back(std::move(argument));
    }

    _token_must_be(TokenType::T_R_PAREN);
    _get_next_token();

    return arguments;
}

/* -----------------------------------------------------------------------------*
 *                               TYPED IDENTIFIER                               *
 *------------------------------------------------------------------------------*/
// [ mut ] identifier: type
// examples: mut fraction: float, a:int, fun_foo: function<mut float, int : none>
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

    std::optional<Type> type{_try_parse_type()};
    if (not type.has_value()) {
        throw std::invalid_argument("couldnt parse typed identifier");
    }  // TODO: replace

    return std::make_unique<TypedIdentifier>(position, identifier, VariableType{type.value(), is_mutable});
}

/* -----------------------------------------------------------------------------*
 *                                   TYPE                                       *
 *------------------------------------------------------------------------------*/
//
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
        // function type
        // example: function<mut int,float:none>
        case TokenType::T_FUNCTION:
            _get_next_token();
            return Type{_parse_function_type_info()};
        // not a type
        default:
            return std::nullopt;
    }
}
// examples: <int:string>, <float,float:float>, <function<string,mut string:none>,string:function<mut string:none>
FunctionTypeInfo Parser::_parse_function_type_info() {
    _token_must_be(TokenType::T_LESS);
    _get_next_token();

    std::vector<VariableType> params{};  // if none, then params vector is empty
    if (not _token_type_is(TokenType::T_NONE)) {
        std::optional<VariableType> param{};
        do {
            param = _try_parse_function_param_type();

            if (not param.has_value()) {
                throw std::invalid_argument("expected type");
            }  // TODO: replace

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
        if (not return_type.has_value()) {
            throw std::invalid_argument("expected return type");
        }
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
        if (is_mutable) {  // przejedliśmy mut a nie dostaliśmy typu -> błąd
            throw std::invalid_argument("couldnt parse variable type");  // TODO: replace
        }
        return std::nullopt;  // nie było mut - po prostu nie sparsowaliśmy typu
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

const std::unordered_map<TokenType, ExpressionKind> Parser::_comparison_tokens_to_expr_kind = {
    {TokenType::T_LESS, ExpressionKind::LESS},
    {TokenType::T_LESS_EQUAL, ExpressionKind::EQUAL},
    {TokenType::T_GREATER, ExpressionKind::GREATER},
    {TokenType::T_GREATER_EQUAL, ExpressionKind::NOT_EQUAL},
};