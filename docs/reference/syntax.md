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

### Mathematical Functions
```c
// Root functions
sqrt(number);              // Square root
cbrt(number);             // Cube root
root(number, nth_root);   // Nth root

// Trigonometric functions (in radians)
sin(angle);               // Sine
cos(angle);               // Cosine
tan(angle);              // Tangent
asin(value);             // Arc sine
acos(value);             // Arc cosine
atan(value);             // Arc tangent

// Number manipulation
abs(number);             // Absolute value
round(number);           // Round to nearest integer
round(number, "U");      // Round up
round(number, "D");      // Round down
toPrecision(number, n);  // Round to n decimal places
min(n1, n2, ...);       // Minimum of numbers
max(n1, n2, ...);       // Maximum of numbers
```

### String Functions
```c
// String information
length(string);          // Returns number of characters
indexOf(string, char);   // Returns index of character

// String manipulation
toLowerCase(string);     // Convert to lowercase
toUpperCase(string);     // Convert to uppercase
trim(string);           // Remove surrounding whitespace
reverse(string);        // Reverse the string
repeat(string, count);  // Repeat string n times
repeat(string, count, delimiter);  // Repeat with delimiter
```

### Date and Time Functions
```c
// Date functions
date();                 // Current date (YYYY-MM-DD)
date("Y");              // Current year
date("M");              // Current month
date("D");              // Current day
date("Y", "M");         // Current year-month

// Time functions
time();                 // Current time (HH:MM:SS)
now();                  // Current date and time
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
