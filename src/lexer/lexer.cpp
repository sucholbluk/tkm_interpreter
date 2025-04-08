#include "lexer.hpp"

#include <cmath>
#include <limits>

Lexer::Lexer(std::unique_ptr<SourceHandler> source_handler) : _source_handler{std::move(source_handler)} {
    _initialize_keywords_map();
    _initialize_operator_builders_map();
    _get_next_char();
}

Token Lexer::get_next_token() {
    if (auto it = _operator_builders_map.find(_character); it != _operator_builders_map.end()) {
        return it->second();
    }

    if (std::isalpha(_character) or _character == '_')
        return _build_identifier_or_keyword();

    if (std::isdigit(_character))
        return _build_literal_int_or_float();

    throw std::logic_error("Not implemented");
}

void Lexer::_get_next_char() {
    std::pair char_and_position = _source_handler.get()->get_char_and_position();
    _character = char_and_position.first;
    _position = char_and_position.second;
}

Token Lexer::_build_identifier_or_keyword() {
    std::string token_value{};
    Position token_position{_position};

    do {
        token_value += _character;
        _get_next_char();
    } while (token_value.length() < MAX_IDENTIFIER_LEN and (std::isalnum(_character) or _character == '_'));

    if (auto it = _keywords_map.find(token_value); it != _keywords_map.end()) {
        return Token{it->second, token_position};
    }
    return Token{TokenType::T_IDENTIFIER, token_position, token_value};
}

Token Lexer::_build_literal_string() {
    Position position{_position};
    std::string token_value{};
    _get_next_char();

    while (token_value.length() < MAX_STR_LITERAL_LEN and _character != '\"') {
        if (_character == '\n' or _character == EOF_CHAR)
            throw UnfinishedStringException(_position.get_position_str());
        if (_character == '\\') {
            _get_next_char();
            switch (_character) {
                case 'n':
                    token_value += '\n';
                    break;
                case 't':
                    token_value += '\t';
                    break;
                case '\\':
                    token_value += '\\';
                    break;
                case '\"':
                    token_value += '\"';
                    break;
                default:
                    token_value += _character;
                    break;
            }
            _get_next_char();
            continue;
        }
        token_value += _character;
        _get_next_char();
    }
    _get_next_char();

    return Token{TokenType::T_LITERAL_STRING, position, token_value};
}

// probably to change
Token Lexer::_build_literal_int_or_float() {
    Position position{_position};
    int digit{_character - '0'};
    int integer_value{0};
    _get_next_char();

    if (digit) {
        while (std::isdigit(_character)) {
            if (integer_value > (std::numeric_limits<int>::max() - digit) / 10) {  // equivalent to token_value * 10 + digit > INT_MAX
                throw IntValueOverflowException(position.get_position_str());
            }
            integer_value = integer_value * 10 + digit;
            digit = _character - '0';
            _get_next_char();
        }
    }

    if (_character != '.')
        return Token{TokenType::T_LITERAL_INT, position, integer_value};
    _get_next_char();

    if (not std::isdigit(_character))
        throw UnexpectedCharacterException(_position.get_position_str());

    unsigned long fraction_value{0};
    int fraction_digits{0};
    digit = _character - '0';
    while (std::isdigit(_character)) {
        if (fraction_value > (std::numeric_limits<unsigned long>::max() - digit) / 10) {  // lets say 20 decimal nums will be enough
            throw std::runtime_error("moze trzeba będzie to lepiej rozegrać");
        }
        fraction_value *= 10;
        fraction_value += digit;
        fraction_digits += 1;
        digit = _character - '0';
        _get_next_char();
    }
    double float_value = integer_value + (fraction_value / std::pow(10.0, fraction_digits));

    return Token{TokenType::T_LITERAL_FLOAT, position, float_value};
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
        {'\"', [this]() -> Token { return this->_build_literal_string(); }},
        {'!', [this]() -> Token {
             Position position = this->_position;
             this->_get_next_char();
             if (this->_character != '=')
                 throw UnexpectedCharacterException(position.get_position_str());
             this->_get_next_char();
             return Token{TokenType::T_NOT_EQUAL, position};
         }},
        {'#', [this]() -> Token {
             Position position = this->_position;
             std::string token_value{};
             _get_next_char();
             while (token_value.length() < MAX_COMMENT_LEN and this->_character != '\n' and this->_character != EOF_CHAR) {
                 token_value += _character;
                 _get_next_char();
             }
             return Token{TokenType::T_COMMENT, position, token_value};
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