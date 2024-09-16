// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "lexer/lexer.h"
#include "ast/ast.h"

typedef struct {
    Lexer *lexer;
    Token *current_token;
} Parser;

/**
 * @brief Creates a new parser with the given lexer.
 * 
 * @param lexer A pointer to the Lexer structure.
 * @return Parser* A pointer to the newly created Parser structure.
 */
Parser *create_parser(Lexer *lexer);

/**
 * @brief Frees the memory allocated for the parser.
 * 
 * @param parser A pointer to the Parser structure to be freed.
 */
void free_parser(Parser *parser);

/**
 * @brief Parses the tokens and generates an Abstract Syntax Tree (AST).
 * 
 * @param parser A pointer to the Parser structure.
 * @return ASTNode* The root node of the generated AST.
 */
ASTNode *parse_tokens(Parser *parser);

#endif // PARSER_H
