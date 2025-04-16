#ifndef LEXER_HPP
#define LEXER_HPP

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

#include "ilexer.hpp"
#include "source_handler.hpp"

/*
 * @brief Concrete Lexer. Builds tokens on requests.
 *
 * Always stores one character after previous token end. (or first character if none token was built)
 */
class Lexer : public ILexer {
   public:
    explicit Lexer(std::unique_ptr<SourceHandler> source_handler);
    Token get_next_token() override;

   private:
    std::unique_ptr<SourceHandler> _source_handler;
    char _character;
    Position _position;
    /*
     * @brief Maps current char to builder function/method simplifying build process.
     */
    std::unordered_map<char, std::function<Token()>> _simple_builders_map;
    /*
     * @brief Maps keywords as strings to correct types.
     *
     * Used to compare if we have built keyword or identifier
     */
    static const std::unordered_map<std::string, std::function<Token(Position)>> _keywords_build_map;

    void _get_next_char();

    void _initialize_operator_builders_map();
    void _ignore_white_chars();
    std::optional<Token> _try_build_identifier_or_keyword();
    std::optional<Token> _try_build_literal_int_or_float();
    Token _build_literal_string();
    /*
     * @brief returns builder function/method that has only 1 correct @ref TokenType
     */
    std::function<Token()> _create_unequivocal_operator_builder(TokenType type);
    /*
     * @bref returns builder function/method that has 2 possible token types.
     *
     * The type is determined by the character after the one that was used to map to this function
     * inside the @ref _simple_builders_map.
     */
    std::function<Token()> _create_equivocal_operator_builder(TokenType type,
                                                              char lookahead_char, TokenType extended_type);
};

#endif  // LEXER_HPP