#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ptx_parse/ptx_function_parse.h"
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
 * Test parsing a basic function declaration
 */
bool test_basic_function() {
    // Correct PTX syntax - no return type list before parameter list
    const char* input = ".entry myKernel(.param .u32 param1, .param .f32 param2)";
    size_t consumed = 0;
    ptx_function_t function;
    
    printf("Parsing: %s\n", input);
    bool result = ptx_parse_function_declaration(input, &consumed, &function);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result) {
        return false;
    }
    
    // Check function attributes
    bool check_directive = function.directive == PTX_FUNC_ENTRY;
    bool check_name = strcmp(function.name, "myKernel") == 0;
    
    printf("Function name: %s, directive: %d\n", function.name, function.directive);
    
    // Check parameters
    ptx_parameter_t* param = function.parameters;
    printf("Parameters: %p\n", (void*)param);
    if (param) {
        printf("First parameter: %s, type: %d\n", param->name, param->type);
    }
    if (param && param->next) {
        printf("Second parameter: %s, type: %d\n", param->next->name, param->next->type);
    }
    
    bool check_param1 = param && strcmp(param->name, "param1") == 0 && 
                     param->type == PTX_TYPE_U32;
    bool check_param2 = param && param->next && strcmp(param->next->name, "param2") == 0 && 
                     param->next->type == PTX_TYPE_F32;
    
    printf("Checks: directive=%d, name=%d, param1=%d, param2=%d\n",
           check_directive, check_name, check_param1, check_param2);
    
    // Free allocated memory
    free(function.name);
    
    ptx_parameter_t* current_param = function.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return check_directive && check_name && check_param1 && check_param2;
}

/**
 * Test parsing a function with ptr attribute in parameters
 */
bool test_function_with_ptr_params() {
    const char* input = ".entry computeKernel(.param .u32 param1, .param .u32 .ptr.global.align 16 param2)";
    size_t consumed = 0;
    ptx_function_t function;
    
    printf("Parsing: %s\n", input);
    bool result = ptx_parse_function_declaration(input, &consumed, &function);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result) {
        return false;
    }
    
    // Check function attributes
    bool check_directive = function.directive == PTX_FUNC_ENTRY;
    bool check_name = strcmp(function.name, "computeKernel") == 0;
    
    printf("Function name: %s, directive: %d\n", function.name, function.directive);
    
    // Check parameters
    ptx_parameter_t* param = function.parameters;
    printf("Parameters: %p\n", (void*)param);
    if (param) {
        printf("First parameter: %s, type: %d, attribute: %d\n", 
               param->name, param->type, param->attribute);
    }
    if (param && param->next) {
        printf("Second parameter: %s, type: %d, attribute: %d, has_alignment: %d, alignment: %u\n", 
               param->next->name, param->next->type, param->next->attribute, 
               param->next->has_alignment, param->next->alignment);
    }
    
    bool check_param1 = param && strcmp(param->name, "param1") == 0 && 
                     param->type == PTX_TYPE_U32 &&
                     param->attribute == PTX_PARAM_NONE;
    
    bool check_param2 = param && param->next && 
                     strcmp(param->next->name, "param2") == 0 && 
                     param->next->type == PTX_TYPE_U32 &&
                     param->next->attribute == PTX_PARAM_PTR &&
                     param->next->has_alignment && 
                     param->next->alignment == 16;
    
    printf("Checks: directive=%d, name=%d, param1=%d, param2=%d\n",
           check_directive, check_name, check_param1, check_param2);
    
    // Free allocated memory
    free(function.name);
    
    ptx_parameter_t* current_param = function.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return check_directive && check_name && check_param1 && check_param2;
}

/**
 * Test parsing a function with visible attribute
 */
bool test_function_with_visible() {
    const char* input = ".visible .entry visibleKernel(.param .u32 param1)";
    size_t consumed = 0;
    ptx_function_t function;
    
    printf("Parsing: %s\n", input);
    bool result = ptx_parse_function_declaration(input, &consumed, &function);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result) {
        return false;
    }
    
    // Check function attributes
    bool check_directive = function.directive == PTX_FUNC_ENTRY;
    bool check_name = strcmp(function.name, "visibleKernel") == 0;
    bool check_visible = function.is_visible;
    
    printf("Function name: %s, directive: %d, is_visible: %d\n", 
           function.name, function.directive, function.is_visible);
    
    // Check parameters
    ptx_parameter_t* param = function.parameters;
    printf("Parameters: %p\n", (void*)param);
    if (param) {
        printf("First parameter: %s, type: %d\n", param->name, param->type);
    }
    
    bool check_param = param && strcmp(param->name, "param1") == 0 && 
                     param->type == PTX_TYPE_U32;
    
    printf("Checks: directive=%d, name=%d, visible=%d, param=%d\n",
           check_directive, check_name, check_visible, check_param);
    
    // Free allocated memory
    free(function.name);
    
    ptx_parameter_t* current_param = function.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return check_directive && check_name && check_visible && check_param;
}

int main() {
    TEST("Basic Function Declaration", test_basic_function());
    TEST("Function with PTR Parameters", test_function_with_ptr_params());
    TEST("Function with Visible Attribute", test_function_with_visible());
    
    printf("\nSummary: %d/%d tests passed\n", test_pass, test_count);
    return test_pass == test_count ? 0 : 1;
} 