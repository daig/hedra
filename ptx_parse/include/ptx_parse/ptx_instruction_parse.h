#pragma once

#include <ptx_ast/ptx_instruction.h>
#include <stdbool.h>

/**
 * @brief Parse a PTX instruction string and store in ptx_instruction_t
 * 
 * This function takes a string representing a PTX instruction (e.g., "add", "mul", "ld"),
 * and determines the corresponding instruction type from the ptx_instruction_t enum.
 *
 * @param str The instruction string to parse
 * @param instruction Pointer to ptx_instruction_t where the result will be stored
 * @return true if successful, false if the instruction is not recognized
 */
bool parse_instruction(const char* str, ptx_instruction_t* instruction); 