#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast/ast.h"

/**
 * @brief Interprets and executes the given Abstract Syntax Tree.
 * 
 * This function walks through the AST, executing each node according to its type.
 * It handles variable declarations, assignments, and print statements.
 * 
 * @param node The root node of the AST to be interpreted.
 */
void interpret(ASTNode *node);

#endif // INTERPRETER_H
