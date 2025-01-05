# A++ Syntax Reference

This page provides a comprehensive reference for A++ syntax.

## Basic Syntax

### Comments
```c
// Single line comment
/* Multi-line
   comment */
```

### Variables
```c
int number = 42;
float pi = 3.14159;
string message = "Hello";
boolean flag = yup;  // A++ uses yup/nope for booleans
```

### Operators

#### Arithmetic Operators
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division
- `%` Modulus
- `**` Power

#### Comparison Operators
- `==` Equal to
- `!=` Not equal to
- `>` Greater than
- `<` Less than
- `>=` Greater than or equal to
- `<=` Less than or equal to

#### Logical Operators
- `&&` Logical AND
- `||` Logical OR
- `!` Logical NOT

## Control Structures

### If Statement
```c
if (condition) {
    // code
} else if (another_condition) {
    // code
} else {
    // code
}
```

### Loops
```c
// For loop
for (int i = 0; i < 10; i++) {
    // code
}
```

## Built-in Functions

### String Functions
```c
// String length
length(string);     // Returns number of characters

// String indexing
charAt(string, int); // Returns character at index
```

### Function Call Syntax
```c
// Basic function calls
length("Hello");      // 5
charAt("Hello", 1);    // "e"

// With variables
string text = "Hello";
int position = 1;
charAt(text, position);  // "e"
```
