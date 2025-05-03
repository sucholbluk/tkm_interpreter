#ifndef PARSER_HPP
#define PARSER_HPP
#include <functional>
#include <memory>
#include <unordered_map>

#include "expression.hpp"
#include "ilexer.hpp"
#include "program.hpp"
#include "statement.hpp"

class Parser {
   public:
    Parser(std::unique_ptr<ILexer> lexer);
    std::unique_ptr<Program> parse_program();

   private:
    void _get_next_token();

    up_statement _try_parse_statement();

    up_statement _try_parse_variable_declaration();
    up_statement _try_parse_code_block();
    up_statement _try_parse_if_statement();
    up_statement _try_parse_for_loop();
    up_statement _try_parse_return_statement();
    up_statement _try_parse_break_statement();
    up_statement _try_parse_continue_statement();
    up_statement _try_parse_function_definition();
    up_statement _try_parse_assignment_or_expression_statement();
    up_statement _try_parse_expression_statement();

    up_expression _try_parse_condition();
    up_expression _try_parse_expression();
    up_expression _try_parse_logical_or();
    up_expression _try_parse_logical_and();
    up_expression _try_parse_equality_expression();
    up_expression _try_parse_comparison_expression();
    up_expression _try_parse_additive_expression();
    up_expression _try_parse_multiplicative_expression();
    up_expression _try_parse_type_cast();
    up_expression _try_parse_unary_expression();
    up_expression _try_parse_function_composition();

    // jest wspolny początek dla (expression) i bind front - arg_list= (expr, ...)
    // możliwe, że próbując zbudować arg_list zbudujemy tylko (expr) i jesli następny token nie jest
    // >> to powinniśmy zwrócić expression, ale function call ma w języku wyższy priorytet
    // niż bind front więc żeby uchronić się przed uniemożliwieniem function calla o postaci
    // (expr)() sprawdzamy czy następny token to nie kolejny argument list itd (expr)()()...
    up_expression _try_parse_bind_front_or_function_call();
    up_expression _try_parse_function_call();
    up_expression _try_parse_function_call(up_expression paren_expr);
    up_expression _try_parse_primary();
    up_expression _try_parse_literal();
    up_expression _try_parse_identifier();
    std::optional<up_expression_vec> _try_parse_argument_list();

    up_expression _try_parse_assigned_expression();

    up_typed_identifier _try_parse_typed_identifier();

    std::optional<Type> _try_parse_type();
    FunctionTypeInfo _parse_function_type_info();
    std::optional<VariableType> _try_parse_function_param_type();

    void _token_must_be(TokenType token_type) const;
    bool _token_type_is(TokenType token_type) const;

    std::unique_ptr<ILexer> _lexer;
    Token _token;

    static const std::unordered_map<TokenType, ExpressionKind> _comparison_tokens_to_expr_kind;
};
#endif  // PARSER_HPP
