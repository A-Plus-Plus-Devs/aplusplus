# A++ Syntax Reference

This page provides a comprehensive reference for A++ syntax.

## Basic Syntax

### Comments
```a++
// Single line comment
/* Multi-line
   comment */
```

### Variables
```a++
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

#### Bitwise Operators
- `&` Bitwise AND
- `|` Bitwise OR
- `^` Bitwise XOR
- `~` Bitwise NOT
- `<<` Left shift
- `>>` Right shift

## Control Structures

### If Statement
```a++
if (condition) {
    // code
} else if (another_condition) {
    // code
} else {
    // code
}
```

### Loops
```a++
// For loop
for (int i = 0; i < 10; i++) {
    // code
}
```

## Functions
```a++
// Function declaration
return_type function_name(parameter_type parameter) {
    // function body
    return value;
}

// Example function
int add(int a, int b) {
    return a + b;
}
```

## Error Handling
```a++
try {
    // code that might throw an error
} catch (error) {
    // handle error
}
```
