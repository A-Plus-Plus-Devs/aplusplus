#ifndef MODULE_H
#define MODULE_H

#include "../ast/ast.h"
#include <stdbool.h>

typedef struct {
    char *name;           // Module name
    char *path;          // Full path to module file
    ASTNode *exports;    // List of exported items
    bool is_loaded;      // Track if module is already loaded
} Module;

// Initialize module system
void init_module_system(void);

// Load and process a module
Module *load_module(const char *module_name);

// Register an exported item from a module
void register_export(Module *module, const char *name, void *value, VariableType type);

// Get an exported item from a module
void *get_export(const char *module_name, const char *export_name);

// Free module resources
void free_module(Module *module);
void cleanup_module_system(void);

#endif // MODULE_H 