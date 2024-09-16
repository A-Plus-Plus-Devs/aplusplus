// #include "codegen.h"
// #include <stdio.h>

// void generate_code(ASTNode *node)
// {
//     if (!node)
//         return;

//     switch (node->type)
//     {
//     case NODE_VAR_DECLARATION:
//         printf("%s %s = ", node->var_type, node->var_name);
//         generate_code(node->left);
//         printf(";\n");
//         break;

//     case NODE_ASSIGNMENT:
//         printf("%s = ", node->var_name);
//         generate_code(node->left);
//         printf(";\n");
//         break;

//     case NODE_BINARY_OP:
//         generate_code(node->left);
//         printf(" %s ", node->value);
//         generate_code(node->right);
//         break;

//     case NODE_INT_LITERAL:
//         printf("%d", node->int_value);
//         break;

//     case NODE_FLOAT_LITERAL:
//         printf("%f", node->float_value);
//         break;

//     case NODE_LITERAL:
//     case AST_NODE_LITERAL:
//         printf("%s", node->value);
//         break;

//     case NODE_PRINT:
//         printf("printf(\"%%d\\n\", ");
//         generate_code(node->left);
//         printf(");\n");
//         break;

//     case NODE_EXPRESSION:
//         generate_code(node->left);
//         break;

//     case NODE_PROGRAM:
//         // Handle program node if needed
//         break;

//     case NODE_NUMBER:
//         // This case might be redundant if you're using INT_LITERAL and FLOAT_LITERAL
//         printf("%s", node->value);
//         break;

//     default:
//         printf("Unknown AST node type: %d\n", node->type);
//         break;
//     }

//     if (node->next)
//     {
//         generate_code(node->next);
//     }
// }
