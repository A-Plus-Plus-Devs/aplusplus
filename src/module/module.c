#include "module.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_MODULES 100
#define DEBUG_ENABLED 1
#define DEBUG_LOG(fmt, ...) \
    do { if (DEBUG_ENABLED) printf("[DEBUG][%s:%d] " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); } while (0)

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
    DEBUG_LOG("Resolving path for module: %s", module_name);
    
    // Remove any quotes from the module name
    if (module_name[0] == '"' || module_name[0] == '\'') {
        module_name++; // Skip opening quote
    }
    
    // Calculate length without quotes
    size_t len = strlen(module_name);
    if (len > 0 && (module_name[len-1] == '"' || module_name[len-1] == '\'')) {
        len--;
    }

    // If the path already has .a++ extension, don't add it again
    bool has_extension = (len > 4 && strcmp(module_name + len - 4, ".a++") == 0);
    
    // Allocate space for the path
    bool add_prefix = (module_name[0] != '.' && module_name[0] != '/');
    size_t path_len = (add_prefix ? 2 : 0) + len + (has_extension ? 0 : 4) + 1;
    char *path = malloc(path_len);
    
    if (!path) {
        DEBUG_LOG("Failed to allocate memory for path");
        return NULL;
    }
    
    // Build the path
    char *p = path;
    if (add_prefix) {
        *p++ = '.';
        *p++ = '/';
    }
    
    // Copy the module name without quotes
    strncpy(p, module_name, len);
    p[len] = '\0';
    
    // Add .a++ extension if needed
    if (!has_extension) {
        strcat(path, ".a++");
    }
    
    DEBUG_LOG("Resolved path: %s", path);
    return path;
}

Module *load_module(const char *module_name) {
    DEBUG_LOG("Loading module: %s", module_name);
    
    // Check if module is already loaded
    for (int i = 0; i < module_count; i++) {
        if (loaded_modules[i] && strcmp(loaded_modules[i]->name, module_name) == 0) {
            DEBUG_LOG("Module already loaded at index %d", i);
            return loaded_modules[i];
        }
    }

    // Create new module
    Module *module = malloc(sizeof(Module));
    if (!module) {
        DEBUG_LOG("Failed to allocate memory for module");
        return NULL;
    }
    
    module->name = strdup(module_name);
    module->path = resolve_module_path(module_name);
    module->exports = NULL;
    module->is_loaded = false;

    DEBUG_LOG("Created new module: name=%s, path=%s", module->name, module->path);

    // Add to loaded modules array
    if (module_count < MAX_MODULES) {
        loaded_modules[module_count++] = module;
        DEBUG_LOG("Added module to loaded_modules at index %d", module_count - 1);
        return module;
    } else {
        DEBUG_LOG("Error: Maximum number of modules reached");
        free(module->name);
        free(module->path);
        free(module);
        return NULL;
    }
}

void register_export(Module *module, const char *name, void *value, VariableType type) {
    DEBUG_LOG("Registering export: module=%p, name=%s, value=%p, type=%d", 
              (void*)module, name, value, type);
    
    if (!module) {
        DEBUG_LOG("Error: NULL module");
        return;
    }

    // Create export node and add to module's exports list
    ASTNode *export_node = create_node(NODE_EXPORT, NULL, NULL, name);
    if (!export_node) {
        DEBUG_LOG("Error: Failed to create export node");
        return;
    }

    const char *type_str = type_to_string(type);
    export_node->var_type = strdup(type_str);
    export_node->value = strdup(name);  // Store the name instead of the value pointer
    
    DEBUG_LOG("Created export node: type=%s, name=%s, value=%p", 
              type_str, export_node->value, value);
    
    // Store the actual value in a separate field
    export_node->function = value;  // Add this field to ASTNode struct
    
    // Add to exports list
    if (!module->exports) {
        module->exports = export_node;
        DEBUG_LOG("First export in module: name=%s, value=%p", 
                 export_node->value, export_node->function);
    } else {
        ASTNode *current = module->exports;
        while (current->next) {
            current = current->next;
        }
        current->next = export_node;
        DEBUG_LOG("Added to existing exports: name=%s, value=%p", 
                 export_node->value, export_node->function);
    }
}

void *get_export(const char *module_name, const char *export_name) {
    DEBUG_LOG("Looking for export: module=%s, name=%s", module_name, export_name);
    
    // Find module
    Module *module = NULL;
    for (int i = 0; i < module_count; i++) {
        if (loaded_modules[i] && strcmp(loaded_modules[i]->name, module_name) == 0) {
            module = loaded_modules[i];
            DEBUG_LOG("Found module at index %d: %s", i, module->name);
            break;
        }
    }

    if (!module) {
        DEBUG_LOG("Module not found");
        return NULL;
    }

    DEBUG_LOG("Module exports list: %p", (void*)module->exports);

    // Find export in module
    ASTNode *current = module->exports;
    while (current) {
        DEBUG_LOG("Checking export node: name=%s, value=%s, function=%p", 
                 current->value, current->value, current->function);
        
        if (strcmp(current->value, export_name) == 0) {
            DEBUG_LOG("Found export! Name=%s, Function=%p", 
                     current->value, current->function);
            return current->function;  // Return the function pointer
        }
        current = current->next;
    }

    DEBUG_LOG("Export not found in module");
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