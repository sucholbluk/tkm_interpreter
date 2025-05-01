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

    up_expression _try_parse_expression();
    up_expression _try_parse_assigned_expression();

    up_typed_identifier _try_parse_typed_identifier();

    std::optional<Type> _try_parse_type();
    FunctionTypeInfo _parse_function_type_info();
    std::optional<VariableType> _try_parse_function_param_type();

    void _token_must_be(TokenType token_type) const;
    bool _token_type_is(TokenType token_type) const;

    std::unique_ptr<ILexer> _lexer;
    Token _token;
    static const std::unordered_map<TokenType, std::function<up_statement(Parser&)>> _simple_statement_builders_map;
};
#endif  // PARSER_HPP
