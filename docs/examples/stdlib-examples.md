# Standard Library Examples

## Mathematical Operations

### Basic Math Examples
```c
#define nothing main() {
    // Square and cube roots
    print("Square root of 16: " + sqrt(16));
    print("Cube root of 27: " + cbrt(27));
    print("Fourth root of 16: " + root(16, 4));

    // Trigonometry
    print("Sin(π/2): " + sin(3.14159/2));
    print("Cos(0): " + cos(0));

    // Number manipulation
    print("Absolute value of -5: " + abs(-5));
    print("3.14159 to 2 decimals: " + toPrecision(3.14159, 2));
    print("Minimum of 5,3,8,1,9: " + min(5,3,8,1,9));
}
```

## String Manipulation

### String Processing Examples
```c
#define nothing main() {
    string text = "  Hello, World!  ";
    
    // Basic string operations
    print("Original: '" + text + "'");
    print("Trimmed: '" + trim(text) + "'");
    print("Lowercase: " + toLowerCase(text));
    print("Uppercase: " + toUpperCase(text));
    print("Reversed: " + reverse(text));
    
    // String searching
    print("Index of 'o': " + indexOf(text, "o"));
    
    // String repetition
    print("Repeated: " + repeat("Ha", 3));
    print("Repeated with delimiter: " + repeat("Hi", 2, "-"));
}
```

## Date and Time

### Time Operations Example
```c
#define nothing main() {
    // Current date and time
    print("Current date: " + date());
    print("Current time: " + time());
    print("Current datetime: " + now());
    
    // Date components
    print("Year: " + date("Y"));
    print("Month: " + date("M"));
    print("Day: " + date("D"));
    
    // Combined components
    print("Year and month: " + date("Y", "M"));
}
```

## Running the Examples

Save any of these examples with a `.a++` extension and run using the A++ compiler:

```bash
a++ example.a++
``` 