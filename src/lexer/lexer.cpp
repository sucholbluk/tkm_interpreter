#include "lexer.hpp"

Lexer::Lexer(std::unique_ptr<SourceHandler> source_handler) : _source_handler{std::move(source_handler)} {
    _initialize_keywords_map();
    _initialize_operator_builders_map();
    _get_next_char();
}

Token Lexer::get_next_token() {
    auto it = _operator_builders_map.find(_character);
    if (it != _operator_builders_map.end()) {
        return it->second();
    }
    throw std::logic_error("Not implemented");
}

void Lexer::_get_next_char() {
    std::pair char_and_position = _source_handler.get()->get_char_and_position();
    _character = char_and_position.first;
    _position = char_and_position.second;
}

void Lexer::_initialize_keywords_map() {
    _keywords_map = {
        {"int", TokenType::T_INT},
        {"float", TokenType::T_FLOAT},
        {"bool", TokenType::T_BOOL},
        {"string", TokenType::T_STRING},
        {"function", TokenType::T_FUNCTION},
        {"none", TokenType::T_NONE},
        {"not", TokenType::T_NOT},
        {"and", TokenType::T_AND},
        {"or", TokenType::T_OR},
        {"def", TokenType::T_DEF},
        {"let", TokenType::T_LET},
        {"mut", TokenType::T_MUT},
        {"as", TokenType::T_AS},
        {"if", TokenType::T_IF},
        {"else", TokenType::T_ELSE},
        {"break", TokenType::T_BREAK},
        {"continue", TokenType::T_CONTINUE},
        {"return", TokenType::T_RETURN},
        {"true", TokenType::T_TRUE},
        {"false", TokenType::T_FALSE},
        {"for", TokenType::T_FOR},
    };
}

void Lexer::_initialize_operator_builders_map() {
    _operator_builders_map = {
        {'(', _create_unequivocal_operator_builder(TokenType::T_L_PAREN)},
        {')', _create_unequivocal_operator_builder(TokenType::T_R_PAREN)},
        {'{', _create_unequivocal_operator_builder(TokenType::T_L_BRACE)},
        {'}', _create_unequivocal_operator_builder(TokenType::T_R_BRACE)},
        {',', _create_unequivocal_operator_builder(TokenType::T_COMMA)},
        {';', _create_unequivocal_operator_builder(TokenType::T_SEMICOLON)},
        {':', _create_unequivocal_operator_builder(TokenType::T_COLON)},
        {'"', _create_unequivocal_operator_builder(TokenType::T_DOUBLE_QUOTE)},
        {'&', _create_unequivocal_operator_builder(TokenType::T_FUNC_COMPOSITION)},
        {'+', _create_unequivocal_operator_builder(TokenType::T_PLUS)},
        {'*', _create_unequivocal_operator_builder(TokenType::T_MULTIPLY)},
        {'/', _create_unequivocal_operator_builder(TokenType::T_DIVIDE)},
        {EOF_CHAR, _create_unequivocal_operator_builder(TokenType::T_EOF)},

        {'-', _create_equivocal_operator_builder(TokenType::T_MINUS, '>', TokenType::T_ARROW)},
        {'=', _create_equivocal_operator_builder(TokenType::T_ASSIGN, '=', TokenType::T_EQUAL)},
        {'<', _create_equivocal_operator_builder(TokenType::T_LESS, '=', TokenType::T_LESS_EQUAL)},
        {'>', _create_equivocal_operator_builder(TokenType::T_GREATER, '=', TokenType::T_GREATER_EQUAL,
                                                 '>', TokenType::T_BIND_FRONT)},
        {'!', [this]() -> Token {
             Position position = this->_position;
             this->_get_next_char();
             if (this->_character != '=')
                 throw UnexpectedCharacterException(position.get_position_str());
             this->_get_next_char();
             return Token(TokenType::T_NOT_EQUAL, position);
         }},
    };
}

std::function<Token()> Lexer::_create_unequivocal_operator_builder(TokenType type) {
    return [this, type]() -> Token {
        Token single_operator_token{type, this->_position};
        this->_get_next_char();
        return single_operator_token;
    };
}

std::function<Token()> Lexer::_create_equivocal_operator_builder(TokenType type,
                                                                 char lookahead_char,
                                                                 TokenType extended_type) {
    return [this, type, lookahead_char, extended_type]() -> Token {
        Token built_token{type, this->_position};
        this->_get_next_char();
        if (this->_character == lookahead_char) {
            built_token = Token{extended_type, built_token.get_position()};
            this->_get_next_char();
        }
        return built_token;
    };
}

std::function<Token()> Lexer::_create_equivocal_operator_builder(TokenType type,
                                                                 char lookahead_char_opt_1,
                                                                 TokenType extended_type_opt_1,
                                                                 char lookahead_char_opt_2,
                                                                 TokenType extended_type_opt_2) {
    return [this,
            type,
            lookahead_char_opt_1,
            extended_type_opt_1,
            lookahead_char_opt_2,
            extended_type_opt_2]() {
        Token built_token{type, this->_position};
        this->_get_next_char();

        if (this->_character == lookahead_char_opt_1) {
            built_token = Token{extended_type_opt_1, built_token.get_position()};
            this->_get_next_char();
        } else if (this->_character == lookahead_char_opt_2) {
            built_token = Token{extended_type_opt_2, built_token.get_position()};
            this->_get_next_char();
        }
        return built_token;
    };
}