# Language Basics

## Variables and Types

A++ supports several basic data types:

```c
// Numbers
int x = 42;          // Integer
float pi = 3.14;     // Floating-point number

// Text
string greeting = "Hello, World!";  // String

// Booleans
boolean isValid = yup;   // Boolean (yup/nope)
```

### Type Rules
- Strong typing: Variables must be declared with their type
- Type inference is not supported
- No implicit type conversion

## Operators

### Arithmetic Operators
```c
int a = 10;
int b = 3;

print(a + b);    // Addition: 13
print(a - b);    // Subtraction: 7
print(a * b);    // Multiplication: 30
print(a / b);    // Division: 3
print(a % b);    // Modulus: 1
```

### Comparison Operators
```c
print(5 == 5);   // Equal to: yup
print(5 != 10);  // Not equal to: yup
print(5 > 3);    // Greater than: yup
print(5 < 3);    // Less than: nope
print(5 >= 5);   // Greater than or equal: yup
print(5 <= 4);   // Less than or equal: nope
```

### Logical Operators
```c
boolean a = yup;
boolean b = nope;

print(a && b);   // Logical AND: nope
print(a || b);   // Logical OR: yup
print(!a);       // Logical NOT: nope
```

## Keywords

A++ has several reserved keywords:

### Type Keywords
- `int`: Integer numbers
- `float`: Floating-point numbers
- `string`: Text strings
- `boolean`: Boolean values
- `char`: Character

### Control Flow Keywords
- `if`: Conditional statements
- `else`: Alternative conditional branch
- `for`: Loop construct
- `#define`: Function definition

### Boolean Values
- `yup`: True value
- `nope`: False value

### Other Keywords
- `print`: Output to console
- `yap`: Alternative output
- `echo`: Alternative output
- `input`: Get user input

## Built-in Functions

### Output Functions
```c
print("Hello");     // Standard output
yap("Hello");       // Alternative output
echo("Hello");      // Alternative output
```

### Input Function

```c
string name = input("What is your name?"); // Standard input

print("Hello, " + name);
``` 