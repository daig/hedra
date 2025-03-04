#pragma once

#include <ptx_ast/ptx_instruction.h>
#include <stdbool.h>

/**
 * @brief Parse a PTX instruction name string and store in ptx_instruction_tag
 * 
 * This function takes a string representing a PTX instruction (e.g., "add", "mul", "ld"),
 * and determines the corresponding instruction type from the ptx_instruction_tag enum.
 *
 * @param str The instruction string to parse
 * @param instruction_tag Pointer to ptx_instruction_tag where the result will be stored
 * @return true if successful, false if the instruction is not recognized
 */
bool parse_instruction_tag(const char* str, ptx_instruction_tag* instruction_tag);

/**
 * @brief Parse a predicate expression and create a ptx_predicate_t
 * 
 * This function parses a predicate expression like "@%p_cond" or "@!%p_cond" and
 * creates a predicate structure with the name and negation flag.
 *
 * @param str The predicate string to parse (should start with @)
 * @param predicate Pointer to ptx_predicate_t* where the allocated result will be stored
 * @return true if successful, false if parsing fails
 */
bool parse_predicate(const char* str, ptx_predicate_t** predicate);

/**
 * @brief Parse a full instruction with optional predicate
 * 
 * This function parses a complete instruction which may include a predicate.
 * It creates a full ptx_instruction_t structure.
 *
 * @param str The string to parse (may start with a predicate)
 * @param instruction Pointer to ptx_instruction_t* where the allocated result will be stored
 * @return true if successful, false if parsing fails
 */
bool parse_full_instruction(const char* str, ptx_instruction_t** instruction);

/**
 * @brief Parse instruction modifiers from a string
 * 
 * @param str The string to parse
 * @param instruction The instruction to add modifiers to
 * @param chars_read Pointer to store the number of characters read
 * @return true if parsing was successful, false otherwise
 */
bool parse_instruction_modifiers(const char* str, ptx_instruction_t* instruction, int* chars_read);

/**
 * @brief Parse instruction operands from a string
 * 
 * @param str The string to parse
 * @param instruction The instruction to add operands to
 * @param chars_read Pointer to store the number of characters read
 * @return true if parsing was successful, false otherwise
 */
bool parse_instruction_operands(const char* str, ptx_instruction_t* instruction, int* chars_read); 