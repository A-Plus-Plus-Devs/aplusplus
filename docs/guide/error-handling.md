# Error Handling in A++

Error handling is crucial for writing robust and reliable programs. A++ provides a comprehensive error handling system.

## Try-Catch Blocks

### Basic Error Handling

```cpp
try {
    // Code that might throw an error
    riskyOperation();
} catch (error) {
    // Handle the error
    print("An error occurred: " + error.message);
}
```

### Multiple Catch Blocks

```cpp
try {
    connectToDatabase();
} catch (ConnectionError e) {
    print("Connection failed: " + e.message);
} catch (AuthenticationError e) {
    print("Authentication failed: " + e.message);
} catch (error) {
    print("Unknown error: " + e.message);
}
```

### Finally Block

```cpp
file resource = null;
try {
    resource = openFile("data.txt");
    processFile(resource);
} catch (error) {
    print("Error processing file: " + error.message);
} finally {
    if (resource != null) {
        resource.close();
    }
}
```

## Custom Errors

### Creating Custom Error Types

```cpp
// Define custom error
class ValidationError extends Error {
    constructor(message) {
        super(message);
        this.name = "ValidationError";
    }
}

// Using custom error
void validateAge(int age) {
    if (age < 0) {
        throw new ValidationError("Age cannot be negative");
    }
    if (age > 150) {
        throw new ValidationError("Age seems unrealistic");
    }
}
```

### Error Hierarchy

```cpp
// Base application error
class AppError extends Error { }

// Specific error types
class DatabaseError extends AppError { }
class NetworkError extends AppError { }
class ValidationError extends AppError { }
```

## Error Propagation

### Throwing Errors

```cpp
int divide(int a, int b) throws DivisionError {
    if (b == 0) {
        throw new DivisionError("Cannot divide by zero");
    }
    return a / b;
}
```

### Rethrowing Errors

```cpp
try {
    processData();
} catch (error) {
    // Log the error
    logError(error);
    // Rethrow for upper layers to handle
    throw error;
}
```

## Best Practices

### 1. Be Specific with Error Types

```cpp
// Bad: Generic error
throw new Error("Something went wrong");

// Good: Specific error
throw new FileNotFoundError("Config file missing: config.json");
```

### 2. Provide Meaningful Error Messages

```cpp
// Bad
throw new Error("Invalid input");

// Good
throw new ValidationError("Username must be between 3 and 20 characters");
```

### 3. Clean Up Resources

```cpp
// Using try-finally for cleanup
database db = null;
try {
    db = openConnection();
    // Use database
} finally {
    if (db != null) {
        db.close();
    }
}
```

### 4. Don't Catch and Ignore

```cpp
// Bad: Swallowing errors
try {
    riskyOperation();
} catch (error) {
    // Don't do this!
}

// Good: Handle errors appropriately
try {
    riskyOperation();
} catch (error) {
    logError(error);
    notifyUser("Operation failed: " + error.message);
}
```

## Advanced Error Handling

### Async Error Handling

```cpp
async void fetchData() {
    try {
        var response = await api.getData();
        processResponse(response);
    } catch (NetworkError e) {
        handleNetworkError(e);
    } catch (error) {
        handleGenericError(error);
    }
}
```

### Error Chaining

```cpp
try {
    processData();
} catch (error) {
    throw new ProcessingError("Failed to process data", error);
}
```

### Custom Error Properties

```cpp
class ApiError extends Error {
    int statusCode;
    string endpoint;
    
    constructor(string message, int code, string endpoint) {
        super(message);
        this.statusCode = code;
        this.endpoint = endpoint;
    }
}

// Usage
throw new ApiError(
    "Failed to fetch user data",
    404,
    "/api/users"
);
```

## Practical Examples

### Form Validation

```cpp
class FormValidator {
    void validateUser(User user) throws ValidationError {
        if (user.name.length < 2) {
            throw new ValidationError("Name is too short");
        }
        
        if (!isValidEmail(user.email)) {
            throw new ValidationError("Invalid email format");
        }
        
        if (user.age < 18) {
            throw new ValidationError("Must be 18 or older");
        }
    }
    
    void processUserForm(User user) {
        try {
            validateUser(user);
            saveUser(user);
        } catch (ValidationError e) {
            displayError(e.message);
        } catch (DatabaseError e) {
            logError(e);
            displayError("Failed to save user");
        }
    }
}
```

### File Processing

```cpp
class FileProcessor {
    void processFile(string path) {
        file file = null;
        try {
            file = openFile(path);
            string content = file.readAll();
            processContent(content);
        } catch (FileNotFoundError e) {
            print("File not found: " + path);
        } catch (IOException e) {
            print("Error reading file: " + e.message);
        } finally {
            if (file != null) {
                file.close();
            }
        }
    }
}
```

## Next Steps

- Learn about [Testing](/guide/testing)
- Explore [Debugging](/guide/debugging)
- Study [Advanced Topics](/guide/advanced-topics)
