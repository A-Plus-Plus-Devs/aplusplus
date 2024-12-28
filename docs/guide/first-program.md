# Your First A++ Program

Let's write your first A++ program and understand the basics of the language.

## Creating a Hello World Program

Create a new file called `hello.a++`:

```cpp
// This is your first A++ program
print("Hello, World!");
```

Run the program:
```bash
a++ hello.a++
```

## Understanding the Program

Let's break down what's happening:

1. `//` starts a single-line comment
2. `print()` is a built-in function for output
3. `"Hello, World!"` is a string literal

## A More Complex Example

Let's write a program that demonstrates more features:

```cpp
// Input/Output example
string name = input("What's your name? ");
print("Hello, " + name + "!");

// Variables and arithmetic
int age = 25;
float height = 1.75;

// Conditional statements
if (age >= 18) {
    print("You are an adult!");
} else {
    print("You are a minor!");
}

// Functions
int square(int x) {
    return x * x;
}

// Using our function
int result = square(5);
print("5 squared is: " + result);

// Loops
for (int i = 1; i <= 5; i++) {
    print("Count: " + i);
}
```

## Running Programs

### Command Line
```bash
# Basic execution
a++ yourprogram.a++

# With debug information
a++ --debug yourprogram.a++

# With optimization
a++ --optimize yourprogram.a++
```

### IDE Integration
If you're using VS Code with the A++ extension:
1. Open your `.a++` file
2. Press F5 to run
3. Use the integrated terminal for input/output

## Common Beginner Mistakes

1. Forgetting semicolons
```cpp
// Wrong
print("Hello")

// Correct
print("Hello");
```

2. Using wrong boolean values
```cpp
// Wrong
boolean flag = true;

// Correct
boolean flag = yup;  // A++ uses yup/nope
```

3. Incorrect string concatenation
```cpp
// Wrong
print(5 + " is a number");

// Correct
print(toString(5) + " is a number");
```

## Next Steps

Now that you've written your first programs:

1. Learn about [Variables and Types](/guide/variables-types)
2. Explore [Control Flow](/guide/control-flow)
3. Master [Functions](/guide/functions)
4. Try the [Examples](/examples/hello-world)
