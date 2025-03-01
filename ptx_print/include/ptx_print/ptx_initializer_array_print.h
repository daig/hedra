#pragma once

#include <prelude/ptx_initializer_array.h>
#include <prelude/ptx_array_shape.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * Print a PTX initializer array to the given file.
 * This function recursively prints nested arrays with proper formatting.
 * 
 * @param file The file to print to.
 * @param array The initializer array to print.
 * @return true if successful, false if an error occurs.
 */
bool print_ptx_initializer_array_to_file(FILE* file, const ptx_initializer_array_t* array);

/**
 * Print a PTX initializer array to a string.
 * 
 * @param str The string to print to.
 * @param size The size of the string buffer.
 * @param array The initializer array to print.
 * @return The number of characters printed, or -1 if an error occurs.
 */
int sprint_ptx_initializer_array(char* str, size_t size, const ptx_initializer_array_t* array); 