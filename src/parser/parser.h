// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "lexer/lexer.h"
#include "ast/ast.h"
#include "array/array_lexer.h"
#include "array/array_parser.h"
#include "array/array_ast.h"

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

/**
 * @brief Checks if the current token is an array operation.
 * 
 * @param parser A pointer to the Parser structure.
 * @return bool True if the current token is an array operation, false otherwise.
 */
bool is_array_operation(Parser *parser);

/**
 * @brief Parses an array statement and generates an Abstract Syntax Tree (AST).
 * 
 * @param parser A pointer to the Parser structure.
 * @return ASTNode* The root node of the generated AST.
 */
ASTNode *parse_array_statement(Parser *parser);

/**
 * @brief Converts an ArrayASTNode to an ASTNode.
 * 
 * @param array_node A pointer to the ArrayASTNode structure.
 * @return ASTNode* The converted ASTNode.
 */
ASTNode *convert_array_ast_to_ast(ArrayASTNode *array_node);

#endif // PARSER_H
