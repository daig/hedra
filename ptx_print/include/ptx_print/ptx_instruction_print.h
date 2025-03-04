#pragma once

#include <ptx_ast/ptx_instruction.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Gets the string representation of a PTX instruction tag
 * 
 * @param tag The instruction tag to get the string for
 * @return const char* The string representation, or NULL if not found
 */
const char* get_instruction_string(ptx_instruction_tag tag);

/**
 * @brief Prints a PTX predicate to a file stream
 * 
 * @param file The file stream to print to
 * @param predicate The predicate to print
 * @return bool true if successful, false otherwise
 */
bool print_predicate_to_file(FILE* file, const ptx_predicate_t* predicate);

/**
 * @brief Prints a PTX predicate to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param predicate The predicate to print
 * @return int The number of characters printed, or negative on error
 */
int print_predicate_to_buffer(char* buffer, size_t buffer_size, const ptx_predicate_t* predicate);

/**
 * @brief Prints a PTX instruction to a file stream
 * 
 * @param file The file stream to print to
 * @param instruction The instruction to print
 * @return bool true if successful, false otherwise
 */
bool print_instruction_to_file(FILE* file, const ptx_instruction_t instruction);

/**
 * @brief Prints a PTX instruction to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param instruction The instruction to print
 * @return int The number of characters printed, or negative on error
 */
int print_instruction_to_buffer(char* buffer, size_t buffer_size, const ptx_instruction_t instruction);

/**
 * @brief Prints instruction modifiers to a file stream
 * 
 * @param file The file stream to print to
 * @param modifiers The modifiers to print
 * @return bool true if successful, false otherwise
 */
bool print_instruction_modifiers_to_file(FILE* file, const ptx_instruction_modifier_t* modifiers);

/**
 * @brief Prints instruction modifiers to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param modifiers The modifiers to print
 * @return int The number of characters printed, or negative on error
 */
int print_instruction_modifiers_to_buffer(char* buffer, size_t buffer_size, const ptx_instruction_modifier_t* modifiers);

/**
 * @brief Prints instruction operands to a file stream
 * 
 * @param file The file stream to print to
 * @param instruction The instruction containing the operands to print
 * @return bool true if successful, false otherwise
 */
bool print_instruction_operands_to_file(FILE* file, const ptx_instruction_t* instruction);

/**
 * @brief Prints instruction operands to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param instruction The instruction containing the operands to print
 * @return int The number of characters printed, or negative on error
 */
int print_instruction_operands_to_buffer(char* buffer, size_t buffer_size, const ptx_instruction_t* instruction); 