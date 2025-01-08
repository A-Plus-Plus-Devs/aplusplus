#include "module.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_MODULES 100

static Module *loaded_modules[MAX_MODULES];
static int module_count = 0;

// Helper function to convert VariableType to string
static const char* type_to_string(VariableType type) {
    switch (type) {
        case INT_TYPE:
            return "int";
        case FLOAT_TYPE:
            return "float";
        case STRING_TYPE:
            return "string";
        case BOOL_TYPE:
            return "bool";
        case CHAR_TYPE:
            return "char";
        case NOTHING_TYPE:
            return "nothing";
        case EMPTY_TYPE:
            return "empty";
        case ARRAY_TYPE:
            return "array";
        default:
            return "unknown";
    }
}

void init_module_system(void) {
    module_count = 0;
    memset(loaded_modules, 0, sizeof(loaded_modules));
}

static char *resolve_module_path(const char *module_name) {
    // Implement path resolution logic here
    // For example: convert "math/utils" to "./math/utils.a++"
    char *path = malloc(strlen(module_name) + 6); // +6 for "./" ".a++" and null terminator
    sprintf(path, "./%s.a++", module_name);
    return path;
}

Module *load_module(const char *module_name) {
    // Check if module is already loaded
    for (int i = 0; i < module_count; i++) {
        if (loaded_modules[i] && strcmp(loaded_modules[i]->name, module_name) == 0) {
            return loaded_modules[i];
        }
    }

    // Create new module
    Module *module = malloc(sizeof(Module));
    if (!module) {
        return NULL;
    }
    
    module->name = strdup(module_name);
    module->path = resolve_module_path(module_name);
    module->exports = NULL;
    module->is_loaded = false;

    // Add to loaded modules array
    if (module_count < MAX_MODULES) {
        loaded_modules[module_count++] = module;
        return module;
    } else {
        free(module->name);
        free(module->path);
        free(module);
        return NULL;
    }
}

void register_export(Module *module, const char *name, void *value, VariableType type) {
    if (!module) return;

    // Create export node and add to module's exports list
    ASTNode *export_node = create_node(NODE_EXPORT, NULL, NULL, name);
    if (!export_node) return;

    const char *type_str = type_to_string(type);
    export_node->var_type = strdup(type_str);
    export_node->value = value;
    
    // Add to exports list
    if (!module->exports) {
        module->exports = export_node;
    } else {
        ASTNode *current = module->exports;
        while (current->next) {
            current = current->next;
        }
        current->next = export_node;
    }
}

void *get_export(const char *module_name, const char *export_name) {
    // Find module
    Module *module = NULL;
    for (int i = 0; i < module_count; i++) {
        if (loaded_modules[i] && strcmp(loaded_modules[i]->name, module_name) == 0) {
            module = loaded_modules[i];
            break;
        }
    }

    if (!module) return NULL;

    // Find export in module
    ASTNode *current = module->exports;
    while (current) {
        if (strcmp(current->value, export_name) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

void free_module(Module *module) {
    if (!module) return;
    
    if (module->name) free(module->name);
    if (module->path) free(module->path);
    if (module->exports) free_ast(module->exports);
    free(module);
}

void cleanup_module_system(void) {
    for (int i = 0; i < module_count; i++) {
        if (loaded_modules[i]) {
            free_module(loaded_modules[i]);
            loaded_modules[i] = NULL;
        }
    }
    module_count = 0;
} 