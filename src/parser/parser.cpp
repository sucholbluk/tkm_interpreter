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
    if (not _token_type_is(TokenType::T_CONTINUE)) {
        return nullptr;
    }

    Position position{_get_position_and_digest_token()};

    _advance_on_required_token(TokenType::T_SEMICOLON);

    return std::make_unique<ContinueStatement>(position);
}
// break;
up_statement Parser::_try_parse_break_statement() {
    if (not _token_type_is(TokenType::T_BREAK)) {
        return nullptr;
    }

    Position position{_get_position_and_digest_token()};

    _advance_on_required_token(TokenType::T_SEMICOLON);

    return std::make_unique<BreakStatement>(position);
}
// return [ expression ]; examples: return 4 + 8, return is_sth or foo(a, b, b+5) ;
up_statement Parser::_try_parse_return_statement() {
    if (not _token_type_is(TokenType::T_RETURN)) {
        return nullptr;
    }

    Position position{_get_position_and_digest_token()};
    up_expression expression = _try_parse_expression();

    _advance_on_required_token(TokenType::T_SEMICOLON);

    return std::make_unique<ReturnStatement>(position, std::move(expression));
}
// let [ mut ] identifier: type = expression;
// examples: let mut i: int = 4;, let foo: function<int,int:bool> = foo2();
up_statement Parser::_try_parse_variable_declaration() {
    if (not _token_type_is(TokenType::T_LET)) {
        return nullptr;
    }

    Position position{_get_position_and_digest_token()};

    up_typed_identifier typed_identifier = _try_parse_typed_identifier();

    if (not typed_identifier) throw std::invalid_argument("couldnt parse typed identifier");  // TODO: replace

    _advance_on_required_token(TokenType::T_ASSIGN);

    up_expression assigned_expression{_try_parse_expression()};
    if (not assigned_expression)
        // dobra już widze że to zaczyna sie powtarzać, do upakowania w inną funkcje np _must_be() po prostu
        throw std::invalid_argument("required expression to assign");  // TODO: replace - eee chociaz może jak działam
                                                                       // na unique ptr to nie warto - do przemyslenia

    _advance_on_required_token(TokenType::T_SEMICOLON);

    return std::make_unique<VariableDeclaration>(position, std::move(typed_identifier), std::move(assigned_expression));
}

up_statement Parser::_try_parse_code_block() {
    if (not _token_type_is(TokenType::T_L_BRACE)) {
        return nullptr;
    }

    Position position{_get_position_and_digest_token()};

    up_statement_vec statements{};
    while (up_statement statement = _try_parse_statement()) {
        statements.push_back(std::move(statement));
    }

    _advance_on_required_token(TokenType::T_R_BRACE);

    return std::make_unique<CodeBlock>(position, std::move(statements));
}
up_statement Parser::_try_parse_if_statement() {
    if (not _token_type_is(TokenType::T_IF)) {
        return nullptr;
    }
    Position if_position{_token.get_position()};
    _get_next_token();

    up_expression condition{_try_parse_condition()};

    if (not condition) {
        throw std::runtime_error("missing if condition");
    }

    up_statement if_body{_try_parse_code_block()};

    if (not if_body) {
        throw std::runtime_error("required if code block");
    }

    up_else_if_vec else_ifs{};
    up_statement else_block{};
    while (_token_type_is(TokenType::T_ELSE)) {
        Position else_if_position{_token.get_position()};
        _get_next_token();

        if (not _token_type_is(TokenType::T_IF)) {
            else_block = _try_parse_code_block();
            if (not else_block) {
                throw std::runtime_error("required body after else");
            }
            break;
        }

        _get_next_token();

        up_expression else_if_condition{_try_parse_condition()};
        if (not else_if_condition) {
            throw std::runtime_error("required condition after else if");
        }

        up_statement else_if_body{_try_parse_code_block()};
        if (not else_if_body) {
            throw std::runtime_error("required body in else if");
        }

        else_ifs.push_back(
            std::make_unique<ElseIf>(else_if_position, std::move(else_if_condition), std::move(else_if_body)));
    }
    return std::make_unique<IfStatement>(if_position, std::move(condition), std::move(if_body), std::move(else_ifs),
                                         std::move(else_block));
}
up_statement Parser::_try_parse_for_loop() {
    return nullptr;
}
up_statement Parser::_try_parse_function_definition() {
    return nullptr;
}
up_statement Parser::_try_parse_assignment_or_expression_statement() {
    std::string identifier;
    if (_token_type_is(TokenType::T_IDENTIFIER)) {
        identifier = _token.get_value_as<std::string>();
    }

    up_expression expr{_try_parse_expression()};

    if (not expr) {
        return nullptr;
    }
    if (not _token_type_is(TokenType::T_ASSIGN)) {
        _advance_on_required_token(TokenType::T_SEMICOLON);
        return std::make_unique<ExpressionStatement>(expr->position, std::move(expr));
    }

    if (expr->kind != ExprKind::IDENTIFIER) {
        throw std::runtime_error("invalid assign target");
    }

    _get_next_token();
    up_expression assigned_expr{_try_parse_expression()};

    if (not assigned_expr) {
        throw std::runtime_error("assignment requires expression");
    }
    _advance_on_required_token(TokenType::T_SEMICOLON);

    return std::make_unique<AssignStatement>(expr->position, identifier, std::move(assigned_expr));
}

/* -----------------------------------------------------------------------------*
 *                             PARSING EXPRESSIONS                              *
 *------------------------------------------------------------------------------*/
up_expression Parser::_try_parse_condition() {
    if (not _token_type_is(TokenType::T_L_PAREN)) {
        return nullptr;
    }
    _get_next_token();

    up_expression condition{_try_parse_expression()};

    _advance_on_required_token(TokenType::T_R_PAREN);

    return condition;
}

up_expression Parser::_try_parse_expression() {
    return _try_parse_logical_or();
}
up_expression Parser::_try_parse_logical_or() {
    return _try_parse_chained_binary_expression([this]() { return _try_parse_logical_and(); }, Parser::_or_token_types,
                                                []() { throw std::runtime_error("missing expression after or"); });
}

up_expression Parser::_try_parse_logical_and() {
    return _try_parse_chained_binary_expression([this]() { return _try_parse_equality_expression(); },
                                                Parser::_and_token_types,
                                                []() { throw std::runtime_error("missing expression after and"); });
}

up_expression Parser::_try_parse_equality_expression() {
    return _try_parse_single_binary_expression([this]() { return _try_parse_comparison_expression(); },
                                               Parser::_equality_token_types,
                                               []() { throw std::runtime_error("missing expression after eq/neq"); });
}

up_expression Parser::_try_parse_comparison_expression() {
    return _try_parse_single_binary_expression(
        [this]() { return _try_parse_additive_expression(); }, Parser::_comparison_token_types,
        []() { throw std::runtime_error("missing expression after comparison op"); });
}

up_expression Parser::_try_parse_additive_expression() {
    return _try_parse_chained_binary_expression(
        [this]() { return _try_parse_multiplicative_expression(); }, Parser::_additive_token_types,
        []() { throw std::runtime_error("missing expression after additive oper"); });
}

up_expression Parser::_try_parse_multiplicative_expression() {
    return _try_parse_chained_binary_expression(
        [this]() { return _try_parse_type_cast(); }, Parser::_multipicative_token_types,
        []() { throw std::runtime_error("missing expression after multiplicative oper"); });
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

    ExprKind kind = _token_type_is(TokenType::T_NOT) ? ExprKind::UNARY_MINUS : ExprKind::LOGICAL_NOT;

    Position position{_get_position_and_digest_token()};

    up_expression expr = _try_parse_function_composition();
    if (not expr) {
        throw std::runtime_error("unary operator must be followed with adequate expression");  // TODO
    }

    return std::make_unique<UnaryExpression>(position, kind, std::move(expr));
}

up_expression Parser::_try_parse_function_composition() {
    return _try_parse_chained_binary_expression(
        [this]() { return _try_parse_bind_front_or_function_call(); }, Parser::_func_comp_token_types,
        []() { throw std::runtime_error("func comp must be followed with adequate expression"); });
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

up_expression Parser::_try_parse_function_call() {
    return _try_parse_function_call(_try_parse_primary());
}

up_expression Parser::_try_parse_function_call(up_expression paren_expr) {
    if (not paren_expr) {
        return nullptr;
    }
    up_expression callee{std::move(paren_expr)};

    while (std::optional<up_expression_vec> arg_list = _try_parse_argument_list()) {
        callee = std::make_unique<FunctionCall>(callee->position, std::move(callee), std::move(arg_list.value()));
    }
    return callee;
}

up_expression Parser::_try_parse_primary() {
    up_expression primary_expr{_try_parse_literal()};

    if (not primary_expr) {
        primary_expr = _try_parse_identifier();
        if (not primary_expr) {
            return nullptr;
        }
    }

    return primary_expr;
}

up_expression Parser::_try_parse_literal() {
    Position position{_token.get_position()};
    up_expression literal;
    switch (_token.get_type()) {
        case TokenType::T_LITERAL_INT:
            literal = std::make_unique<LiteralInt>(position, _token.get_value_as<int>());
            break;
        case TokenType::T_LITERAL_FLOAT:
            literal = std::make_unique<LiteralFloat>(position, _token.get_value_as<double>());
            break;
        case TokenType::T_LITERAL_STRING:
            literal = std::make_unique<LiteralString>(position, _token.get_value_as<std::string>());
            break;
        case TokenType::T_LITERAL_BOOL:
            literal = std::make_unique<LiteralBool>(position, _token.get_value_as<bool>());
            break;
        default:
            return nullptr;
    }
    _get_next_token();
    return literal;
}

up_expression Parser::_try_parse_identifier() {
    if (not _token_type_is(TokenType::T_IDENTIFIER)) {
        return nullptr;
    }
    up_expression idenitifier = std::make_unique<Identifier>(_token.get_position(), _token.get_value_as<std::string>());
    _get_next_token();
    return idenitifier;
}

/* -----------------------------------------------------------------------------*
 *                             PARSE_BINARY_EXPR                                *
 *------------------------------------------------------------------------------*/
up_expression Parser::_try_parse_chained_binary_expression(std::function<up_expression()> try_parse_subexpr,
                                                           const std::unordered_set<TokenType>& token_types,
                                                           std::function<void()> on_error) {
    up_expression left{try_parse_subexpr()};
    if (not left) {
        return nullptr;
    }

    while (token_types.contains(_token.get_type())) {
        ExprKind kind{Parser::_token_type_to_expr_kind.at(_token.get_type())};

        _get_next_token();
        up_expression right{try_parse_subexpr()};
        if (not right) {
            on_error();
        }
        left = std::make_unique<BinaryExpression>(left->position, kind, std::move(left), std::move(right));
    }

    return left;
}

up_expression Parser::_try_parse_single_binary_expression(std::function<up_expression()> try_parse_subexpr,
                                                          const std::unordered_set<TokenType>& token_types,
                                                          std::function<void()> on_error) {
    up_expression left{try_parse_subexpr()};
    if (not left) {
        return nullptr;
    }

    if (token_types.contains(_token.get_type())) {
        ExprKind kind{Parser::_token_type_to_expr_kind.at(_token.get_type())};

        _get_next_token();
        up_expression right{try_parse_subexpr()};
        if (not right) {
            on_error();
        }
        left = std::make_unique<BinaryExpression>(left->position, kind, std::move(left), std::move(right));
    }

    return left;
}

/* -----------------------------------------------------------------------------*
 *                            PARSE_ARGUMENT_LIST                               *
 *------------------------------------------------------------------------------*/
std::optional<up_expression_vec> Parser::_try_parse_argument_list() {
    if (not _token_type_is(TokenType::T_L_PAREN)) {
        return std::nullopt;
    }
    up_expression_vec arguments{};
    Position position(_get_position_and_digest_token());

    up_expression argument = _try_parse_expression();
    if (not argument) {
        _advance_on_required_token(TokenType::T_R_PAREN);
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

    _advance_on_required_token(TokenType::T_R_PAREN);

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
    _get_next_token();

    _advance_on_required_token(TokenType::T_COLON);

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
    _advance_on_required_token(TokenType::T_LESS);

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
    _advance_on_required_token(TokenType::T_COLON);

    std::optional<Type> return_type{};

    if (_token_type_is(TokenType::T_NONE)) {
        _get_next_token();
    } else {
        return_type = _try_parse_type();
        if (not return_type.has_value()) {
            throw std::invalid_argument("expected return type");
        }
    }

    _advance_on_required_token(TokenType::T_GREATER);

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

void Parser::_advance_on_required_token(TokenType token_type) {
    _token_must_be(token_type);
    _get_next_token();
}

Position Parser::_get_position_and_digest_token() {
    Position position{_token.get_position()};
    _get_next_token();
    return position;
}

void Parser::_token_must_be(TokenType token_type) const {
    if (not _token_type_is(token_type))
        throw std::invalid_argument("required token of type ...");  // TODO: create custom exception for ts
}

bool Parser::_token_type_is(TokenType token_type) const {
    return _token.get_type() == token_type;
}

const std::unordered_set<TokenType> Parser::_or_token_types = {TokenType::T_OR};

const std::unordered_set<TokenType> Parser::_and_token_types = {TokenType::T_AND};

const std::unordered_set<TokenType> Parser::_equality_token_types = {
    TokenType::T_EQUAL,
    TokenType::T_NOT_EQUAL,
};

const std::unordered_set<TokenType> Parser::_comparison_token_types = {
    TokenType::T_LESS,
    TokenType::T_LESS_EQUAL,
    TokenType::T_GREATER,
    TokenType::T_GREATER_EQUAL,
};
const std::unordered_set<TokenType> Parser::_additive_token_types = {
    TokenType::T_PLUS,
    TokenType::T_MINUS,
};
const std::unordered_set<TokenType> Parser::_multipicative_token_types = {
    TokenType::T_MULTIPLY,
    TokenType::T_DIVIDE,
};
const std::unordered_set<TokenType> Parser::_func_comp_token_types = {TokenType::T_FUNC_COMPOSITION};

const std::unordered_map<TokenType, ExprKind> Parser::_token_type_to_expr_kind = {
    {TokenType::T_OR, ExprKind::LOGICAL_OR},
    {TokenType::T_AND, ExprKind::LOGICAL_AND},
    {TokenType::T_EQUAL, ExprKind::EQUAL},
    {TokenType::T_NOT_EQUAL, ExprKind::NOT_EQUAL},
    {TokenType::T_LESS, ExprKind::LESS},
    {TokenType::T_LESS_EQUAL, ExprKind::LESS_EQUAL},
    {TokenType::T_GREATER, ExprKind::GREATER},
    {TokenType::T_GREATER_EQUAL, ExprKind::GREATER_EQUAL},
    {TokenType::T_PLUS, ExprKind::ADDITION},
    {TokenType::T_MINUS, ExprKind::SUBTRACTION},
    {TokenType::T_MULTIPLY, ExprKind::MULTIPICATION},
    {TokenType::T_DIVIDE, ExprKind::DIVISION},
    {TokenType::T_FUNC_COMPOSITION, ExprKind::FUNCTION_COMPOSITION},
};