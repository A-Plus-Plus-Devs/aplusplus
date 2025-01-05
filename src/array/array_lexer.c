#include "array_lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Helper function declarations
static void advance(ArrayLexer *lexer);
static char peek_char(ArrayLexer *lexer);
static void skip_whitespace(ArrayLexer *lexer);
static ArrayToken *create_token(ArrayTokenType type, const char *value, size_t line, size_t column);
static char *read_identifier(ArrayLexer *lexer);
static char *read_number(ArrayLexer *lexer);
static char *read_string(ArrayLexer *lexer);
static ArrayTokenType check_keyword(const char *identifier);

ArrayLexer *array_lexer_init(const char *input) {
    ArrayLexer *lexer = malloc(sizeof(ArrayLexer));
    if (!lexer) return NULL;

    lexer->input = input;
    lexer->position = 0;
    lexer->read_position = 0;
    lexer->current_char = '\0';
    lexer->line = 1;
    lexer->column = 0;

    advance(lexer); // Initialize current_char
    return lexer;
}

void array_lexer_free(ArrayLexer *lexer) {
    if (lexer) {
        free(lexer);
    }
}

static void advance(ArrayLexer *lexer) {
    if (lexer->read_position >= strlen(lexer->input)) {
        lexer->current_char = '\0';
    } else {
        lexer->current_char = lexer->input[lexer->read_position];
    }
    
    lexer->position = lexer->read_position;
    lexer->read_position++;
    lexer->column++;
}

static char peek_char(ArrayLexer *lexer) {
    if (lexer->read_position >= strlen(lexer->input)) {
        return '\0';
    }
    return lexer->input[lexer->read_position];
}

static void skip_whitespace(ArrayLexer *lexer) {
    while (isspace(lexer->current_char)) {
        if (lexer->current_char == '\n') {
            lexer->line++;
            lexer->column = 0;
        }
        advance(lexer);
    }
}

ArrayToken *array_lexer_next_token(ArrayLexer *lexer) {
    skip_whitespace(lexer);

    size_t current_line = lexer->line;
    size_t current_column = lexer->column;

    printf("Current char: %c\n", lexer->current_char);

    switch (lexer->current_char) {
        case '\0':
            return create_token(ARRAY_TOKEN_EOF, NULL, current_line, current_column);

        case '[':
            advance(lexer);
            return create_token(ARRAY_TOKEN_LBRACKET, "[", current_line, current_column);

        case ']':
            advance(lexer);
            return create_token(ARRAY_TOKEN_RBRACKET, "]", current_line, current_column);

        case ',':
            advance(lexer);
            return create_token(ARRAY_TOKEN_COMMA, ",", current_line, current_column);

        case '.':
            advance(lexer);
            return create_token(ARRAY_TOKEN_DOT, ".", current_line, current_column);

        case '<':
            advance(lexer);
            return create_token(ARRAY_TOKEN_LESS_THAN, "<", current_line, current_column);

        case '>':
            advance(lexer);
            return create_token(ARRAY_TOKEN_GREATER_THAN, ">", current_line, current_column);

        case '=':
            advance(lexer);
            return create_token(ARRAY_TOKEN_ASSIGN, "=", current_line, current_column);

        case '(':
            advance(lexer);
            return create_token(ARRAY_TOKEN_LPAREN, "(", current_line, current_column);

        case ')':
            advance(lexer);
            return create_token(ARRAY_TOKEN_RPAREN, ")", current_line, current_column);

        case ';':
            advance(lexer);
            return create_token(ARRAY_TOKEN_SEMICOLON, ";", current_line, current_column);

        case '"':
            return create_token(ARRAY_TOKEN_STRING, read_string(lexer), current_line, current_column);

        default:
            if (isalpha(lexer->current_char) || lexer->current_char == '_') {
                char *identifier = read_identifier(lexer);
                printf("Read identifier: %s\n", identifier);
                ArrayTokenType type = check_keyword(identifier);
                printf("Token type determined: %d\n", type);
                return create_token(type, identifier, current_line, current_column);
            }
            else if (isdigit(lexer->current_char)) {
                return create_token(ARRAY_TOKEN_NUMBER, read_number(lexer), current_line, current_column);
            }
            else {
                char error_msg[100];
                snprintf(error_msg, sizeof(error_msg), "Invalid character: %c", lexer->current_char);
                array_lexer_error(lexer, error_msg);
                advance(lexer);
                return create_token(ARRAY_TOKEN_ERROR, error_msg, current_line, current_column);
            }
    }
}

static ArrayToken *create_token(ArrayTokenType type, const char *value, size_t line, size_t column) {
    ArrayToken *token = malloc(sizeof(ArrayToken));
    if (!token) return NULL;

    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->line = line;
    token->column = column;

    return token;
}

void array_token_free(ArrayToken *token) {
    if (token) {
        free(token->value);
        free(token);
    }
}

static char *read_identifier(ArrayLexer *lexer) {
    size_t start = lexer->position;
    
    // Allow letters, digits, and underscores in identifiers
    while (isalnum(lexer->current_char) || lexer->current_char == '_') {
        advance(lexer);
    }
    
    size_t length = lexer->position - start;
    char *identifier = malloc(length + 1);
    if (!identifier) return NULL;
    
    strncpy(identifier, &lexer->input[start], length);
    identifier[length] = '\0';
    
    printf("Read identifier: '%s'\n", identifier);
    return identifier;
}

static char *read_number(ArrayLexer *lexer) {
    size_t start = lexer->position;
    while (isdigit(lexer->current_char) || lexer->current_char == '.') {
        advance(lexer);
    }
    
    size_t length = lexer->position - start;
    char *number = malloc(length + 1);
    strncpy(number, &lexer->input[start], length);
    number[length] = '\0';
    
    return number;
}

static char *read_string(ArrayLexer *lexer) {
    advance(lexer); // Skip opening quote
    size_t start = lexer->position;
    
    while (lexer->current_char != '"' && lexer->current_char != '\0') {
        advance(lexer);
    }
    
    size_t length = lexer->position - start;
    char *string = malloc(length + 1);
    strncpy(string, &lexer->input[start], length);
    string[length] = '\0';
    
    if (lexer->current_char == '"') {
        advance(lexer); // Skip closing quote
    }
    
    return string;
}

static ArrayTokenType check_keyword(const char *identifier) {
    // Debug output
    printf("Checking keyword: %s\n", identifier);

    struct {
        const char *keyword;
        ArrayTokenType type;
    } keywords[] = {
        {"int", ARRAY_TOKEN_TYPE_INT},
        {"string", ARRAY_TOKEN_TYPE_STRING},
        {"float", ARRAY_TOKEN_TYPE_FLOAT},
        {"bool", ARRAY_TOKEN_TYPE_BOOL},
        {"char", ARRAY_TOKEN_TYPE_CHAR},
        {"addLast", ARRAY_TOKEN_METHOD_ADD_LAST},
        {"addFirst", ARRAY_TOKEN_METHOD_ADD_FIRST},
        {"removeLast", ARRAY_TOKEN_METHOD_REMOVE_LAST},
        {"removeFirst", ARRAY_TOKEN_METHOD_REMOVE_FIRST},
        {"length", ARRAY_TOKEN_METHOD_LENGTH},
        {"yup", ARRAY_TOKEN_BOOL},
        {"nope", ARRAY_TOKEN_BOOL},
        {"print", ARRAY_TOKEN_PRINT},
        {NULL, ARRAY_TOKEN_IDENTIFIER}
    };

    for (int i = 0; keywords[i].keyword != NULL; i++) {
        if (strcmp(identifier, keywords[i].keyword) == 0) {
            printf("Found keyword match: %s -> %d\n", identifier, keywords[i].type);
            return keywords[i].type;
        }
    }
    
    printf("No keyword match found, treating as identifier\n");
    return ARRAY_TOKEN_IDENTIFIER;
}

void array_lexer_error(ArrayLexer *lexer, const char *message) {
    fprintf(stderr, "Error at line %zu, column %zu: %s\n", 
            lexer->line, lexer->column, message);
}

const char *array_token_type_to_string(ArrayTokenType type) {
    switch (type) {
        case ARRAY_TOKEN_EOF: return "EOF";
        case ARRAY_TOKEN_IDENTIFIER: return "IDENTIFIER";
        case ARRAY_TOKEN_NUMBER: return "NUMBER";
        case ARRAY_TOKEN_STRING: return "STRING";
        case ARRAY_TOKEN_BOOL: return "BOOL";
        case ARRAY_TOKEN_CHAR: return "CHAR";
        case ARRAY_TOKEN_LBRACKET: return "LBRACKET";
        case ARRAY_TOKEN_RBRACKET: return "RBRACKET";
        case ARRAY_TOKEN_COMMA: return "COMMA";
        case ARRAY_TOKEN_DOT: return "DOT";
        case ARRAY_TOKEN_LESS_THAN: return "LESS_THAN";
        case ARRAY_TOKEN_GREATER_THAN: return "GREATER_THAN";
        case ARRAY_TOKEN_ASSIGN: return "EQUALS";
        case ARRAY_TOKEN_TYPE_INT: return "TYPE_INT";
        case ARRAY_TOKEN_TYPE_STRING: return "TYPE_STRING";
        case ARRAY_TOKEN_TYPE_FLOAT: return "TYPE_FLOAT";
        case ARRAY_TOKEN_TYPE_BOOL: return "TYPE_BOOL";
        case ARRAY_TOKEN_TYPE_CHAR: return "TYPE_CHAR";
        case ARRAY_TOKEN_METHOD_ADD_LAST: return "METHOD_ADD_LAST";
        case ARRAY_TOKEN_METHOD_ADD_FIRST: return "METHOD_ADD_FIRST";
        case ARRAY_TOKEN_METHOD_REMOVE_LAST: return "METHOD_REMOVE_LAST";
        case ARRAY_TOKEN_METHOD_REMOVE_FIRST: return "METHOD_REMOVE_FIRST";
        case ARRAY_TOKEN_METHOD_LENGTH: return "METHOD_LENGTH";
        case ARRAY_TOKEN_LPAREN: return "LPAREN";
        case ARRAY_TOKEN_RPAREN: return "RPAREN";
        case ARRAY_TOKEN_SEMICOLON: return "SEMICOLON";
        case ARRAY_TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
} 