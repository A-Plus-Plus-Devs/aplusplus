# Semicolon Handling in A++ Compiler

## Current Behavior

The current implementation has inconsistent semicolon handling across different statement types:

1. In `parse_statement`, semicolons are:
   - Optional for print statements (consumed if present)
   - Optional for assignments (consumed if present)
   - Not explicitly required for other statement types

2. In `parse_print`, no semicolon handling is done within the function itself. Instead, it relies on the parent `parse_statement` function to handle semicolons.

## Issue

This inconsistent semicolon handling leads to:
1. Some statements working without semicolons while others require them
2. Potential ambiguity in statement boundaries
3. Inconsistent syntax requirements that may confuse users

## Proposed Solution

To maintain consistency, we should:

1. Make semicolons mandatory for all statements
2. Move semicolon handling to individual statement parsers
3. Return error if semicolon is missing after any statement

### Implementation Details

1. Modify `parse_statement` to:
   - Remove the current semicolon handling logic
   - Let individual statement parsers handle their own semicolons

2. Update statement parsers (print, assignment, etc.) to:
   - Expect and consume semicolons
   - Return error if semicolon is missing

3. Add clear error messages for missing semicolons

## Questions/Assumptions

1. Should we maintain backward compatibility with code that doesn't use semicolons?
2. Should we add a strict mode where semicolons are mandatory?
3. Do we need to handle automatic semicolon insertion (like JavaScript)?

## Next Steps

1. Implement consistent semicolon handling in all statement parsers
2. Add proper error reporting for missing semicolons
3. Update documentation to reflect the new semicolon requirements
4. Add test cases to verify semicolon handling
