#include <ptx_print/ptx_initializer_value_print.h>
#include <ptx_print/ptx_expr_print.h>
#include <string.h>
#include <inttypes.h> // For PRId64

/**
 * Convert a mask value to its string representation.
 */
static const char* mask_value_to_string(ptx_mask_value_t mask) {
    switch (mask) {
        case MASK_BYTE0:
            return "0xFF";
        case MASK_BYTE1:
            return "0xFF00";
        case MASK_BYTE2:
            return "0xFF0000";
        case MASK_BYTE3:
            return "0xFF000000";
        case MASK_BYTE4:
            return "0xFF00000000";
        case MASK_BYTE5:
            return "0xFF0000000000";
        case MASK_BYTE6:
            return "0xFF000000000000";
        case MASK_BYTE7:
            return "0xFF00000000000000";
        case MASK_UNMASKED:
        default:
            return "";
    }
}

/**
 * Print an address operand (variable reference with optional offset and generic flag).
 */
static int print_address_operand(FILE* file, const ptx_address_operand_t* addr) {
    int count = 0;
    
    // Handle generic() wrapper if needed
    if (addr->is_generic) {
        count += fprintf(file, "generic(");
    }
    
    // Print variable name
    count += fprintf(file, "%s", addr->var_name);
    
    // Add offset if non-zero
    if (addr->offset > 0) {
        count += fprintf(file, "+%" PRId64, addr->offset);
    } else if (addr->offset < 0) {
        count += fprintf(file, "%" PRId64, addr->offset);
    }
    
    // Close generic() wrapper if needed
    if (addr->is_generic) {
        count += fprintf(file, ")");
    }
    
    return count;
}

/**
 * Print a masked value (using the mask() operator syntax).
 */
static int print_masked_value(FILE* file, ptx_mask_value_t mask, const void* value, bool is_scalar) {
    int count = 0;
    const char* mask_str = mask_value_to_string(mask);
    
    // If no mask or MASK_UNMASKED, just print the value
    if (mask == MASK_UNMASKED || strlen(mask_str) == 0) {
        if (is_scalar) {
            ptx_expr_t* expr = (ptx_expr_t*)value;
            // First write to a temporary string to get the actual count
            char temp_buf[256];
            FILE* temp_file = fmemopen(temp_buf, sizeof(temp_buf), "w");
            if (!temp_file) return -1;
            
            bool success = print_expr_to_file(temp_file, expr);
            fclose(temp_file);
            
            if (!success) return -1;
            
            // Now print the result to the actual file and return the length
            count = fprintf(file, "%s", temp_buf);
            return (count > 0) ? count : -1;
        } else {
            const ptx_address_operand_t* addr = (const ptx_address_operand_t*)value;
            return print_address_operand(file, addr);
        }
    }
    
    // Print with mask
    count += fprintf(file, "%s(", mask_str);
    
    if (is_scalar) {
        ptx_expr_t* expr = (ptx_expr_t*)value;
        // First write to a temporary string to get the actual count
        char temp_buf[256];
        FILE* temp_file = fmemopen(temp_buf, sizeof(temp_buf), "w");
        if (!temp_file) return -1;
        
        bool success = print_expr_to_file(temp_file, expr);
        fclose(temp_file);
        
        if (!success) return -1;
        
        // Now print the result to the actual file
        int result = fprintf(file, "%s", temp_buf);
        if (result < 0) return -1;
        count += result;
    } else {
        const ptx_address_operand_t* addr = (const ptx_address_operand_t*)value;
        int result = print_address_operand(file, addr);
        if (result < 0) return -1;
        count += result;
    }
    
    count += fprintf(file, ")");
    return count;
}

int print_ptx_initializer_value(FILE* file, const ptx_initializer_value_t* value) {
    if (value == NULL || file == NULL) {
        return -1;
    }
    
    /* Using direct enum value indices to avoid redefinition issues
     * 0 = INIT_VALUE_SCALAR
     * 1 = INIT_VALUE_ADDR_VAR
     */
    if (value->kind == 0) { // INIT_VALUE_SCALAR
        return print_masked_value(file, value->mask, value->scalar_expr, true);
    } else if (value->kind == 1) { // INIT_VALUE_ADDR_VAR
        return print_masked_value(file, value->mask, &value->addr_var, false);
    } else {
        // Unknown initializer type
        return -1;
    }
}

int sprint_ptx_initializer_value(char* str, size_t size, const ptx_initializer_value_t* value) {
    if (value == NULL || str == NULL || size == 0) {
        return -1;
    }
    
    // Create a temporary file in memory
    FILE* mem_file = fmemopen(str, size, "w");
    if (mem_file == NULL) {
        return -1;
    }
    
    int result = print_ptx_initializer_value(mem_file, value);
    fclose(mem_file);
    
    // Ensure null termination
    if (result >= 0 && (size_t)result < size) {
        str[result] = '\0';
    } else if ((size_t)result >= size && size > 0) {
        str[size - 1] = '\0';
    }
    
    return result;
} 