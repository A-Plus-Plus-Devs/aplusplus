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

// Arrays (New!)
numbers<int> = [1, 2, 3];         // Integer array
names<string> = ["John", "Jane"]; // String array
mixed<any> = [1, "two", yup];    // Mixed type array
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
float ageIn5Years = age + 5;
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

### Built-in Functions
- `print`: Output to console
- `yap`: Alternative output
- `echo`: Alternative output
- `input`: Get user input
- `length`: Get string length
- `charAt`: Get character at string index

## Built-in Functions

### Mathematical Operations
```c
// Root calculations
float root = sqrt(16);          // 4.0
float cube = cbrt(-8);         // -2.0
float fourth = root(16, 4);    // 2.0

// Trigonometry
float sine = sin(3.14159/2);   // 1.0
float cosine = cos(0);         // 1.0
float tangent = tan(0.785398); // 1.0

// Number manipulation
float absolute = abs(-5.5);    // 5.5
int rounded = round(3.7);      // 4
float precise = toPrecision(3.14159, 2);  // 3.14
int minimum = min(5, 3, 8, 1); // 1
int maximum = max(5, 3, 8, 1); // 8
```

### String Operations
```c
// String information
int idx = indexOf("Hello", "e");     // 1
int len = length("Hello");           // 5

// String manipulation
string lower = toLowerCase("Hello");  // "hello"
string upper = toUpperCase("hello"); // "HELLO"
string clean = trim("  hello  ");    // "hello"
string rev = reverse("hello");       // "olleh"
string rep = repeat("ha", 3);        // "hahaha"
string repd = repeat("hi", 2, "-");  // "hi-hi"
```

### Date and Time Operations
```c
// Date handling
string today = date();              // "2024-03-14"
string year = date("Y");           // "2024"
string month = date("M");          // "03"
string day = date("D");            // "14"

// Time handling
string current = time();           // "15:30:45"
string datetime = now();           // "2024-03-14 15:30:45"
```

### Type Conversion Rules

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
// Mathematical operations
float root = sqrt(25);                    // 5.0
float value = toPrecision(3.14159, 2);    // 3.14

// String manipulation
string text = "  Hello, World!  ";
print(trim(text));                        // "Hello, World!"
print(toLowerCase(text));                 // "  hello, world!  "
print(repeat("Hi", 3, " "));             // "Hi Hi Hi"

// Date and time
print(date("Y") + "-" + date("M"));      // "2024-03"
print(now());                            // "2024-03-14 15:30:45"
```

## String Operations

### String Functions
```c
// Get string length
string name = "Paul";
print(length(name));    // Output: 4

// Get character at index
print(charAt(name, 0));  // Output: "P"
print(charAt(name, 3));  // Output: "l"

// Get part of string from start to end index
print(substring("hello", 1, 3));  // Output: "el"

// Combine two strings into one
print(concat("he", "llo"));  // Output: "hello"

// Replace character at index with new char
print(replace("hello", 0, "j"));  // Output: "jello"
```

### String Function Rules
1. **length(string)**
   - Returns the number of characters in a string
   - Returns 0 for empty strings
   - Works with both string literals and variables

2. **charAt(string, int)**
   - Returns the character at the specified index as a string
   - Index starts at 0
   - Returns empty string if index is out of bounds
   - Works with both string literals and variables

3. **substring(string, int, int)**
   - Returns a portion of the string from start index (inclusive) to end index (exclusive)
   - Returns empty string if indices are invalid
   - Example: substring("hello", 1, 3) returns "el"

4. **concat(string, string)**
   - Combines two strings into one
   - Alternative to + operator for strings
   - Example: concat("he", "llo") returns "hello"

5. **replace(string, int, char)**
   - Returns a new string with the character at specified index replaced
   - Returns original string if index is invalid
   - Example: replace("hello", 0, "j") returns "jello"

### Examples
```c
string message = "Hello, World!";
print(length(message));     // Output: 13
print(charAt(message, 0));   // Output: "H"
print(charAt(message, 7));   // Output: "W"

// Using with string literals
print(length("A++"));       // Output: 3
print(charAt("A++", 2));     // Output: "+"

// Error handling
print(charAt(message, 50));  // Output: "" (empty string, index out of bounds)
```

## Arrays

### Array Declaration
Arrays in A++ are declared using angle brackets for type specification:
```c
names<string> = ["Alice", "Bob"];    // String array
scores<int> = [95, 87, 92];         // Integer array
mixed<any> = [1, "two", yup];       // Mixed type array
```

### Array Operations
Arrays support several built-in methods:
```c
// Adding elements
numbers.addLast(42);     // Add to end
numbers.addFirst(10);    // Add to start
numbers.insert(1, 25);   // Insert at index 1

// Removing elements
numbers.removeLast();    // Remove from end
numbers.removeFirst();   // Remove from start

// Getting array information
int len = length(numbers);   // Get array length
```

### Array Access
```c
// Reading elements
int first = numbers[0];      // Get first element
int last = numbers[length(numbers) - 1];  // Get last element

// Updating elements
int numbers[1] = 50;        // Update second element
``` 