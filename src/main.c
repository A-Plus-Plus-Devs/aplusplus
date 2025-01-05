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

// Add array-specific includes
#include "array/array_lexer.h"
#include "array/array_parser.h"
#include "array/array_interpreter.h"
#include "array/array_ast.h"

// Global array interpreter instance
static ArrayInterpreter *array_interpreter = NULL;

/**
 * @brief Initializes the array subsystem.
 * @return true if initialization was successful, false otherwise
 */
static bool init_array_subsystem() {
    array_interpreter = array_interpreter_init();
    return array_interpreter != NULL;
}

/**
 * @brief Cleans up the array subsystem.
 */
static void cleanup_array_subsystem() {
    if (array_interpreter) {
        array_interpreter_free(array_interpreter);
        array_interpreter = NULL;
    }
}

/**
 * @brief Handles array-specific operations.
 * @param source The array operation source code
 * @return true if operation was successful, false otherwise
 */
static bool handle_array_operation(const char *source_code) {
    printf("Handling array operation:\n%s\n", source_code);
    
    // Initialize array subsystem components
    ArrayLexer *array_lexer = array_lexer_init(source_code);
    if (!array_lexer) {
        printf("Error: Failed to initialize array lexer\n");
        return false;
    }

    ArrayParser *array_parser = array_parser_init(array_lexer);
    if (!array_parser) {
        array_lexer_free(array_lexer);
        printf("Error: Failed to initialize array parser\n");
        return false;
    }

    // Parse the array operation
    printf("Parsing array operation...\n");
    ArrayASTNode *array_ast = array_parse(array_parser);
    if (!array_ast) {
        array_parser_free(array_parser);
        array_lexer_free(array_lexer);
        printf("Error: Failed to parse array operation\n");
        return false;
    }
    printf("Successfully created AST\n");

    // Initialize array interpreter
    ArrayInterpreter *interpreter = array_interpreter_init();
    if (!interpreter) {
        array_free_ast(array_ast);
        array_parser_free(array_parser);
        array_lexer_free(array_lexer);
        printf("Error: Failed to initialize array interpreter\n");
        return false;
    }
    printf("Interpreter initialized\n");

    // Interpret the array operation
    printf("Interpreting array operation...\n");
    ArrayInterpretResult result = array_interpret(interpreter, array_ast);
    printf("Interpretation %s\n", result.success ? "succeeded" : "failed");

    // Cleanup
    array_interpreter_free(interpreter);
    array_free_ast(array_ast);
    array_parser_free(array_parser);
    array_lexer_free(array_lexer);

    return result.success;
}

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
static void run_file(const char *filename)
{
    // Read the source file
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Could not open file '%s'\n", filename);
        exit(1);
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory for the source code
    char *source_code = (char *)malloc(file_size + 1);
    if (!source_code)
    {
        printf("Error: Failed to allocate memory for source code.\n");
        fclose(file);
        exit(1);
    }

    // Read the file
    size_t bytes_read = fread(source_code, 1, file_size, file);
    source_code[bytes_read] = '\0';
    fclose(file);

    printf("Processing file contents:\n%s\n", source_code);

    // Check if this is an array operation
    if (strstr(source_code, "<int>") || strstr(source_code, "<string>") ||
        strstr(source_code, "<float>") || strstr(source_code, "<bool>") ||
        strstr(source_code, "<char>")) {
        
        printf("Detected array operation\n");
        if (!handle_array_operation(source_code)) {
            printf("Error: Failed to handle array operation\n");
            free(source_code);
            exit(1);
        }
        free(source_code);
        return;
    }

    // Continue with regular parsing if not an array operation
    Lexer *lexer = init_lexer(source_code);
    if (!lexer) {
        printf("Error: Failed to initialize lexer.\n");
        free(source_code);
        exit(1);
    }

    // Initialize parser
    Parser *parser = create_parser(lexer);
    if (!parser)
    {
        printf("Error: Failed to initialize parser.\n");
        free(source_code);
        exit(1);
    }

    // Parse the source code
    ASTNode *ast = parse_tokens(parser);
    if (!ast)
    {
        printf("Error: Failed to parse the source file.\n");
        free_parser(parser);
        free(lexer);
        free(source_code);
        exit(1);
    }

    // Interpret the AST (execute the program)
    interpret(ast);

    // Clean up
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
