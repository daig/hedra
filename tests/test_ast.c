#include "ptx_predefined_identifier.h"
#include "ptx_identifier.h"
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
    
    return 0;
} 