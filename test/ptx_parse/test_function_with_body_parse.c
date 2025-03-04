#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ptx_parse/ptx_function_parse.h"
#include "ptx_print/ptx_function_print.h"
#include "prelude/prelude.h"

static int test_count = 0;
static int test_pass = 0;

#define TEST(name, code) \
    do { \
        printf("Test %d: %s - ", ++test_count, name); \
        if (code) { \
            printf("PASS\n"); \
            test_pass++; \
        } else { \
            printf("FAIL\n"); \
        } \
    } while (0)

/**
 * Test parsing a simple function with a body
 */
bool test_simple_function_with_body() {
    const char* input = ".entry simpleKernel(.param .u32 param1)\n"
                        "{\n"
                        "    mov.u32 %r1, %tid.x;\n"
                        "    add.u32 %r2, %r1, 1;\n"
                        "    ret;\n"
                        "}";
    
    size_t consumed = 0;
    ptx_function_t* function = NULL;
    
    printf("Parsing: %s\n", input);
    bool result = ptx_parse_function(input, &consumed, &function);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result || !function) {
        return false;
    }
    
    // Check function attributes
    bool check_directive = function->directive == PTX_FUNC_ENTRY;
    bool check_name = strcmp(function->name, "simpleKernel") == 0;
    
    printf("Function name: %s, directive: %d\n", function->name, function->directive);
    
    // Check parameters
    ptx_parameter_t* param = function->parameters;
    printf("Parameters: %p\n", (void*)param);
    if (param) {
        printf("Parameter: %s, type: %d\n", param->name, param->type);
    }
    
    bool check_param = param && strcmp(param->name, "param1") == 0 && 
                     param->type == PTX_TYPE_U32;
    
    // Check the body
    printf("Body: %p\n", (void*)function->body);
    bool check_body = function->body != NULL;
    
    if (function->body) {
        printf("Number of statements in body: %zu\n", function->body->num_statements);
        check_body = check_body && function->body->num_statements == 3;
    }
    
    // Free the function
    ptx_function_free(function);
    
    return check_directive && check_name && check_param && check_body;
}

/**
 * Test round-trip parsing and printing of a function with body
 */
bool test_function_with_body_round_trip() {
    const char* input = ".entry roundTripKernel(.param .u32 param1)\n"
                        "{\n"
                        "    mov.u32 %r1, %tid.x;\n"
                        "    add.u32 %r2, %r1, 1;\n"
                        "    ret;\n"
                        "}";
    
    // First parse
    size_t consumed = 0;
    ptx_function_t* function = NULL;
    bool result = ptx_parse_function(input, &consumed, &function);
    
    if (!result || !function) {
        return false;
    }
    
    // Print to a buffer
    char buffer[2048];
    FILE* memfile = fmemopen(buffer, sizeof(buffer), "w");
    if (!memfile) {
        ptx_function_free(function);
        return false;
    }
    
    bool print_result = ptx_print_function(memfile, function);
    fclose(memfile);
    
    if (!print_result) {
        ptx_function_free(function);
        return false;
    }
    
    printf("Printed function:\n%s\n", buffer);
    
    // Parse the printed function again
    ptx_function_t* function2 = NULL;
    size_t consumed2 = 0;
    bool result2 = ptx_parse_function(buffer, &consumed2, &function2);
    
    if (!result2 || !function2) {
        ptx_function_free(function);
        return false;
    }
    
    // Check that the function attributes are the same
    bool check_directive = function->directive == function2->directive;
    bool check_name = strcmp(function->name, function2->name) == 0;
    
    // Check that the body has the same number of statements
    bool check_body = function->body && function2->body && 
                    function->body->num_statements == function2->body->num_statements;
    
    printf("Checks: directive=%d, name=%d, body=%d\n",
           check_directive, check_name, check_body);
    
    // Free the functions
    ptx_function_free(function);
    ptx_function_free(function2);
    
    return check_directive && check_name && check_body;
}

int main() {
    TEST("Simple function with body", test_simple_function_with_body());
    TEST("Function with body round-trip", test_function_with_body_round_trip());
    
    printf("\nTests: %d/%d passed\n", test_pass, test_count);
    return test_pass == test_count ? 0 : 1;
} 