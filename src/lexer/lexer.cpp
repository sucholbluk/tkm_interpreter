#include "lexer.hpp"

#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>

Lexer::Lexer(std::unique_ptr<SourceHandler> source_handler) : _source_handler{std::move(source_handler)} {
    _get_next_char();
}

Token Lexer::get_next_token() {
    _ignore_white_chars();

    if (auto it = _simple_builders_map.find(_character); it != _simple_builders_map.end()) {
        return std::invoke(it->second, *this);
    }

    if (auto token = _try_build_identifier_or_keyword()) {
        return *token;
    }
    if (auto token = _try_build_literal_int_or_float()) {
        return *token;
    }

    throw UnexpectedCharacterException(_position, _character);
}

void Lexer::_get_next_char() {
    std::pair char_and_position = _source_handler.get()->get_char_and_position();
    _character = char_and_position.first;
    _position = char_and_position.second;
}

void Lexer::_ignore_white_chars() {
    while (std::isspace(_character)) _get_next_char();
}

std::optional<Token> Lexer::_try_build_identifier_or_keyword() {
    if (not(std::isalpha(_character) or _character == '_')) {
        return std::nullopt;
    }

    std::string lexeme{};
    Position token_position{_position};

    do {
        if (lexeme.length() == MAX_IDENTIFIER_LEN) {
            throw IdentifierTooLongException(_position);
        }

        lexeme += _character;
        _get_next_char();
    } while (std::isalnum(_character) or _character == '_');

    if (auto it = _keywords_build_map.find(lexeme); it != _keywords_build_map.end()) {
        return it->second(token_position);
    }
    return Token{TokenType::T_IDENTIFIER, token_position, lexeme};
}

Token Lexer::_build_literal_string() {
    Position position{_position};
    std::string token_value{};
    _get_next_char();

    while (token_value.length() < MAX_STR_LITERAL_LEN and _character != '\"') {
        if (_character == '\n' or _character == EOF_CHAR) {
            throw UnfinishedStringException(_position);
        }
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

std::optional<Token> Lexer::_try_build_literal_int_or_float() {
    if (not std::isdigit(_character)) {
        return std::nullopt;
    }

    Position position{_position};
    int digit{_character - '0'};
    int integer_value{digit};
    _get_next_char();

    if (integer_value) {
        while (std::isdigit(_character)) {
            // equivalent to token_value * 10 + digit > INT_MAX
            if (integer_value > (std::numeric_limits<int>::max() - digit) / 10) {
                throw ParseIntOverflowException(position);
            }
            digit = _character - '0';
            integer_value = integer_value * 10 + digit;
            _get_next_char();
        }
    }

    if (_character != '.') {
        return Token{TokenType::T_LITERAL_INT, position, integer_value};
    }
    _get_next_char();

    if (not std::isdigit(_character)) {
        throw UnexpectedCharacterException(_position, _character);
    }

    unsigned long fraction_value{0};
    int fraction_digits{1};
    digit = _character - '0';
    fraction_value += digit;

    _get_next_char();
    while (std::isdigit(_character)) {
        digit = _character - '0';

        if (fraction_value > (std::numeric_limits<unsigned long>::max() - digit) / 10) {
            throw ParseFractionRangeExceededException(position);
        }
        fraction_value = fraction_value * 10 + digit;
        fraction_digits += 1;
        _get_next_char();
    }

    std::cout << std::to_string(fraction_value) << std::endl;
    double float_value = integer_value + ((double)fraction_value / std::pow(10.0, fraction_digits));

    return Token{TokenType::T_LITERAL_FLOAT, position, float_value};
}

std::function<Token(Lexer&)> Lexer::_create_unequivocal_operator_builder(TokenType type) {
    return [type](Lexer& lexer) -> Token {
        Token single_operator_token{type, lexer._position};
        lexer._get_next_char();
        return single_operator_token;
    };
}

std::function<Token(Lexer&)> Lexer::_create_equivocal_operator_builder(TokenType type, char lookahead_char,
                                                                       TokenType extended_type) {
    return [type, lookahead_char, extended_type](Lexer& lexer) -> Token {
        Position position{lexer._position};
        lexer._get_next_char();

        if (lexer._character == lookahead_char) {
            lexer._get_next_char();
            return Token{extended_type, position};
        }
        return Token{type, position};
    };
}

const std::unordered_map<std::string, std::function<Token(Position)>> Lexer::_keywords_build_map = {
    {"int",
     [](Position pos) {
         return Token{TokenType::T_INT, pos};
     }},
    {"float",
     [](Position pos) {
         return Token{TokenType::T_FLOAT, pos};
     }},
    {"bool",
     [](Position pos) {
         return Token{TokenType::T_BOOL, pos};
     }},
    {"string",
     [](Position pos) {
         return Token{TokenType::T_STRING, pos};
     }},
    {"function",
     [](Position pos) {
         return Token{TokenType::T_FUNCTION, pos};
     }},
    {"none",
     [](Position pos) {
         return Token{TokenType::T_NONE, pos};
     }},
    {"not",
     [](Position pos) {
         return Token{TokenType::T_NOT, pos};
     }},
    {"and",
     [](Position pos) {
         return Token{TokenType::T_AND, pos};
     }},
    {"or",
     [](Position pos) {
         return Token{TokenType::T_OR, pos};
     }},
    {"def",
     [](Position pos) {
         return Token{TokenType::T_DEF, pos};
     }},
    {"let",
     [](Position pos) {
         return Token{TokenType::T_LET, pos};
     }},
    {"mut",
     [](Position pos) {
         return Token{TokenType::T_MUT, pos};
     }},
    {"as",
     [](Position pos) {
         return Token{TokenType::T_AS, pos};
     }},
    {"if",
     [](Position pos) {
         return Token{TokenType::T_IF, pos};
     }},
    {"else",
     [](Position pos) {
         return Token{TokenType::T_ELSE, pos};
     }},
    {"break",
     [](Position pos) {
         return Token{TokenType::T_BREAK, pos};
     }},
    {"continue",
     [](Position pos) {
         return Token{TokenType::T_CONTINUE, pos};
     }},
    {"return",
     [](Position pos) {
         return Token{TokenType::T_RETURN, pos};
     }},
    {"for",
     [](Position pos) {
         return Token{TokenType::T_FOR, pos};
     }},
    {"true",
     [](Position pos) {
         return Token{TokenType::T_LITERAL_BOOL, pos, true};
     }},
    {"false",
     [](Position pos) {
         return Token{TokenType::T_LITERAL_BOOL, pos, false};
     }},
};

const std::unordered_map<char, std::function<Token(Lexer&)>> Lexer::_simple_builders_map = {
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
    {'\"', [](Lexer& lexer) -> Token { return lexer._build_literal_string(); }},
    {'>',
     [](Lexer& lexer) -> Token {  // can be >, >= or >>
         Position position{lexer._position};
         lexer._get_next_char();

         if (lexer._character == '=') {
             lexer._get_next_char();
             return Token{TokenType::T_GREATER_EQUAL, position};
         } else if (lexer._character == '>') {
             lexer._get_next_char();
             return Token{TokenType::T_BIND_FRONT, position};
         }
         return Token{TokenType::T_GREATER, position};
     }},
    {'!',
     [](Lexer& lexer) -> Token {
         Position position{lexer._position};
         lexer._get_next_char();
         if (lexer._character != '=') throw UnexpectedCharacterException(position, lexer._character);
         lexer._get_next_char();
         return Token{TokenType::T_NOT_EQUAL, position};
     }},
    {'#',
     [](Lexer& lexer) -> Token {
         Position position{lexer._position};
         std::string token_value{};
         lexer._get_next_char();
         while (token_value.length() < MAX_COMMENT_LEN and lexer._character != '\n' and lexer._character != EOF_CHAR) {
             token_value += lexer._character;
             lexer._get_next_char();
         }
         return Token{TokenType::T_COMMENT, position, token_value};
     }},
};
