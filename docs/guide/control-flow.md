# Control Flow in A++

Control flow structures in A++ help you control the execution path of your program. This guide covers all the control flow statements available in A++.

## Conditional Statements

### If-Else Statement

```c
if (condition) {
    // code when condition is yup
} else if (another_condition) {
    // code when another_condition is yup
} else {
    // code when all conditions are nope
}
```

## Loops

### For Loop

```c
// Basic for loop
for (int i = 0; i < 5; i++) {
    print(i);
}
```

## Best Practices

1. **Keep Conditions Simple**
```c
// Bad
if (user.age >= 18 && user.hasLicense && !user.isRestricted) {
    // code
}

// Good
boolean canDrive = user.age >= 18 
                   && user.hasLicense 
                   && !user.isRestricted;
if (canDrive) {
    // code
}
```

2. **Avoid Deep Nesting**
```c
// Bad
if (condition1) {
    if (condition2) {
        if (condition3) {
            // code
        }
    }
}

// Good
if (!condition1) return;
if (!condition2) return;
if (!condition3) return;
// code
```

## Next Steps

- Learn about [Functions](/guide/functions)
- Explore [Error Handling](/guide/error-handling)
- Study [Advanced Topics](/guide/advanced-topics)
