# Array Implementation in A++

## Overview
This document describes the implementation of dynamic arrays in A++.

## Array Features
- Dynamic sizing
- Zero-based indexing
- Type-safe arrays (string[], int[], etc.)
- Mixed-type arrays (any[])
- Array operations:
  - `.addLast()`: Add element to end
  - `.removeLast()`: Remove last element
  - `.length`: Get array length
  - `.addFirst()`: Add element at start
  - `.removeFirst()`: Remove first element

## Implementation Details

### Array Types
Arrays are implemented as a new variable type with the following characteristics:
- Dynamic allocation
- Type checking for typed arrays
- Flexible sizing for any[] arrays

### Array Operations
Each array operation is implemented as a method call:
- `.length`: O(1) operation
- `.addLast()`: O(1) amortized
- `.removeLast()`: O(1)
- `.addFirst()`: O(n)
- `.removeFirst()`: O(n)

### Memory Management
Arrays use dynamic memory allocation with automatic resizing when needed.

## Examples
```
names<string> = ["John", "Jane"];  // String array
numbers<int> = [1, 2, 3];         // Integer array
mixed<any> = [1, "two", true];    // Mixed type array

// Array operations
names.addLast("Bob");             // ["John", "Jane", "Bob"]
names.removeFirst();              // ["Jane", "Bob"]
len = names.length;               // 2
```
