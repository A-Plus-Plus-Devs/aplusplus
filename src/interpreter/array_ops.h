#ifndef ARRAY_OPS_H
#define ARRAY_OPS_H

#include "../ast/ast.h"
#include "array.h"
#include "../common/types.h"
#include "interpreter.h"

// Handle array operations in the interpreter
void* interpret_array_declaration(ASTNode* node);
void* interpret_array_method_call(ASTNode* node, ArrayValue* array);
void* interpret_array_access(ASTNode* node, ArrayValue* array);
void* interpret_array_literal(ASTNode* node);

#endif // ARRAY_OPS_H 