#ifndef LEXER_HPP
#define LEXER_HPP

#include <functional>
#include <memory>
#include <unordered_map>

#include "source_handler.hpp"
#include "token.hpp"

class Lexer {
   public:
    explicit Lexer(std::unique_ptr<SourceHandler> source_handler);
    Token get_next_token();

   private:
    std::unique_ptr<SourceHandler> _source_handler;
    char _character;
    Position _position;
    std::unordered_map<char, std::function<Token()>> _operator_builders_map;
    std::unordered_map<std::string, TokenType> _keywords_map;

    void _get_next_char();

    void _initialize_operator_builders_map();
    void _initialize_keywords_map();
    Token _build_identifier_or_keyword();
    Token _build_literal_int_or_float();
    Token _build_literal_string();
    std::function<Token()> _create_unequivocal_operator_builder(TokenType type);
    std::function<Token()> _create_equivocal_operator_builder(TokenType type,
                                                              char lookahead_char, TokenType extended_type);
    std::function<Token()> _create_equivocal_operator_builder(TokenType type,
                                                              char lookahead_char_opt_1,
                                                              TokenType extended_type_opt_1,
                                                              char lookahead_char_opt_2,
                                                              TokenType extended_type_opt_2);
};

#endif  // LEXER_HPP