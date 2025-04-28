#ifndef PARSER_HPP
#define PARSER_HPP
#include <memory>

#include "ilexer.hpp"
#include "program.hpp"
class Parser {
   public:
    Parser(std::unique_ptr<ILexer> lexer);
    std::unique_ptr<Program> parse_program();
    std::unique_ptr<Statement> try_parse_statement();

   private:
    void _get_next_token();

    std::unique_ptr<ILexer> _lexer;
    Token _token;
};
#endif  // PARSER_HPP
