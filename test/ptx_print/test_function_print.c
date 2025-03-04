#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "ptx_parse/ptx_function_parse.h"
#include "ptx_print/ptx_function_print.h"

/**
 * Test the parse->print->parse cycle for a kernel function
 */
bool test_kernel_print_cycle() {
    printf("=== Testing kernel print cycle ===\n");
    const char* input = ".entry kernelFunction(.param .u32 param1, .param .f32 param2)";
    size_t consumed = 0;
    ptx_function_t function1, function2;
    char buffer[1024] = {0};
    FILE* memfile = NULL;
    
    printf("Original input: %s\n", input);
    
    // First parse
    bool result = ptx_parse_function_declaration(input, &consumed, &function1);
    if (!result) {
        printf("First parse failed\n");
        return false;
    }
    printf("First parse succeeded, consumed: %zu\n", consumed);
    
    // Print to memory buffer
    memfile = fmemopen(buffer, sizeof(buffer), "w");
    if (!memfile) {
        printf("Failed to open memory file\n");
        return false;
    }
    
    result = ptx_print_function(memfile, &function1);
    fclose(memfile);
    
    if (!result) {
        printf("Print failed\n");
        return false;
    }
    printf("Printed output: %s\n", buffer);
    
    // Second parse
    consumed = 0;
    result = ptx_parse_function_declaration(buffer, &consumed, &function2);
    if (!result) {
        printf("Second parse failed\n");
        return false;
    }
    printf("Second parse succeeded, consumed: %zu\n", consumed);
    
    // Compare results
    bool same_directive = function1.directive == function2.directive;
    bool same_name = strcmp(function1.name, function2.name) == 0;
    bool same_visibility = function1.is_visible == function2.is_visible;
    bool same_extern = function1.is_extern == function2.is_extern;
    
    // Count parameters
    int param_count1 = 0;
    int param_count2 = 0;
    ptx_parameter_t* param1 = function1.parameters;
    ptx_parameter_t* param2 = function2.parameters;
    
    while (param1) {
        param_count1++;
        param1 = param1->next;
    }
    
    while (param2) {
        param_count2++;
        param2 = param2->next;
    }
    
    bool same_param_count = param_count1 == param_count2;
    
    printf("Same directive: %s\n", same_directive ? "yes" : "no");
    printf("Same name: %s\n", same_name ? "yes" : "no");
    printf("Same visibility: %s\n", same_visibility ? "yes" : "no");
    printf("Same extern: %s\n", same_extern ? "yes" : "no");
    printf("Same parameter count: %s (%d vs %d)\n", 
           same_param_count ? "yes" : "no", param_count1, param_count2);
    
    // Free resources
    free(function1.name);
    free(function2.name);
    
    ptx_parameter_t* current_param = function1.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    current_param = function2.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return same_directive && same_name && same_visibility && 
           same_extern && same_param_count;
}

/**
 * Test the parse->print->parse cycle for a device function with return parameters
 */
bool test_device_function_print_cycle() {
    printf("=== Testing device function print cycle ===\n");
    const char* input = ".func (.reg .u32 result) deviceFunction(.param .f32 input1, .param .f32 input2)";
    size_t consumed = 0;
    ptx_function_t function1, function2;
    char buffer[1024] = {0};
    FILE* memfile = NULL;
    
    printf("Original input: %s\n", input);
    
    // First parse
    bool result = ptx_parse_function_declaration(input, &consumed, &function1);
    if (!result) {
        printf("First parse failed\n");
        return false;
    }
    printf("First parse succeeded, consumed: %zu\n", consumed);
    
    // Print to memory buffer
    memfile = fmemopen(buffer, sizeof(buffer), "w");
    if (!memfile) {
        printf("Failed to open memory file\n");
        return false;
    }
    
    result = ptx_print_function(memfile, &function1);
    fclose(memfile);
    
    if (!result) {
        printf("Print failed\n");
        return false;
    }
    printf("Printed output: %s\n", buffer);
    
    // Second parse
    consumed = 0;
    result = ptx_parse_function_declaration(buffer, &consumed, &function2);
    if (!result) {
        printf("Second parse failed\n");
        return false;
    }
    printf("Second parse succeeded, consumed: %zu\n", consumed);
    
    // Compare results
    bool same_directive = function1.directive == function2.directive;
    bool same_name = strcmp(function1.name, function2.name) == 0;
    
    // Count input parameters
    int param_count1 = 0;
    int param_count2 = 0;
    ptx_parameter_t* param1 = function1.parameters;
    ptx_parameter_t* param2 = function2.parameters;
    
    while (param1) {
        param_count1++;
        param1 = param1->next;
    }
    
    while (param2) {
        param_count2++;
        param2 = param2->next;
    }
    
    bool same_param_count = param_count1 == param_count2;
    
    // Count return parameters
    int ret_param_count1 = 0;
    int ret_param_count2 = 0;
    param1 = function1.return_parameters;
    param2 = function2.return_parameters;
    
    while (param1) {
        ret_param_count1++;
        param1 = param1->next;
    }
    
    while (param2) {
        ret_param_count2++;
        param2 = param2->next;
    }
    
    bool same_ret_param_count = ret_param_count1 == ret_param_count2;
    
    printf("Same directive: %s\n", same_directive ? "yes" : "no");
    printf("Same name: %s\n", same_name ? "yes" : "no");
    printf("Same parameter count: %s (%d vs %d)\n", 
           same_param_count ? "yes" : "no", param_count1, param_count2);
    printf("Same return parameter count: %s (%d vs %d)\n", 
           same_ret_param_count ? "yes" : "no", ret_param_count1, ret_param_count2);
    
    // Free resources
    free(function1.name);
    free(function2.name);
    
    ptx_parameter_t* current_param = function1.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    current_param = function2.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    current_param = function1.return_parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    current_param = function2.return_parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return same_directive && same_name && same_param_count && same_ret_param_count;
}

/**
 * Test the parse->print->parse cycle for a visible device function
 */
bool test_visible_function_print_cycle() {
    printf("=== Testing visible function print cycle ===\n");
    const char* input = ".visible .func deviceFunction(.param .u32 input)";
    size_t consumed = 0;
    ptx_function_t function1, function2;
    char buffer[1024] = {0};
    FILE* memfile = NULL;
    
    printf("Original input: %s\n", input);
    
    // First parse
    bool result = ptx_parse_function_declaration(input, &consumed, &function1);
    if (!result) {
        printf("First parse failed\n");
        return false;
    }
    printf("First parse succeeded, consumed: %zu\n", consumed);
    
    // Print to memory buffer
    memfile = fmemopen(buffer, sizeof(buffer), "w");
    if (!memfile) {
        printf("Failed to open memory file\n");
        return false;
    }
    
    result = ptx_print_function(memfile, &function1);
    fclose(memfile);
    
    if (!result) {
        printf("Print failed\n");
        return false;
    }
    printf("Printed output: %s\n", buffer);
    
    // Second parse
    consumed = 0;
    result = ptx_parse_function_declaration(buffer, &consumed, &function2);
    if (!result) {
        printf("Second parse failed\n");
        return false;
    }
    printf("Second parse succeeded, consumed: %zu\n", consumed);
    
    // Compare results
    bool same_directive = function1.directive == function2.directive;
    bool same_name = strcmp(function1.name, function2.name) == 0;
    bool same_visibility = function1.is_visible == function2.is_visible;
    
    printf("Same directive: %s\n", same_directive ? "yes" : "no");
    printf("Same name: %s\n", same_name ? "yes" : "no");
    printf("Same visibility: %s\n", same_visibility ? "yes" : "no");
    printf("Is visible: %s\n", function1.is_visible ? "yes" : "no");
    
    // Free resources
    free(function1.name);
    free(function2.name);
    
    ptx_parameter_t* current_param = function1.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    current_param = function2.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return same_directive && same_name && same_visibility;
}

/**
 * Test the parse->print->parse cycle for a kernel with aligned pointer parameters
 */
bool test_kernel_with_aligned_ptr_print_cycle() {
    printf("=== Testing kernel with aligned pointer print cycle ===\n");
    const char* input = ".entry computeKernel(.param .u32 param1, .param .u32 .ptr.global.align 16 param2)";
    size_t consumed = 0;
    ptx_function_t function1, function2;
    char buffer[1024] = {0};
    FILE* memfile = NULL;
    
    printf("Original input: %s\n", input);
    
    // First parse
    bool result = ptx_parse_function_declaration(input, &consumed, &function1);
    if (!result) {
        printf("First parse failed\n");
        return false;
    }
    printf("First parse succeeded, consumed: %zu\n", consumed);
    
    // Print debug info for the second parameter
    ptx_parameter_t* second_param = function1.parameters ? function1.parameters->next : NULL;
    if (second_param) {
        printf("DEBUG: Second param - state_space: %d, attribute: %d, has_alignment: %d, alignment: %u\n",
               second_param->state_space, second_param->attribute, 
               second_param->has_alignment, second_param->alignment);
    }
    
    // Print to memory buffer
    memfile = fmemopen(buffer, sizeof(buffer), "w");
    if (!memfile) {
        printf("Failed to open memory file\n");
        return false;
    }
    
    result = ptx_print_function(memfile, &function1);
    fclose(memfile);
    
    if (!result) {
        printf("Print failed\n");
        return false;
    }
    printf("Printed output: %s\n", buffer);
    
    // Second parse
    consumed = 0;
    result = ptx_parse_function_declaration(buffer, &consumed, &function2);
    if (!result) {
        printf("Second parse failed\n");
        return false;
    }
    printf("Second parse succeeded, consumed: %zu\n", consumed);
    
    // Compare results
    bool same_directive = function1.directive == function2.directive;
    bool same_name = strcmp(function1.name, function2.name) == 0;
    
    // Count parameters
    int param_count1 = 0;
    int param_count2 = 0;
    ptx_parameter_t* param1 = function1.parameters;
    ptx_parameter_t* param2 = function2.parameters;
    
    while (param1) {
        param_count1++;
        param1 = param1->next;
    }
    
    while (param2) {
        param_count2++;
        param2 = param2->next;
    }
    
    bool same_param_count = param_count1 == param_count2;
    
    // Check if second parameter has the alignment attribute
    bool has_alignment1 = false;
    bool has_alignment2 = false;
    unsigned int alignment1 = 0;
    unsigned int alignment2 = 0;
    
    param1 = function1.parameters;
    if (param1) param1 = param1->next; // Move to second parameter
    if (param1) {
        has_alignment1 = param1->has_alignment;
        alignment1 = param1->alignment;
    }
    
    param2 = function2.parameters;
    if (param2) param2 = param2->next; // Move to second parameter
    if (param2) {
        has_alignment2 = param2->has_alignment;
        alignment2 = param2->alignment;
    }
    
    bool same_alignment = has_alignment1 == has_alignment2 && alignment1 == alignment2;
    
    printf("Same directive: %s\n", same_directive ? "yes" : "no");
    printf("Same name: %s\n", same_name ? "yes" : "no");
    printf("Same parameter count: %s (%d vs %d)\n", 
           same_param_count ? "yes" : "no", param_count1, param_count2);
    printf("Same alignment on ptr param: %s (%u vs %u)\n", 
           same_alignment ? "yes" : "no", alignment1, alignment2);
    
    // Free resources
    free(function1.name);
    free(function2.name);
    
    ptx_parameter_t* current_param = function1.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    current_param = function2.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return same_directive && same_name && same_param_count && same_alignment;
}

/**
 * Test the parse->print->parse cycle for a function with array parameters
 */
bool test_function_with_array_params_print_cycle() {
    printf("=== Testing function with array parameters print cycle ===\n");
    // Simplified array parameter syntax
    const char* input = ".func foo(.reg .b32 N, .param .b8 buffer[12])";
    size_t consumed = 0;
    ptx_function_t function1, function2;
    char buffer[1024] = {0};
    FILE* memfile = NULL;
    
    printf("Original input: %s\n", input);
    
    // First parse
    bool result = ptx_parse_function_declaration(input, &consumed, &function1);
    if (!result) {
        printf("First parse failed\n");
        return false;
    }
    printf("First parse succeeded, consumed: %zu\n", consumed);
    
    // Print to memory buffer
    memfile = fmemopen(buffer, sizeof(buffer), "w");
    if (!memfile) {
        printf("Failed to open memory file\n");
        return false;
    }
    
    result = ptx_print_function(memfile, &function1);
    fclose(memfile);
    
    if (!result) {
        printf("Print failed\n");
        return false;
    }
    printf("Printed output: %s\n", buffer);
    
    // Second parse
    consumed = 0;
    result = ptx_parse_function_declaration(buffer, &consumed, &function2);
    if (!result) {
        printf("Second parse failed\n");
        return false;
    }
    printf("Second parse succeeded, consumed: %zu\n", consumed);
    
    // Compare results
    bool same_directive = function1.directive == function2.directive;
    bool same_name = strcmp(function1.name, function2.name) == 0;
    
    // Count parameters
    int param_count1 = 0;
    int param_count2 = 0;
    ptx_parameter_t* param1 = function1.parameters;
    ptx_parameter_t* param2 = function2.parameters;
    
    while (param1) {
        param_count1++;
        param1 = param1->next;
    }
    
    while (param2) {
        param_count2++;
        param2 = param2->next;
    }
    
    bool same_param_count = param_count1 == param_count2;
    
    // Check if second parameter has array dimensions
    bool has_array_dims1 = false;
    bool has_array_dims2 = false;
    
    param1 = function1.parameters;
    if (param1) param1 = param1->next; // Move to second parameter
    if (param1) {
        has_array_dims1 = param1->has_array_dims;
    }
    
    param2 = function2.parameters;
    if (param2) param2 = param2->next; // Move to second parameter
    if (param2) {
        has_array_dims2 = param2->has_array_dims;
    }
    
    bool same_array_dims = has_array_dims1 == has_array_dims2;
    
    printf("Same directive: %s\n", same_directive ? "yes" : "no");
    printf("Same name: %s\n", same_name ? "yes" : "no");
    printf("Same parameter count: %s (%d vs %d)\n", 
           same_param_count ? "yes" : "no", param_count1, param_count2);
    printf("Both have array dimensions: %s\n", same_array_dims ? "yes" : "no");
    
    // Free resources
    free(function1.name);
    free(function2.name);
    
    ptx_parameter_t* current_param = function1.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    current_param = function2.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return same_directive && same_name && same_param_count && same_array_dims;
}

int main() {
    bool all_tests_pass = true;
    
    // Run tests
    all_tests_pass &= test_kernel_print_cycle();
    all_tests_pass &= test_device_function_print_cycle();
    all_tests_pass &= test_visible_function_print_cycle();
    all_tests_pass &= test_kernel_with_aligned_ptr_print_cycle();
    all_tests_pass &= test_function_with_array_params_print_cycle();
    
    // Print summary
    printf("\n=== Test Summary ===\n");
    printf("Result: %s\n", all_tests_pass ? "PASS" : "FAIL");
    
    return all_tests_pass ? 0 : 1;
} 