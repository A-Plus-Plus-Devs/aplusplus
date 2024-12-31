# Your First A++ Program

Let's write your first A++ program and understand the basics of the language.

## Creating a Hello World Program

Create a new file called `hello.a++`:

```c
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

```c
// Input/Output example
string name = "Name";
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
```

## Common Beginner Mistakes

1. Forgetting semicolons
```c
// Wrong
print("Hello")

// Correct
print("Hello");
```

2. Using wrong boolean values
```c
// Wrong
boolean flag = true;

// Correct
boolean flag = yup;  // A++ uses yup/nope
```

## Next Steps

Now that you've written your first programs:

1. Learn about [Variables and Types](/guide/variables-types)
2. Explore [Control Flow](/guide/control-flow)
3. Master [Functions](/guide/functions)
4. Try the [Examples](/examples/hello-world)
