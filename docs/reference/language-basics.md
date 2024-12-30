# Language Basics

## Variables and Types

A++ supports several basic data types:

```a++
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
```a++
int a = 10;
int b = 3;

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

### Control Flow Keywords
- `if`: Conditional statements
- `else`: Alternative conditional branch
- `for`: Loop construct
<!-- - `while`: While loop
- `break`: Exit a loop
- `continue`: Skip to next iteration -->

### Boolean Values
- `yup`: True value
- `nope`: False value

### Other Keywords
- `print`: Output to console
- `yap`: Alternative output
- `echo`: Alternative output
<!-- - `return`: Return from function -->

## Built-in Functions

### Output Functions
```a++
print("Hello");     // Standard output
yap("Hello");       // Alternative output
echo("Hello");      // Alternative output
```

### Type Conversion
```a++
int x = 42;
string str = toString(x);    // Convert to string
int n = toNumber("42");    // Convert to number
```

### Math Functions
```a++
int max = max(5, 10);      // Returns 10
int min = min(5, 10);      // Returns 5
int abs = abs(-5);         // Returns 5
```

### String Functions
```a++
string str = "Hello";
int len = length(str);     // Returns 5
string upper = toUpper(str); // Returns "HELLO"
string lower = toLower(str); // Returns "hello"
```
