#pragma once

#include <ptx_ast/ptx_type_decl.h>
#include <ptx_ast/ptx_constant_expr.h>
#include <stdbool.h>

/**
 * @brief Parse a mask value from a string
 * 
 * @param str String to parse
 * @param pos Pointer to current position in string (will be updated)
 * @param mask Pointer to mask value to be filled in
 * @return true if parsing successful, false otherwise
 */
bool parse_mask(const char* str, int* pos, ptx_mask_value_t* mask);

/**
 * @brief Parse a masked expression from a string
 * 
 * @param str String to parse
 * @param pos Pointer to current position in string (will be updated)
 * @param value Pointer to initializer value to be filled in
 * @return true if parsing successful, false otherwise
 */
bool parse_masked_expr(const char* str, int* pos, ptx_initializer_value_t* value);

/**
 * @brief Parse a variable address expression from a string
 * 
 * @param str String to parse
 * @param pos Pointer to current position in string (will be updated)
 * @param value Pointer to initializer value to be filled in
 * @return true if parsing successful, false otherwise
 */
bool parse_var_addr_expr(const char* str, int* pos, ptx_initializer_value_t* value);

/**
 * @brief Parse a generic variable address expression from a string
 * 
 * @param str String to parse
 * @param pos Pointer to current position in string (will be updated)
 * @param value Pointer to initializer value to be filled in
 * @return true if parsing successful, false otherwise
 */
bool parse_generic_addr_expr(const char* str, int* pos, ptx_initializer_value_t* value);

/**
 * @brief Parse a scalar initializer value from a string
 * 
 * @param str The string to parse
 * @param pos Pointer to the current position in the string (will be updated)
 * @param value Pointer to the initializer value structure to fill
 * @param data_type Optional type information for the data being initialized (can be NULL)
 * @return true if parsing succeeded, false otherwise
 */
bool parse_scalar_initializer(const char* str, int* pos, ptx_initializer_value_t* value, ptx_type_t data_type);

/**
 * @brief Parse an initializer value from a string
 * 
 * @param str String to parse
 * @param value Pointer to initializer value to be filled in
 * @param data_type The data type of the elements to be parsed
 * @return true if parsing successful, false otherwise
 */
bool parse_initializer_value(const char* str, ptx_initializer_value_t* value, ptx_type_t data_type); 