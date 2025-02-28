#include "ptx_predefined_identifier.h"
#include "ptx_identifier.h"
#include "ptx_constant.h"
#include <stdio.h>
#include <math.h> // For isnan, isinf

// Helper function to print float in both decimal and hex representation
void print_float(float f) {
    uint32_t bits;
    memcpy(&bits, &f, sizeof(float));
    printf("%.10g (hex: 0x%08X)", (double)f, bits);
}

// Helper function to print double in both decimal and hex representation
void print_double(double d) {
    uint64_t bits;
    memcpy(&bits, &d, sizeof(double));
    printf("%.17g (hex: 0x%016lX)", d, bits);
}

int main() {
    // Test predefined identifier lookup
    predefined_identifier_t clock_id = get_predefined_identifier("%clock");
    printf("Predefined identifier %%clock has value: %d\n", clock_id);
    
    predefined_identifier_t invalid_id = get_predefined_identifier("invalid_identifier");
    printf("Invalid predefined identifier has value: %d\n", invalid_id);
    
    // Test user-defined identifier validation
    const char* valid_ids[] = {
        "valid_id",
        "_valid",
        "$valid",
        "%valid"
    };
    
    const char* invalid_ids[] = {
        "",
        "invalid@id",
        "_",
        "$",
        "%"
    };
    
    printf("\nTesting valid identifiers:\n");
    for (int i = 0; i < sizeof(valid_ids) / sizeof(valid_ids[0]); i++) {
        printf("'%s' is %s\n", valid_ids[i], 
               is_valid_user_defined_identifier(valid_ids[i]) ? "valid" : "INVALID (unexpected)");
    }
    
    printf("\nTesting invalid identifiers:\n");
    for (int i = 0; i < sizeof(invalid_ids) / sizeof(invalid_ids[0]); i++) {
        printf("'%s' is %s\n", invalid_ids[i], 
               is_valid_user_defined_identifier(invalid_ids[i]) ? "VALID (unexpected)" : "invalid");
    }
    
    // Test PTX constants parsing
    printf("\nTesting PTX constant parsing:\n");
    
    // Test cases for different integer literals
    const char* int_literals[] = {
        "123",          // decimal
        "0xABC",        // hexadecimal
        "0123",         // octal
        "0b101",        // binary
        "123U",         // unsigned decimal
        "0xABCU",       // unsigned hexadecimal
        "0123U",        // unsigned octal
        "0b101U",       // unsigned binary
        "9223372036854775807",    // max signed 64-bit
        "18446744073709551615U"   // max unsigned 64-bit
    };
    
    ptx_constant_t constant;
    
    for (int i = 0; i < sizeof(int_literals) / sizeof(int_literals[0]); i++) {
        if (parse_int_literal(int_literals[i], &constant)) {
            printf("'%s' parsed successfully: ", int_literals[i]);
            if (constant.type == PTX_CONST_INT_SIGNED) {
                printf("signed value = %lld (.s64)\n", (long long)constant.s64_val);
            } else {
                printf("unsigned value = %llu (.u64)\n", (unsigned long long)constant.u64_val);
            }
        } else {
            printf("'%s' parsing FAILED\n", int_literals[i]);
        }
    }
    
    // Test invalid integer literals
    const char* invalid_int_literals[] = {
        "abc",          // not a number
        "0xG123",       // invalid hex
        "09876",        // invalid octal (8 and 9 not allowed)
        "0b210",        // invalid binary (only 0 and 1 allowed)
        "-123"          // negative not supported by our parser
    };
    
    printf("\nTesting invalid integer literals:\n");
    for (int i = 0; i < sizeof(invalid_int_literals) / sizeof(invalid_int_literals[0]); i++) {
        if (!parse_int_literal(invalid_int_literals[i], &constant)) {
            printf("'%s' correctly rejected\n", invalid_int_literals[i]);
        } else {
            printf("'%s' incorrectly parsed to ", invalid_int_literals[i]);
            if (constant.type == PTX_CONST_INT_SIGNED) {
                printf("signed value = %lld (.s64)\n", (long long)constant.s64_val);
            } else {
                printf("unsigned value = %llu (.u64)\n", (unsigned long long)constant.u64_val);
            }
        }
    }
    
    // Test floating-point literal parsing
    printf("\nTesting floating-point constant parsing:\n");
    
    // Test cases for different floating-point literals
    const char* float_literals[] = {
        "123.456",           // simple decimal
        "0.1",               // decimal less than 1
        "1e10",              // decimal with positive exponent
        "1.5e-5",            // decimal with negative exponent
        "0F3f800000",        // hex representation of 1.0f (32-bit)
        "0F00000000",        // hex representation of 0.0f (32-bit)
        "0F7f800000",        // hex representation of +infinity (32-bit)
        "0Fff800000",        // hex representation of NaN (32-bit)
        "0D3ff0000000000000" // hex representation of 1.0 (64-bit)
    };
    
    ptx_constant_t float_constant;
    
    for (int i = 0; i < sizeof(float_literals) / sizeof(float_literals[0]); i++) {
        if (parse_float_literal(float_literals[i], &float_constant)) {
            printf("'%s' parsed successfully: ", float_literals[i]);
            
            if (float_constant.type == PTX_CONST_FLOAT) {
                // 64-bit double-precision
                printf("double value = ");
                print_double(float_constant.f64_val);
                printf(" (.f64)\n");
            } else if (float_constant.type == PTX_CONST_FLOAT_SINGLE) {
                // 32-bit single-precision
                printf("float value = ");
                print_float(float_constant.f32_val);
                printf(" (.f32)\n");
            }
        } else {
            printf("'%s' parsing FAILED\n", float_literals[i]);
        }
    }
    
    // Test invalid floating-point literals
    const char* invalid_float_literals[] = {
        "123.456f",          // suffix not allowed
        "0F123",             // too short for f32 hex
        "0F123456789",       // too long for f32 hex
        "0D123",             // too short for f64 hex
        "123.456.789"        // multiple decimal points
    };
    
    printf("\nTesting invalid floating-point literals:\n");
    for (int i = 0; i < sizeof(invalid_float_literals) / sizeof(invalid_float_literals[0]); i++) {
        if (!parse_float_literal(invalid_float_literals[i], &float_constant)) {
            printf("'%s' correctly rejected\n", invalid_float_literals[i]);
        } else {
            printf("'%s' incorrectly parsed\n", invalid_float_literals[i]);
        }
    }
    
    // Test WARP_SZ constant
    printf("\nWARP_SZ value: %d\n", WARP_SZ);
    
    return 0;
} 