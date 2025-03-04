#pragma once
#include <ptx_ast/ptx_memory_operand.h>
#include <stdbool.h>

/**
 * Parse a memory operand from a string
 * 
 * @param str The string to parse
 * @param operand Pointer to store the parsed memory operand
 * @param chars_read Pointer to store the number of characters read
 * @return true if parsing was successful, false otherwise
 */
bool parse_memory_operand(const char* str, ptx_memory_operand_t** operand, int* chars_read); 