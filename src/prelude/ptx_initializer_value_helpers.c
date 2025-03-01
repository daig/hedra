/**
 * @file ptx_initializer_value_helpers.c
 * @brief Implementation of helper constructor functions for ptx_initializer_value_t variants
 */

#include "ast/ptx_initializer_value_helpers.h"
#include <string.h>

ptx_initializer_value_t ptx_initializer_value_int(int64_t value) {
    ptx_initializer_value_t val;
    memset(&val, 0, sizeof(ptx_initializer_value_t));
    val.kind = INIT_SCALAR_INT;
    val.value.int_val = value;
    return val;
}

ptx_initializer_value_t ptx_initializer_value_float(double value) {
    ptx_initializer_value_t val;
    memset(&val, 0, sizeof(ptx_initializer_value_t));
    val.kind = INIT_SCALAR_FLOAT;
    val.value.float_val = value;
    return val;
}

ptx_initializer_value_t ptx_initializer_value_var_addr(const char* var_name, int64_t offset, bool is_generic) {
    ptx_initializer_value_t val;
    memset(&val, 0, sizeof(ptx_initializer_value_t));
    val.kind = INIT_VAR_ADDR;
    val.value.var_addr.var_name = var_name;
    val.value.var_addr.offset = offset;
    val.value.var_addr.is_generic = is_generic;
    return val;
}

ptx_initializer_value_t ptx_initializer_value_func_addr(const char* func_name) {
    ptx_initializer_value_t val;
    memset(&val, 0, sizeof(ptx_initializer_value_t));
    val.kind = INIT_FUNC_ADDR;
    val.value.func_name = func_name;
    return val;
}

ptx_initializer_value_t ptx_initializer_value_masked_var_addr(const char* var_name, int64_t offset, bool is_generic, ptx_mask_value_t mask) {
    ptx_initializer_value_t val;
    memset(&val, 0, sizeof(ptx_initializer_value_t));
    val.kind = INIT_MASKED_VAR_ADDR;
    val.value.masked.value.var_addr.var_name = var_name;
    val.value.masked.value.var_addr.offset = offset;
    val.value.masked.value.var_addr.is_generic = is_generic;
    val.value.masked.mask = mask;
    return val;
}

ptx_initializer_value_t ptx_initializer_value_masked_int(int64_t value, ptx_mask_value_t mask) {
    ptx_initializer_value_t val;
    memset(&val, 0, sizeof(ptx_initializer_value_t));
    val.kind = INIT_MASKED_INT;
    val.value.masked.value.int_val = value;
    val.value.masked.mask = mask;
    return val;
} 