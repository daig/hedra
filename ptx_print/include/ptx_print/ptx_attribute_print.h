/**
 * @file ptx_attribute_print.h
 * @brief Functions for printing PTX variable and function attributes
 */

#ifndef HEDRA_PTX_ATTRIBUTE_PRINT_H
#define HEDRA_PTX_ATTRIBUTE_PRINT_H

#include <ptx_ast/ptx_declaration_lhs.h>

/**
 * @brief Print an attribute tag to a buffer
 * 
 * This function prints a PTX attribute tag to the provided buffer.
 * 
 * @param buffer Buffer to print to
 * @param buffer_size Size of the buffer
 * @param tag Attribute tag to print
 * @return int Number of characters printed or -1 if buffer is too small
 */
int print_attribute_tag_to_buffer(char* buffer, size_t buffer_size, ptx_attribute_tag_t tag);

/**
 * @brief Print a complete attribute to a buffer
 * 
 * This function prints a complete PTX attribute to the provided buffer,
 * including any parameters (like UUID for unified attributes).
 * 
 * @param buffer Buffer to print to
 * @param buffer_size Size of the buffer
 * @param attr Attribute to print
 * @return int Number of characters printed or -1 if buffer is too small
 */
int print_attribute_to_buffer(char* buffer, size_t buffer_size, const ptx_attribute_t* attr);

#endif /* HEDRA_PTX_ATTRIBUTE_PRINT_H */ 