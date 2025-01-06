# A++ Standard Library Reference

## Mathematical Functions

### Basic Math Functions
```c
// Square root
sqrt(16);       // Returns: 4
sqrt(2);        // Returns: 1.414213562

// Cube root (works with negative numbers)
cbrt(27);       // Returns: 3
cbrt(-8);       // Returns: -2

// Nth root
root(16, 2);    // Returns: 4 (square root)
root(27, 3);    // Returns: 3 (cube root)
root(16, 4);    // Returns: 2 (fourth root)
```

### Trigonometric Functions (in radians)
```c
// Basic trigonometric functions
sin(3.14159/2);  // Returns: 1
cos(0);          // Returns: 1
tan(3.14159/4);  // Returns: 1

// Inverse trigonometric functions
asin(1);        // Returns: 1.5708 (π/2)
acos(0);        // Returns: 1.5708 (π/2)
atan(1);        // Returns: 0.7854 (π/4)
```

### Number Manipulation
```c
// Absolute value
abs(-5);        // Returns: 5
abs(3.14);      // Returns: 3.14

// Rounding functions
round(3.7);     // Returns: 4
round(3.2);     // Returns: 3
round(3.5, "U"); // Returns: 4 (force round up)
round(3.5, "D"); // Returns: 3 (force round down)

// Precision control
toPrecision(3.14159, 2);  // Returns: 3.14
toPrecision(123.456789, 3); // Returns: 123.457

// Min/Max functions
min(5, 3, 8, 1, 9);  // Returns: 1
max(5, 3, 8, 1, 9);  // Returns: 9
```

## String Functions

### String Manipulation
```c
// Get index of character
indexOf("Hello", "e");  // Returns: 1
indexOf("Hello", "o");  // Returns: 4
indexOf("Hello", "z");  // Returns: -1

// Case conversion
toLowerCase("Hello WORLD");  // Returns: "hello world"
toUpperCase("Hello world");  // Returns: "HELLO WORLD"

// String trimming
trim("  Hello World  ");    // Returns: "Hello World"

// String reversal
reverse("Hello");           // Returns: "olleH"

// String repetition
repeat("Ha", 3);           // Returns: "HaHaHa"
repeat("Hi", 2, "-");      // Returns: "Hi-Hi"
```

## Date and Time Functions

### Date Functions
```c
// Current date (YYYY-MM-DD)
date();              // Returns: "2024-03-14"

// Specific date components
date("Y");          // Returns: "2024" (year)
date("M");          // Returns: "03" (month)
date("D");          // Returns: "14" (day)

// Combined components
date("Y", "M");     // Returns: "2024-03"
date("M", "D");     // Returns: "03-14"
date("Y", "D");     // Returns: "2024-14"
```

### Time Functions
```c
// Current time (HH:MM:SS)
time();             // Returns: "15:30:45"

// Current date and time
now();              // Returns: "2024-03-14 15:30:45"
``` 