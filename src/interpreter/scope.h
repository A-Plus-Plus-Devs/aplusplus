#ifndef SCOPE_H
#define SCOPE_H

typedef enum {
    GLOBAL_SCOPE,
    LOCAL_SCOPE,
    MODULE_SCOPE
} ScopeType;

// Function declarations
void init_scope(void);
void enter_scope(ScopeType type);
void exit_scope(void);
ScopeType current_scope(void);

#endif // SCOPE_H 