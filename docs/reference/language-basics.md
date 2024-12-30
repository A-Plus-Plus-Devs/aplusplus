# Language Basics

## Variables and Types

A++ supports several basic data types:

```a++
// Numbers
num x = 42;          // Integer
float pi = 3.14;     // Floating-point number

// Text
text greeting = "Hello, World!";  // String

// Booleans
bool isValid = yup;   // Boolean (yup/nope)
```

### Type Rules
- Strong typing: Variables must be declared with their type
- Type inference is not supported
- No implicit type conversion

## Operators

### Arithmetic Operators
```a++
num a = 10;
num b = 3;

print(a + b);    // Addition: 13
print(a - b);    // Subtraction: 7
print(a * b);    // Multiplication: 30
print(a / b);    // Division: 3
print(a % b);    // Modulus: 1
```

### Comparison Operators
```a++
print(5 == 5);   // Equal to: yup
print(5 != 10);  // Not equal to: yup
print(5 > 3);    // Greater than: yup
print(5 < 3);    // Less than: nope
print(5 >= 5);   // Greater than or equal: yup
print(5 <= 4);   // Less than or equal: nope
```

### Logical Operators
```a++
bool a = yup;
bool b = nope;

print(a && b);   // Logical AND: nope
print(a || b);   // Logical OR: yup
print(!a);       // Logical NOT: nope
```

## Keywords

A++ has several reserved keywords:

### Type Keywords
- `num`: Integer numbers
- `float`: Floating-point numbers
- `text`: Text strings
- `bool`: Boolean values

### Control Flow Keywords
- `if`: Conditional statements
- `else`: Alternative conditional branch
- `for`: Loop construct
- `while`: While loop
- `break`: Exit a loop
- `continue`: Skip to next iteration

### Boolean Values
- `yup`: True value
- `nope`: False value

### Other Keywords
- `print`: Output to console
- `yap`: Alternative output
- `echo`: Alternative output
- `return`: Return from function

## Built-in Functions

### Output Functions
```a++
print("Hello");     // Standard output
yap("Hello");       // Alternative output
echo("Hello");      // Alternative output
```

### Type Conversion
```a++
num x = 42;
text str = toString(x);    // Convert to string
num n = toNumber("42");    // Convert to number
```

### Math Functions
```a++
num max = max(5, 10);      // Returns 10
num min = min(5, 10);      // Returns 5
num abs = abs(-5);         // Returns 5
```

### String Functions
```a++
text str = "Hello";
num len = length(str);     // Returns 5
text upper = toUpper(str); // Returns "HELLO"
text lower = toLower(str); // Returns "hello"
```
