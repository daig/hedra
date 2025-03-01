#pragma once

#include <ptx_ast/ptx_label.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Prints a PTX label to a file stream
 * 
 * @param file The file stream to print to
 * @param label The label to print
 * @return bool true if successful, false otherwise
 */
bool print_label_to_file(FILE* file, const ptx_label_t* label);

/**
 * @brief Prints a PTX label to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param label The label to print
 * @return int The number of characters printed, or negative on error
 */
int print_label_to_buffer(char* buffer, size_t buffer_size, const ptx_label_t* label); 