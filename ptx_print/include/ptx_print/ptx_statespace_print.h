#pragma once

#include <ptx_ast/ptx_statespace.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Prints a PTX state space to a file stream
 * 
 * @param file The file stream to print to
 * @param space The PTX state space to print
 * @return bool true if successful, false otherwise
 */
bool print_state_space_to_file(FILE* file, ptx_state_space_t space);

/**
 * @brief Prints a PTX state space to a string buffer
 * 
 * @param buffer The buffer to print to
 * @param buffer_size The size of the buffer
 * @param space The PTX state space to print
 * @return int The number of characters printed, or negative on error
 */
int print_state_space_to_buffer(char* buffer, size_t buffer_size, ptx_state_space_t space); 