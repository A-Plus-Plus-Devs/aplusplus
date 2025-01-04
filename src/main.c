/*
 * A++ Programming Language Main Entry Point
 * 
 * This file serves as the entry point for the A++ programming language interpreter.
 * It handles command-line arguments, file loading, and orchestrates the compilation
 * and execution process.
 * 
 * Key features:
 * - Command-line argument processing
 * - Source file loading and validation
 * - Compilation pipeline coordination
 * - Error handling and reporting
 * - Version and help information
 *
 * Original Author: Paul Kabulu
 * Created: August 2024
 * 
 * Edited by:
 *
 * File: src/main.c
 */

#include <stdio.h>  
#include <stdlib.h> // This line includes the standard library for functions like malloc and free
#include <string.h>
#include "lexer/lexer.h"           
#include "parser/parser.h"         
#include "interpreter/interpreter.h" 
#include "common/version.h" 

/**
 * @brief Prints the usage instructions for the A++ compiler.
 *
 * This function displays how to use the A++ compiler from the command line.
 */
void print_usage()
{
    printf("A++ Programming Language %s\n\n", A_PLUS_PLUS_VERSION_STRING);
    printf("Usage: a++ [options] <source_file>.a++\n\n");
    printf("Options:\n");
    printf("  --version     Display version information\n");
    printf("  --help        Display this help message\n");
}

/**
 * @brief Prints version information for the A++ compiler.
 */
void print_version()
{
    printf("A++ Programming Language %s\n", A_PLUS_PLUS_VERSION_STRING);
    printf("Copyright (c) 2024 Paul Kabulu\n");
    printf("Released under the MIT License\n");
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
    if (argc < 2)
    {
        print_usage();
        return 1;
    }

    // Check for --version flag
    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0)
    {
        print_version();
        return 0;
    }

    // Check for --help flag
    if (strcmp(argv[1], "--help") == 0)
    {
        print_usage();
        return 0;
    }

    // Check if the file has the correct extension
    const char *filename = argv[1];
    if (strlen(filename) < 5 || strcmp(filename + strlen(filename) - 4, ".a++") != 0)
    {
        printf("Error: File must have .a++ extension\n");
        return 1;
    }

    run_file(filename);
    return 0;
}
