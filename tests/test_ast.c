#include "ptx_predefined_identifier.h"
#include "ptx_identifier.h"
#include "ptx_constant.h"
#include <stdio.h>

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
    
    // Test WARP_SZ constant
    printf("\nWARP_SZ value: %d\n", WARP_SZ);
    
    return 0;
} 