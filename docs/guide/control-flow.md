# Control Flow in A++

Control flow structures in A++ help you control the execution path of your program. This guide covers all the control flow statements available in A++.

## Conditional Statements

### If-Else Statement

```cpp
if (condition) {
    // code when condition is yup
} else if (another_condition) {
    // code when another_condition is yup
} else {
    // code when all conditions are nope
}
```

### Switch Statement

```cpp
switch (value) {
    case 1:
        print("One");
        break;
    case 2:
        print("Two");
        break;
    default:
        print("Other");
}
```

## Loops

### For Loop

```cpp
// Basic for loop
for (int i = 0; i < 5; i++) {
    print(i);
}

// For-each loop with arrays
array<string> names = ["Alice", "Bob", "Charlie"];
for (string name in names) {
    print(name);
}
```

### While Loop

```cpp
// While loop
while (condition) {
    // code
}

// Do-while loop
do {
    // code
} while (condition);
```

## Control Statements

### Break

Exit a loop early:

```cpp
for (int i = 0; i < 10; i++) {
    if (i == 5) {
        break;  // Exit loop when i is 5
    }
    print(i);
}
```

### Continue

Skip to the next iteration:

```cpp
for (int i = 0; i < 5; i++) {
    if (i == 2) {
        continue;  // Skip printing 2
    }
    print(i);
}
```

### Return

Exit a function and optionally return a value:

```cpp
int findMax(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}
```

## Pattern Matching

A++ supports modern pattern matching:

```cpp
match (value) {
    case 1 -> print("One");
    case 2 -> print("Two");
    case _ -> print("Other");
}

// Pattern matching with types
match (obj) {
    case string s -> print("String: " + s);
    case int n -> print("Number: " + n);
    case _ -> print("Unknown type");
}
```

## Error Handling

### Try-Catch Blocks

```cpp
try {
    // Code that might throw an error
    riskyFunction();
} catch (error) {
    // Handle the error
    print("An error occurred: " + error.message);
} finally {
    // Always executed
    cleanup();
}
```

## Best Practices

1. **Keep Conditions Simple**
```cpp
// Bad
if (user.age >= 18 && user.hasLicense && !user.isRestricted) {
    // code
}

// Good
boolean canDrive = user.age >= 18 
                   && user.hasLicense 
                   && !user.isRestricted;
if (canDrive) {
    // code
}
```

2. **Avoid Deep Nesting**
```cpp
// Bad
if (condition1) {
    if (condition2) {
        if (condition3) {
            // code
        }
    }
}

// Good
if (!condition1) return;
if (!condition2) return;
if (!condition3) return;
// code
```

3. **Use Switch for Multiple Conditions**
```cpp
// Bad
if (value == 1) {
    // code
} else if (value == 2) {
    // code
} else if (value == 3) {
    // code
}

// Good
switch (value) {
    case 1: // code; break;
    case 2: // code; break;
    case 3: // code; break;
}
```

## Examples

### Menu System
```cpp
boolean running = yup;
while (running) {
    print("1. Start Game");
    print("2. Settings");
    print("3. Exit");
    
    int choice = parseInt(input("Choose option: "));
    
    switch (choice) {
        case 1:
            startGame();
            break;
        case 2:
            showSettings();
            break;
        case 3:
            running = nope;
            break;
        default:
            print("Invalid option");
    }
}
```

### Data Processing
```cpp
array<int> numbers = [1, 2, 3, 4, 5];
array<int> evenNumbers = [];

for (int num in numbers) {
    if (num % 2 == 0) {
        evenNumbers.push(num);
    }
}
```

## Next Steps

- Learn about [Functions](/guide/functions)
- Explore [Error Handling](/guide/error-handling)
- Study [Advanced Topics](/guide/advanced-topics)
