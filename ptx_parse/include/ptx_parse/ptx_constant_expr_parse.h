#pragma once

#include <ptx_ast/ptx_constant_expr.h>
#include <stdbool.h>

/**
 * @brief Parse a primary expression (constant or parenthesized expression)
 * 
 * @param str The string to parse
 * @param pos Pointer to the current position in the string (will be updated)
 * @param expr Pointer to store the resulting expression
 * @return true if parsing succeeded, false otherwise
 */
bool parse_primary_expr(const char* str, int* pos, ptx_expr_t** expr);

/**
 * @brief Parse a unary expression (+expr, -expr, !expr, ~expr)
 * 
 * @param str The string to parse
 * @param pos Pointer to the current position in the string (will be updated)
 * @param expr Pointer to store the resulting expression
 * @return true if parsing succeeded, false otherwise
 */
bool parse_unary_expr(const char* str, int* pos, ptx_expr_t** expr);

/**
 * @brief Parse an expression using operator precedence parsing
 * 
 * @param str The string to parse
 * @param pos Pointer to the current position in the string (will be updated)
 * @param expr Pointer to store the resulting expression
 * @param min_precedence Minimum precedence level to parse
 * @return true if parsing succeeded, false otherwise
 */
bool parse_expr_precedence(const char* str, int* pos, ptx_expr_t** expr, int min_precedence);

/**
 * @brief Parse a constant expression
 * 
 * @param str The string to parse
 * @param expr Pointer to store the resulting expression
 * @return true if parsing succeeded, false otherwise
 */
bool parse_expr(const char* str, ptx_expr_t** expr); 