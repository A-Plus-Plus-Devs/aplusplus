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

## Type Casting

A++ supports explicit type casting using the following syntax:
```c
(target_type) expression
```

### Basic Type Casting
```c
// String to Number conversions
int num1 = (int) "42";        // 42
float num2 = (float) "3.14";  // 3.14

// Number to String
string str1 = "Number: " + (string) 42;      // "Number: 42"
string str2 = "Pi: " + (string) 3.14;        // "Pi: 3.14"

// Float to Int (truncates decimal)
int num3 = (int) 3.14;        // 3

// Int to Float
float num4 = (float) 42;      // 42.0

// Boolean conversions
boolean b1 = (boolean) 1;     // yup
boolean b2 = (boolean) 0;     // nope
string str3 = (string) yup;   // "yup"
```

### Type Casting Rules

1. **Integer Casting**
   - From float: Truncates decimal portion
   - From string: Parses numeric content
   - From boolean: `yup` → 1, `nope` → 0

2. **Float Casting**
   - From int: Adds .0 for whole numbers
   - From string: Parses decimal number
   - From boolean: `yup` → 1.0, `nope` → 0.0

3. **String Casting**
   - From int: Direct conversion ("42")
   - From float: Preserves decimals ("3.14")
   - From boolean: "yup" or "nope"

4. **Boolean Casting**
   - From int: 0 → `nope`, non-zero → `yup`
   - From string: Empty → `nope`, non-empty → `yup`
   - From float: 0.0 → `nope`, non-zero → `yup`

### Examples
```c
// Input conversion
float age = (float) input("Enter your age: ");  // "20" → 20.0
float ageIn5Years = age * 5;
print("In 5 years you will be: " + ageIn5Years);  // "25.0"

int x = (int) 42.9;  // 42 (truncates decimal)
boolean isValid = (boolean) 1;  // yup
```

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