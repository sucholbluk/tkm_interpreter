#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP
#include "enum.h"

BETTER_ENUM(TokenType, int,
            T_IDENTIFIER,

            T_INT,
            T_FLOAT,
            T_BOOL,
            T_STRING,
            T_FUNCTION,
            T_NONE,

            T_PLUS,
            T_MINUS,
            T_MULTIPLY,
            T_DIVIDE,
            T_ASSIGN,

            T_EQUAL,
            T_NOT_EQUAL,
            T_LESS_EQUAL,
            T_GREATER_EQUAL,
            T_LESS,
            T_GREATER,

            T_BIND_FRONT,
            T_FUNC_COMPOSITION,

            T_NOT,
            T_AND,
            T_OR,

            T_DEF,
            T_LET,
            T_MUT,
            T_AS,
            T_IF,
            T_ELSE,
            T_FOR,
            T_BREAK,
            T_CONTINUE,
            T_RETURN,

            T_L_PAREN,
            T_R_PAREN,
            T_L_BRACE,
            T_R_BRACE,
            T_COMMA,
            T_SEMICOLON,
            T_COLON,
            T_ARROW,
            T_DOUBLE_QUOTE,

            T_TRUE,
            T_FALSE,
            T_LITERAL_STRING,
            T_LITERAL_INT,
            T_LITERAL_FLOAT,

            T_COMMENT,
            T_EOF)

#endif  // TOKEN_TYPE_HPP
