#include <stdio.h>  // This line includes the standard input/output library
#include <stdlib.h> // This line includes the standard library for functions like malloc and free
#include <string.h> // This line includes the string manipulation library
#include "lexer/lexer.h"           // This includes our custom lexer code
#include "parser/parser.h"         // This includes our custom parser code
#include "codegen/codegen.h"       // This includes our custom code generation code
#include "interpreter/interpreter.h" // This includes our custom interpreter code

/**
 * @brief Prints the usage instructions for the A++ compiler.
 *
 * This function displays how to use the A++ compiler from the command line.
 */
void print_usage()
{
    // This function prints instructions on how to use the program

    printf("Usage: ./build/bin/a++c <source_file>.a++\n");
}

/**
 * @brief Runs the A++ compiler on the specified file.
 *
 * This function reads the input file, initializes the lexer and parser,
 * generates the AST, and interprets the code.
 *
 * @param filename The path to the .a++ source file to compile and run.
 */
void run_file(const char *filename)
{
    // This function opens the source file, reads its contents, and prepares for compilation

    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        // If the file couldn't be opened, print an error message and exit
        printf("Error: Could not open file '%s'.\n", filename);
        exit(1);
    }

    // Move to the end of the file to determine its size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    // Move back to the start of the file
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the source code
    char *source_code = (char *)malloc(file_size + 1);
    if (!source_code)
    {
        // If memory allocation fails, print an error message and exit
        printf("Error: Failed to allocate memory for source code.\n");
        fclose(file);
        exit(1);
    }

    // Read the entire file into the allocated memory
    size_t read_size = fread(source_code, 1, file_size, file);
    source_code[read_size] = '\0'; // Add a null terminator to the end of the string

    // Close the file as we're done reading from it
    fclose(file);

    // Initialize the lexer with the source code
    Lexer *lexer = init_lexer(source_code);

    // Create a parser using the lexer
    Parser *parser = create_parser(lexer);

    // Parse the tokens to create an Abstract Syntax Tree (AST)
    ASTNode *ast = parse_tokens(parser);

    if (ast == NULL)
    {
        // If parsing failed, print an error message
        printf("Error: Failed to parse the source file.\n");
        free_parser(parser);
        free(lexer);
        free(source_code);
        exit(1);
    }

    // Interpret the AST (execute the program)
    interpret(ast);

    // Clean up: free all allocated memory
    free_parser(parser);
    free_ast(ast);
    free(lexer);
    free(source_code);
}

/**
 * @brief Main entry point for the A++ compiler.
 *
 * This function handles command-line arguments and initiates the compilation process.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return int Returns 0 on successful execution, 1 on error.
 */
int main(int argc, char *argv[])
{
    // This is the main function, the entry point of the program
    if (argc != 2)
    {
        // If the wrong number of arguments is provided, print usage instructions and exit
        print_usage();
        return 1;
    }

    const char *filename = argv[1];           // Get the filename from the command line argument
    const char *ext = strrchr(filename, '.'); // Get the file extension

    if (!ext || strcmp(ext, ".a++") != 0)
    {
        // If the file doesn't have a .a++ extension, print an error message and exit
        printf("Error: Input file must have a .a++ extension.\n");
        return 1;
    }

    // Run the compiler on the provided file
    run_file(filename);

    return 0; // Return 0 to indicate successful execution
}
