# Variables and Types in A++

A++ is a statically-typed language with type inference. This guide covers the variable system and built-in types.

## Basic Types

### Primitive Types

```c
// Numbers
int age = 25;            // Integer
float price = 19.99;     // Floating-point number
double precise = 3.14159265359;  // Double precision float

// Text
char grade = 'A';        // Single character
string name = "Alice";   // Text string

// Boolean
boolean isValid = yup;   // A++ uses yup/nope instead of true/false
boolean isDone = nope;
```

### Type Inference
Not supported as yet.

## Type Conversion

### Implicit Conversion

A++ performs safe implicit conversions:

```c
int x = 5;
float y = x;      // int to float is safe
```

## Best Practices

1. **Use Descriptive Names**
```c
// Bad
int x = 5;

// Good
int userAge = 5;
```

2. **Initialize Variables**
```c
// Bad
int count;

// Good
int count = 0;
```

4. **Choose Appropriate Types**
```c
// Bad: Using float for whole numbers
float age = 25.0;

// Good: Using int for whole numbers
int age = 25;
```

## Next Steps

- Learn about [Control Flow](/guide/control-flow)
- Explore [Functions](/guide/functions)
- Study [Error Handling](/guide/error-handling)
