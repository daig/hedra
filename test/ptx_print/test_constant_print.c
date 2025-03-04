#include <ptx_print/ptx_constant_print.h>
#include <ptx_parse/ptx_constant_parse.h>
#include <ptx_ast/ptx_constant.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define BUFFER_SIZE 128
#define FLOAT_EPSILON 1e-6

// Helper function to normalize whitespace for comparison
void normalize_whitespace(char* str) {
    if (!str) return;
    
    char* src = str;
    char* dst = str;
    
    // Skip leading whitespace
    while (*src && (*src == ' ' || *src == '\t' || *src == '\n')) {
        src++;
    }
    
    // Copy non-whitespace characters and collapse multiple whitespace to single space
    int was_whitespace = 0;
    while (*src) {
        if (*src == ' ' || *src == '\t' || *src == '\n') {
            if (!was_whitespace) {
                *dst++ = ' ';
                was_whitespace = 1;
            }
        } else {
            *dst++ = *src;
            was_whitespace = 0;
        }
        src++;
    }
    
    // Remove trailing whitespace
    if (dst > str && *(dst-1) == ' ') {
        dst--;
    }
    
    *dst = '\0';
}

// Helper function to convert a hex string to its decimal equivalent for comparison
void normalize_hex_to_decimal(char* str) {
    if (!str || strncmp(str, "0x", 2) != 0 && strncmp(str, "0X", 2) != 0) {
        return;  // Not a hex string
    }
    
    // Convert hex to decimal
    char* endptr;
    unsigned long long value = strtoull(str, &endptr, 16);
    
    // Check if it's unsigned
    bool is_unsigned = (*endptr == 'U' || *endptr == 'u');
    
    // Format as decimal
    if (is_unsigned) {
        sprintf(str, "%lluU", value);
    } else {
        sprintf(str, "%lld", (long long)value);
    }
}

// Helper function to convert a binary string to its decimal equivalent for comparison
void normalize_binary_to_decimal(char* str) {
    if (!str || strncmp(str, "0b", 2) != 0 && strncmp(str, "0B", 2) != 0) {
        return;  // Not a binary string
    }
    
    // Convert binary to decimal
    char* endptr;
    unsigned long long value = strtoull(str + 2, &endptr, 2);
    
    // Check if it's unsigned
    bool is_unsigned = (*endptr == 'U' || *endptr == 'u');
    
    // Format as decimal
    if (is_unsigned) {
        sprintf(str, "%lluU", value);
    } else {
        sprintf(str, "%lld", (long long)value);
    }
}

// Helper function to convert an octal string to its decimal equivalent for comparison
void normalize_octal_to_decimal(char* str) {
    if (!str || str[0] != '0' || str[1] == 'x' || str[1] == 'X' || str[1] == 'b' || str[1] == 'B') {
        return;  // Not an octal string
    }
    
    // Convert octal to decimal
    char* endptr;
    unsigned long long value = strtoull(str, &endptr, 8);
    
    // Check if it's unsigned
    bool is_unsigned = (*endptr == 'U' || *endptr == 'u');
    
    // Format as decimal
    if (is_unsigned) {
        sprintf(str, "%lluU", value);
    } else {
        sprintf(str, "%lld", (long long)value);
    }
}

// Function to check if a string is a floating point number
bool is_float(const char* str) {
    // Check if the string contains a decimal point or 'e'/'E' (scientific notation)
    if (strchr(str, '.') != NULL || strchr(str, 'e') != NULL || strchr(str, 'E') != NULL) {
        return true;
    }
    
    // Check for IEEE 754 hex formats
    if (strncmp(str, "0F", 2) == 0 || strncmp(str, "0f", 2) == 0 || 
        strncmp(str, "0D", 2) == 0 || strncmp(str, "0d", 2) == 0) {
        return true;
    }
    
    return false;
}

// Function to test round-trip parsing and printing
void test_parse_print_roundtrip(const char* constant_str) {
    ptx_constant_t* constant = NULL;
    char buffer[BUFFER_SIZE] = {0};
    
    // Parse the constant string
    assert(parse_constant(constant_str, &constant) == true);
    assert(constant != NULL);
    
    // Print the constant to the buffer
    int printed = print_constant_to_buffer(buffer, BUFFER_SIZE, *constant);
    assert(printed > 0);
    
    // Normalize whitespace for comparison
    char normalized_input[BUFFER_SIZE];
    char normalized_output[BUFFER_SIZE];
    
    strncpy(normalized_input, constant_str, BUFFER_SIZE);
    strncpy(normalized_output, buffer, BUFFER_SIZE);
    
    normalize_whitespace(normalized_input);
    normalize_whitespace(normalized_output);
    
    // For floats or if the output contains a decimal point, we need to compare values not strings
    if (is_float(normalized_input) || strchr(normalized_output, '.') != NULL) {
        double input_val = atof(normalized_input);
        double output_val = atof(normalized_output);
        
        // Handle division by zero for input_val == 0
        double rel_diff = 0.0;
        if (input_val != 0.0) {
            rel_diff = fabs((input_val - output_val) / input_val);
        } else {
            rel_diff = fabs(output_val);
        }
        
        printf("Original: '%s' (%.15g), Printed: '%s' (%.15g), Relative diff: %.15g\n", 
               normalized_input, input_val, normalized_output, output_val, rel_diff);
        
        // Check that the values are close enough
        assert(rel_diff < FLOAT_EPSILON);
    } else {
        // For non-floats, normalize the input format to decimal for comparison
        normalize_hex_to_decimal(normalized_input);
        normalize_binary_to_decimal(normalized_input);
        normalize_octal_to_decimal(normalized_input);
        
        // Check that the printed string matches the original
        printf("Original: '%s', Printed: '%s'\n", normalized_input, normalized_output);
        assert(strcmp(normalized_input, normalized_output) == 0);
    }
    
    // Free the allocated memory
    free_constant(constant);
    
    printf("Roundtrip test passed for constant: '%s'\n", constant_str);
}

void test_basic_constants() {
    printf("Testing basic constant printing...\n");
    
    // Test printing to buffer
    char buffer[BUFFER_SIZE];
    
    // Test signed integer
    ptx_constant_t signed_int = {
        .type = PTX_CONST_INT_SIGNED,
        .s64_val = -42,
        .original_str = NULL
    };
    int result = print_constant_to_buffer(buffer, BUFFER_SIZE, signed_int);
    assert(result > 0);
    assert(strcmp(buffer, "-42") == 0);
    
    // Test unsigned integer
    ptx_constant_t unsigned_int = {
        .type = PTX_CONST_INT_UNSIGNED,
        .u64_val = 42,
        .original_str = NULL
    };
    result = print_constant_to_buffer(buffer, BUFFER_SIZE, unsigned_int);
    assert(result > 0);
    assert(strcmp(buffer, "42U") == 0);
    
    // Test standard float
    ptx_constant_t float_val = {
        .type = PTX_CONST_FLOAT,
        .f64_val = 3.14159,
        .original_str = NULL
    };
    result = print_constant_to_buffer(buffer, BUFFER_SIZE, float_val);
    assert(result > 0);
    
    // Test single-precision float
    ptx_constant_t float_single_val = {
        .type = PTX_CONST_FLOAT_SINGLE,
        .f32_val = 2.71828f,
        .original_str = NULL
    };
    result = print_constant_to_buffer(buffer, BUFFER_SIZE, float_single_val);
    assert(result > 0);
    
    // Test scientific notation float
    ptx_constant_t float_scientific_val = {
        .type = PTX_CONST_FLOAT_SCIENTIFIC,
        .f64_val = 1.23e45,
        .original_str = strdup("1.23e45")
    };
    result = print_constant_to_buffer(buffer, BUFFER_SIZE, float_scientific_val);
    assert(result > 0);
    assert(strcmp(buffer, "1.23e45") == 0);
    free(float_scientific_val.original_str);
    
    // Test IEEE 754 single-precision hex format
    ptx_constant_t float_ieee_f32_val = {
        .type = PTX_CONST_FLOAT_IEEE_HEX_F32,
        .f32_val = 1.0f,
        .original_str = strdup("0F3f800000")
    };
    result = print_constant_to_buffer(buffer, BUFFER_SIZE, float_ieee_f32_val);
    assert(result > 0);
    assert(strcmp(buffer, "0F3f800000") == 0);
    free(float_ieee_f32_val.original_str);
    
    // Test IEEE 754 double-precision hex format
    ptx_constant_t float_ieee_f64_val = {
        .type = PTX_CONST_FLOAT_IEEE_HEX_F64,
        .f64_val = 1.0,
        .original_str = strdup("0D3ff0000000000000")
    };
    result = print_constant_to_buffer(buffer, BUFFER_SIZE, float_ieee_f64_val);
    assert(result > 0);
    assert(strcmp(buffer, "0D3ff0000000000000") == 0);
    free(float_ieee_f64_val.original_str);
    
    // Test predicate
    ptx_constant_t pred_val = {
        .type = PTX_CONST_PRED,
        .pred_val = 1,
        .original_str = NULL
    };
    result = print_constant_to_buffer(buffer, BUFFER_SIZE, pred_val);
    assert(result > 0);
    assert(strcmp(buffer, "1") == 0);
    
    printf("Basic constant printing tests passed!\n");
}

void test_roundtrip_parsing_printing() {
    printf("Testing roundtrip parsing and printing...\n");
    
    // Test integers
    test_parse_print_roundtrip("42");
    test_parse_print_roundtrip("-123");
    test_parse_print_roundtrip("0");
    test_parse_print_roundtrip("0xFF");
    test_parse_print_roundtrip("0xABCDEFU");
    test_parse_print_roundtrip("0b101010");
    test_parse_print_roundtrip("0123");  // Octal
    
    // Test standard floats
    test_parse_print_roundtrip("3.14159");
    
    // Test scientific notation
    test_parse_print_roundtrip("2.0e10");
    test_parse_print_roundtrip("-0.0001");
    test_parse_print_roundtrip("1.23e45");
    test_parse_print_roundtrip("9.87e-6");
    
    // Test IEEE 754 hex formats
    test_parse_print_roundtrip("0F3f800000");  // 1.0 in single-precision
    test_parse_print_roundtrip("0F40490fdb");  // 3.14159 in single-precision
    test_parse_print_roundtrip("0D3ff0000000000000");  // 1.0 in double-precision
    test_parse_print_roundtrip("0D400921fb54442d18");  // 3.14159 in double-precision
    
    printf("Roundtrip parsing and printing tests passed!\n");
}

void test_file_output() {
    printf("Testing constant printing to file...\n");
    
    // Open a temporary file for testing
    FILE* temp_file = tmpfile();
    assert(temp_file != NULL);
    
    // Create some constants
    ptx_constant_t signed_int = {
        .type = PTX_CONST_INT_SIGNED,
        .s64_val = -42,
        .original_str = NULL
    };
    
    ptx_constant_t float_val = {
        .type = PTX_CONST_FLOAT,
        .f64_val = 3.14159,
        .original_str = NULL
    };
    
    // Print constants to the file
    assert(print_constant_to_file(temp_file, signed_int) == true);
    fprintf(temp_file, " ");  // Add space between constants
    assert(print_constant_to_file(temp_file, float_val) == true);
    
    // Rewind the file to read from the beginning
    rewind(temp_file);
    
    // Read and check the contents
    char buffer[BUFFER_SIZE];
    assert(fgets(buffer, BUFFER_SIZE, temp_file) != NULL);
    
    // Remove trailing newline if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    // Check that the output contains the expected values
    // We can't do an exact string comparison because the float formatting might vary
    char* int_part = buffer;
    char* float_part = strchr(buffer, ' ');
    assert(float_part != NULL);
    *float_part = '\0';  // Split the string
    float_part++;  // Skip the space
    
    // Check the integer part
    assert(strcmp(int_part, "-42") == 0);
    
    // Check the float part (convert to double and compare)
    double parsed_float = atof(float_part);
    assert(fabs(parsed_float - 3.14159) < FLOAT_EPSILON);
    
    // Close the file
    fclose(temp_file);
    
    printf("File output tests passed!\n");
}

// Function to test that parse->print->parse results in the same AST as just parsing
void test_parse_print_parse(const char* constant_str) {
    // First parse
    ptx_constant_t* first_constant = NULL;
    assert(parse_constant(constant_str, &first_constant) == true);
    assert(first_constant != NULL);
    
    // Print
    char buffer[BUFFER_SIZE] = {0};
    int printed = print_constant_to_buffer(buffer, BUFFER_SIZE, *first_constant);
    assert(printed > 0);
    
    // Second parse (of the printed output)
    ptx_constant_t* second_constant = NULL;
    assert(parse_constant(buffer, &second_constant) == true);
    assert(second_constant != NULL);
    
    // Compare the two constants
    assert(first_constant->type == second_constant->type);
    
    switch (first_constant->type) {
        case PTX_CONST_INT_SIGNED:
            assert(first_constant->s64_val == second_constant->s64_val);
            break;
        case PTX_CONST_INT_UNSIGNED:
            assert(first_constant->u64_val == second_constant->u64_val);
            break;
        case PTX_CONST_FLOAT:
            {
                double diff = fabs(first_constant->f64_val - second_constant->f64_val);
                double abs_val = fabs(first_constant->f64_val);
                double rel_diff = diff / (abs_val > 1.0 ? abs_val : 1.0);
                assert(rel_diff < FLOAT_EPSILON);
            }
            break;
        case PTX_CONST_FLOAT_SINGLE:
            {
                float diff = fabsf(first_constant->f32_val - second_constant->f32_val);
                float abs_val = fabsf(first_constant->f32_val);
                float rel_diff = diff / (abs_val > 1.0f ? abs_val : 1.0f);
                assert(rel_diff < FLOAT_EPSILON);
            }
            break;
        case PTX_CONST_FLOAT_SCIENTIFIC:
            {
                double diff = fabs(first_constant->f64_val - second_constant->f64_val);
                double abs_val = fabs(first_constant->f64_val);
                double rel_diff = diff / (abs_val > 1.0 ? abs_val : 1.0);
                assert(rel_diff < FLOAT_EPSILON);
            }
            break;
        case PTX_CONST_FLOAT_IEEE_HEX_F32:
            {
                float diff = fabsf(first_constant->f32_val - second_constant->f32_val);
                float abs_val = fabsf(first_constant->f32_val);
                float rel_diff = diff / (abs_val > 1.0f ? abs_val : 1.0f);
                assert(rel_diff < FLOAT_EPSILON);
            }
            break;
        case PTX_CONST_FLOAT_IEEE_HEX_F64:
            {
                double diff = fabs(first_constant->f64_val - second_constant->f64_val);
                double abs_val = fabs(first_constant->f64_val);
                double rel_diff = diff / (abs_val > 1.0 ? abs_val : 1.0);
                assert(rel_diff < FLOAT_EPSILON);
            }
            break;
        case PTX_CONST_PRED:
            assert(first_constant->pred_val == second_constant->pred_val);
            break;
        default:
            assert(false && "Unknown constant type");
    }
    
    printf("Parse->Print->Parse test passed for constant: '%s' -> '%s'\n", 
           constant_str, buffer);
    
    // Free the allocated memory
    free_constant(first_constant);
    free_constant(second_constant);
}

void test_parse_print_parse_roundtrip() {
    printf("Testing parse->print->parse roundtrip...\n");
    
    // Test integers with different formats but same value
    test_parse_print_parse("42");
    test_parse_print_parse("0x2A");  // Hex for 42
    test_parse_print_parse("052");   // Octal for 42
    test_parse_print_parse("0b101010"); // Binary for 42
    
    // Test unsigned integers
    test_parse_print_parse("42U");
    test_parse_print_parse("0xFFU");
    
    // Test large integers (but within 64-bit range)
    test_parse_print_parse("12345678901234");
    test_parse_print_parse("0xABCDEF01"); // Smaller hex value that fits in 64 bits
    
    // Test standard floats
    test_parse_print_parse("3.14159");
    
    // Test scientific notation with different formats
    test_parse_print_parse("3.14159e0");
    test_parse_print_parse("0.314159e1");
    test_parse_print_parse("31.4159e-1");
    test_parse_print_parse("1.23e45");
    test_parse_print_parse("9.87e-6");
    
    // Test IEEE 754 hex formats
    test_parse_print_parse("0F3f800000");  // 1.0 in single-precision
    test_parse_print_parse("0F40490fdb");  // 3.14159 in single-precision
    test_parse_print_parse("0D3ff0000000000000");  // 1.0 in double-precision
    test_parse_print_parse("0D400921fb54442d18");  // 3.14159 in double-precision
    
    printf("Parse->Print->Parse roundtrip tests passed!\n");
}

int main() {
    printf("=== PTX Constant Printer Tests ===\n");
    
    test_basic_constants();
    test_roundtrip_parsing_printing();
    test_parse_print_parse_roundtrip();
    test_file_output();
    
    printf("All constant printer tests passed!\n");
    return 0;
} 