# Variables and Types in A++

A++ is a statically-typed language with type inference. This guide covers the variable system and built-in types.

## Basic Types

### Primitive Types

```a++
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
<!-- 
A++ can infer types when initialized:

```a++
var count = 42;          // Inferred as int
var message = "Hello";   // Inferred as string
var pi = 3.14;          // Inferred as float
``` -->

## Complex Types

### Arrays

```a++
// Dynamic arrays
array<int> dynamic_numbers = [1, 2, 3];
dynamic_numbers.push(4);  // Add element
```
<!-- 
### Objects

```a++
object person = {
    name: "Alice",
    age: 25,
    isStudent: yup
};

// Accessing object properties
print(person.name);      // Using dot notation
print(person["age"]);    // Using bracket notation
``` -->

## Type Conversion

### Implicit Conversion

A++ performs safe implicit conversions:

```a++
int x = 5;
float y = x;      // int to float is safe
```

### Explicit Conversion

Use type casting for explicit conversions:

```a++
float pi = 3.14;
int rounded = (int)pi;           // Float to int
string num = toString(42);       // Number to string
int parsed = parseInt("42");     // String to int
```

## Constants

Use `const` for immutable values:

```a++
const int MAX_USERS = 100;
const string APP_NAME = "MyApp";
const float PI = 3.14159;
```

## Variable Scope

### Block Scope

```a++
{
    int x = 5;      // Only available in this block
    print(x);
}
// x is not accessible here
```

### Function Scope

```a++
function test() {
    int y = 10;     // Only available in this function
}
// y is not accessible here
```

### Global Scope

```a++
global int counter = 0;  // Available everywhere

function increment() {
    counter++;          // Can access global
}
```

## Best Practices

1. **Use Descriptive Names**
```a++
// Bad
int x = 5;

// Good
int userAge = 5;
```

2. **Initialize Variables**
```a++
// Bad
int count;

// Good
int count = 0;
```

3. **Use Constants for Magic Numbers**
```a++
// Bad
if (users.length > 100) { ... }

// Good
const int MAX_USERS = 100;
if (users.length > MAX_USERS) { ... }
```

4. **Choose Appropriate Types**
```a++
// Bad: Using float for whole numbers
float age = 25.0;

// Good: Using int for whole numbers
int age = 25;
```

## Next Steps

- Learn about [Control Flow](/guide/control-flow)
- Explore [Functions](/guide/functions)
- Study [Error Handling](/guide/error-handling)
