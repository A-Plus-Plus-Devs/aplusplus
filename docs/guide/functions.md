# Functions in A++

Functions are reusable blocks of code that perform specific tasks. A++ provides a rich set of features for working with functions.

## Function Basics

### Function Declaration

```a++
// Basic function
return_type function_name(parameter_type parameter) {
    // function body
    return value;
}

// Example
int add(int a, int b) {
    return a + b;
}
```

### Function Types

```a++
// Void function (no return value)
void greet(string name) {
    print("Hello, " + name + "!");
}

// Function with return value
int multiply(int x, int y) {
    return x * y;
}

// Function with multiple parameters
string formatName(string firstName, string lastName) {
    return firstName + " " + lastName;
}
```

## Advanced Function Features

### Default Parameters

```a++
// Parameters with default values
void configure(string host = "localhost", int port = 8080) {
    // ...
}

// Can be called as:
configure();               // Uses defaults
configure("example.com"); // Uses custom host, default port
configure("example.com", 3000); // Uses all custom values
```

### Named Parameters

```a++
void createUser(string name, int age, boolean isAdmin = nope) {
    // ...
}

// Call with named parameters
createUser(
    name: "Alice",
    age: 25,
    isAdmin: yup
);
```

### Variable Arguments

```a++
// Function that accepts variable number of arguments
int sum(...int numbers) {
    int total = 0;
    for (int n in numbers) {
        total += n;
    }
    return total;
}

// Usage
sum(1, 2, 3);       // Returns 6
sum(1, 2, 3, 4, 5); // Returns 15
```

## Function Overloading

```a++
// Same function name, different parameters
int add(int a, int b) {
    return a + b;
}

float add(float a, float b) {
    return a + b;
}

string add(string a, string b) {
    return a + b;
}
```

## Lambda Functions

```a++
// Anonymous function
var multiply = (int x, int y) => x * y;

// Using lambda in array methods
array<int> numbers = [1, 2, 3, 4, 5];
array<int> doubled = numbers.map(n => n * 2);
```

## Higher-Order Functions

```a++
// Function that takes a function as parameter
void processNumbers(array<int> numbers, function<int, int> processor) {
    for (int n in numbers) {
        print(processor(n));
    }
}

// Usage
array<int> nums = [1, 2, 3];
processNumbers(nums, n => n * n);  // Prints squares
```

## Generators

```a++
// Generator function
generator<int> range(int start, int end) {
    for (int i = start; i <= end; i++) {
        yield i;
    }
}

// Using generator
for (int num in range(1, 5)) {
    print(num);  // Prints 1, 2, 3, 4, 5
}
```

## Error Handling in Functions

```a++
// Function that might throw an error
int divide(int a, int b) throws DivisionError {
    if (b == 0) {
        throw new DivisionError("Cannot divide by zero");
    }
    return a / b;
}

// Using the function safely
try {
    int result = divide(10, 0);
} catch (DivisionError e) {
    print("Error: " + e.message);
}
```

## Best Practices

1. **Single Responsibility**
```a++
// Bad: Function does too much
void processUser(User user) {
    validateUser(user);
    saveToDatabase(user);
    sendEmail(user);
}

// Good: Separate concerns
void validateUser(User user) { ... }
void saveUser(User user) { ... }
void notifyUser(User user) { ... }
```

2. **Descriptive Names**
```a++
// Bad
void p(string s) { ... }

// Good
void printMessage(string message) { ... }
```

3. **Parameter Validation**
```a++
int calculateArea(int width, int height) {
    if (width <= 0 || height <= 0) {
        throw new ValueError("Dimensions must be positive");
    }
    return width * height;
}
```

4. **Return Early Pattern**
```a++
boolean validateUser(User user) {
    if (!user.name) return nope;
    if (!user.email) return nope;
    if (user.age < 18) return nope;
    return yup;
}
```

## Examples

### Working with Arrays
```a++
// Map function
array<string> capitalize(array<string> words) {
    return words.map(word => {
        return word[0].toUpperCase() + word.slice(1);
    });
}

// Filter function
array<int> getEvenNumbers(array<int> numbers) {
    return numbers.filter(n => n % 2 == 0);
}

// Reduce function
int calculateTotal(array<int> numbers) {
    return numbers.reduce((acc, curr) => acc + curr, 0);
}
```

### Practical Example: Data Processing
```a++
// Define data structure
struct UserData {
    string name;
    int age;
    array<string> hobbies;
}

// Process user data
array<UserData> filterAndFormatUsers(array<UserData> users) {
    return users
        .filter(user => user.age >= 18)
        .map(user => {
            return {
                name: user.name.toUpperCase(),
                age: user.age,
                hobbies: user.hobbies.sort()
            };
        });
}
```

## Next Steps

- Learn about [Classes and Objects](/guide/classes)
- Explore [Error Handling](/guide/error-handling)
- Study [Advanced Topics](/guide/advanced-topics)
