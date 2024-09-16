#ifndef LEXER_H
#define LEXER_H

#include <stddef.h> // For size_t

// Token types
// Add more token types as needed
typedef enum
{
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_ASSIGN,
    TOKEN_FLOAT,
    TOKEN_INT,
    // NODE_LITERAL,
    TOKEN_NUMBER,
    TOKEN_PRINT,
    TOKEN_BOOL,
    TOKEN_KEYWORD,
    TOKEN_LBRACE,                // {
    TOKEN_RBRACE,                // }
    TOKEN_SEMICOLON,             // ;
    TOKEN_LPAREN,                // (
    TOKEN_RPAREN,                // )
    TOKEN_LBRACKET,              // [
    TOKEN_RBRACKET,              // ]
    TOKEN_COMMA,                 // ,
    TOKEN_DOT,                   // .
    TOKEN_PLUS,                  // +
    TOKEN_MINUS,                 // -
    TOKEN_MULTIPLY,              // *
    TOKEN_DIVIDE,                // /
    TOKEN_MODULUS,               // %
    TOKEN_POWER,                 // **
    TOKEN_NEWLINE,               // \n
    TOKEN_EQUAL,                 // ==
    TOKEN_NOT_EQUAL,             // !=
    TOKEN_GREATER_THAN,          // >
    TOKEN_LESS_THAN,             // <
    TOKEN_GREATER_THAN_OR_EQUAL, // >=
    TOKEN_LESS_THAN_OR_EQUAL,    // <=
    TOKEN_AND,                   // &&
    TOKEN_OR,                    // ||
    TOKEN_NOT,                   // !
    TOKEN_BITWISE_AND,           // &
    TOKEN_BITWISE_OR,            // |
    TOKEN_BITWISE_XOR,           // ^
    TOKEN_BITWISE_NOT,           // ~
    TOKEN_BITWISE_SHIFT_LEFT,    // <<
    TOKEN_BITWISE_SHIFT_RIGHT,   // >>
    TOKEN_UNKNOWN,               // For
    TOKEN_INT_TYPE,
    TOKEN_FLOAT_TYPE,
    TOKEN_CHAR_TYPE,
    TOKEN_SHORT_TYPE,
    TOKEN_BOOL_TYPE,
    TOKEN_LONG_TYPE,
    TOKEN_UNSIGNED_TYPE,
    TOKEN_SIGNED_TYPE,
    TOKEN_DOUBLE_TYPE,
    TOKEN_STRING_TYPE,
} TokenType;

// Define keywords
typedef enum
{
    KEYWORD_PRINT,
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_ELSEIF,
    KEYWORD_BREAK,
    KEYWORD_CONTINUE,
    KEYWORD_INT,
    KEYWORD_FLOAT,
    KEYWORD_BOOL,
    // KEYWORD_STRING,
    KEYWORD_VOID,
    KEYWORD_RETURN,
    KEYWORD_CONST,
    KEYWORD_CHAR,
    KEYWORD_SHORT,
    KEYWORD_LONG,
    KEYWORD_UNSIGNED,
    KEYWORD_SIGNED,
    KEYWORD_DOUBLE,
    KEYWORD_STRING,
    // Add other keywords here
} KeywordType;

// Token structure
typedef struct
{
    TokenType type;
    union
    {
        char *value;         // For identifiers and literals
        KeywordType keyword; // For keywords
    };
} Token;

// Lexer structure
typedef struct
{
    const char *input;
    size_t position;      // Current position in the input
    size_t read_position; // Current reading position in the input
    char current_char;
} Lexer;

/**
 * @brief Initializes a new lexer with the given input.
 * 
 * @param input The source code string to be tokenized.
 * @return Lexer* A pointer to the newly created Lexer structure.
 */
Lexer *init_lexer(const char *input);

/**
 * @brief Advances the lexer to the next character in the input.
 * 
 * @param lexer A pointer to the Lexer structure.
 */
void advance(Lexer *lexer);

/**
 * @brief Peeks at the next character in the input without advancing the lexer.
 * 
 * @param lexer A pointer to the Lexer structure.
 * @return char The next character in the input.
 */
char peek_char(Lexer *lexer);

/**
 * @brief Retrieves the next token from the input.
 * 
 * @param lexer A pointer to the Lexer structure.
 * @return Token* A pointer to the next Token in the input.
 */
Token *next_token(Lexer *lexer);

#endif // LEXER_H
