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
 * Test parsing a basic kernel declaration (without return type)
 */
bool test_basic_kernel() {
    // Correct PTX syntax for kernels - no return type list before parameter list
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
    
    // Kernels should not have return parameters
    bool check_no_returns = function.return_parameters == NULL;
    
    printf("Checks: directive=%d, name=%d, param1=%d, param2=%d, no_returns=%d\n",
           check_directive, check_name, check_param1, check_param2, check_no_returns);
    
    // Free allocated memory
    free(function.name);
    
    ptx_parameter_t* current_param = function.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return check_directive && check_name && check_param1 && check_param2 && check_no_returns;
}

/**
 * Test parsing a kernel with ptr attribute in parameters
 */
bool test_kernel_with_ptr_params() {
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
    
    // Kernels should not have return parameters
    bool check_no_returns = function.return_parameters == NULL;
    
    printf("Checks: directive=%d, name=%d, param1=%d, param2=%d, no_returns=%d\n",
           check_directive, check_name, check_param1, check_param2, check_no_returns);
    
    // Free allocated memory
    free(function.name);
    
    ptx_parameter_t* current_param = function.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return check_directive && check_name && check_param1 && check_param2 && check_no_returns;
}

/**
 * Test parsing a kernel with visible attribute
 */
bool test_kernel_with_visible() {
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
    
    // Kernels should not have return parameters
    bool check_no_returns = function.return_parameters == NULL;
    
    printf("Checks: directive=%d, name=%d, visible=%d, param=%d, no_returns=%d\n",
           check_directive, check_name, check_visible, check_param, check_no_returns);
    
    // Free allocated memory
    free(function.name);
    
    ptx_parameter_t* current_param = function.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return check_directive && check_name && check_visible && check_param && check_no_returns;
}

/**
 * Test parsing a device function with return type list
 */
bool test_device_function_with_return() {
    // Proper function syntax with return parameter list before input parameter list
    const char* input = ".func (.reg .u32 result) deviceFunction(.param .f32 input1, .param .f32 input2)";
    size_t consumed = 0;
    ptx_function_t function;
    
    printf("Parsing: %s\n", input);
    bool result = ptx_parse_function_declaration(input, &consumed, &function);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result) {
        return false;
    }
    
    // Check function attributes
    bool check_directive = function.directive == PTX_FUNC_FUNC;
    bool check_name = strcmp(function.name, "deviceFunction") == 0;
    
    printf("Function name: %s, directive: %d\n", function.name, function.directive);
    
    // Check return parameters
    ptx_parameter_t* ret_param = function.return_parameters;
    printf("Return parameters: %p\n", (void*)ret_param);
    if (ret_param) {
        printf("Return parameter: %s, type: %d, state space: %d\n", 
               ret_param->name, ret_param->type, ret_param->state_space);
    }
    
    bool check_return = ret_param && strcmp(ret_param->name, "result") == 0 && 
                     ret_param->type == PTX_TYPE_U32 &&
                     ret_param->state_space == PTX_STATE_REG;
    
    // Check input parameters
    ptx_parameter_t* param = function.parameters;
    printf("Input parameters: %p\n", (void*)param);
    if (param) {
        printf("First input parameter: %s, type: %d, state space: %d\n", 
               param->name, param->type, param->state_space);
    }
    if (param && param->next) {
        printf("Second input parameter: %s, type: %d, state space: %d\n", 
               param->next->name, param->next->type, param->next->state_space);
    }
    
    bool check_param1 = param && strcmp(param->name, "input1") == 0 && 
                     param->type == PTX_TYPE_F32 &&
                     param->state_space == PTX_STATE_PARAM;
    bool check_param2 = param && param->next && strcmp(param->next->name, "input2") == 0 && 
                     param->next->type == PTX_TYPE_F32 &&
                     param->next->state_space == PTX_STATE_PARAM;
    
    printf("Checks: directive=%d, name=%d, return=%d, param1=%d, param2=%d\n",
           check_directive, check_name, check_return, check_param1, check_param2);
    
    // Free allocated memory
    free(function.name);
    
    // Free return parameters
    ptx_parameter_t* current_ret = function.return_parameters;
    while (current_ret) {
        ptx_parameter_t* next = current_ret->next;
        free(current_ret->name);
        free(current_ret);
        current_ret = next;
    }
    
    // Free input parameters
    ptx_parameter_t* current_param = function.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return check_directive && check_name && check_return && check_param1 && check_param2;
}

/**
 * Test parsing a device function with multiple return values
 */
bool test_device_function_with_multiple_returns() {
    // Function with multiple return values
    const char* input = ".func (.reg .u32 result1, .reg .f32 result2) complexFunction(.param .u32 input)";
    size_t consumed = 0;
    ptx_function_t function;
    
    printf("Parsing: %s\n", input);
    bool result = ptx_parse_function_declaration(input, &consumed, &function);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result) {
        return false;
    }
    
    // Check function attributes
    bool check_directive = function.directive == PTX_FUNC_FUNC;
    bool check_name = strcmp(function.name, "complexFunction") == 0;
    
    printf("Function name: %s, directive: %d\n", function.name, function.directive);
    
    // Check return parameters
    ptx_parameter_t* ret_param = function.return_parameters;
    printf("Return parameters: %p\n", (void*)ret_param);
    if (ret_param) {
        printf("First return parameter: %s, type: %d, state space: %d\n", 
               ret_param->name, ret_param->type, ret_param->state_space);
    }
    if (ret_param && ret_param->next) {
        printf("Second return parameter: %s, type: %d, state space: %d\n", 
              ret_param->next->name, ret_param->next->type, ret_param->next->state_space);
    }
    
    bool check_return1 = ret_param && strcmp(ret_param->name, "result1") == 0 && 
                     ret_param->type == PTX_TYPE_U32 &&
                     ret_param->state_space == PTX_STATE_REG;
    bool check_return2 = ret_param && ret_param->next && 
                     strcmp(ret_param->next->name, "result2") == 0 && 
                     ret_param->next->type == PTX_TYPE_F32 &&
                     ret_param->next->state_space == PTX_STATE_REG;
    
    // Check input parameters
    ptx_parameter_t* param = function.parameters;
    printf("Input parameters: %p\n", (void*)param);
    if (param) {
        printf("Input parameter: %s, type: %d, state space: %d\n", 
               param->name, param->type, param->state_space);
    }
    
    bool check_param = param && strcmp(param->name, "input") == 0 && 
                     param->type == PTX_TYPE_U32 &&
                     param->state_space == PTX_STATE_PARAM;
    
    printf("Checks: directive=%d, name=%d, return1=%d, return2=%d, param=%d\n",
           check_directive, check_name, check_return1, check_return2, check_param);
    
    // Free allocated memory
    free(function.name);
    
    // Free return parameters
    ptx_parameter_t* current_ret = function.return_parameters;
    while (current_ret) {
        ptx_parameter_t* next = current_ret->next;
        free(current_ret->name);
        free(current_ret);
        current_ret = next;
    }
    
    // Free input parameters
    ptx_parameter_t* current_param = function.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return check_directive && check_name && check_return1 && check_return2 && check_param;
}

/**
 * Test parsing a device function with .reg input parameters
 */
bool test_device_function_with_reg_inputs() {
    // Function with .reg input parameters
    const char* input = ".func (.reg .u32 result) regInputFunction(.reg .b32 input1, .reg .f32 input2)";
    size_t consumed = 0;
    ptx_function_t function;
    
    printf("Parsing: %s\n", input);
    bool result = ptx_parse_function_declaration(input, &consumed, &function);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result) {
        return false;
    }
    
    // Check function attributes
    bool check_directive = function.directive == PTX_FUNC_FUNC;
    bool check_name = strcmp(function.name, "regInputFunction") == 0;
    
    printf("Function name: %s, directive: %d\n", function.name, function.directive);
    
    // Check return parameters
    ptx_parameter_t* ret_param = function.return_parameters;
    printf("Return parameters: %p\n", (void*)ret_param);
    if (ret_param) {
        printf("Return parameter: %s, type: %d, state space: %d\n", 
               ret_param->name, ret_param->type, ret_param->state_space);
    }
    
    bool check_return = ret_param && strcmp(ret_param->name, "result") == 0 && 
                     ret_param->type == PTX_TYPE_U32 &&
                     ret_param->state_space == PTX_STATE_REG;
    
    // Check input parameters
    ptx_parameter_t* param = function.parameters;
    printf("Input parameters: %p\n", (void*)param);
    if (param) {
        printf("First input parameter: %s, type: %d, state space: %d\n", 
               param->name, param->type, param->state_space);
    }
    if (param && param->next) {
        printf("Second input parameter: %s, type: %d, state space: %d\n", 
               param->next->name, param->next->type, param->next->state_space);
    }
    
    bool check_param1 = param && strcmp(param->name, "input1") == 0 && 
                     param->type == PTX_TYPE_B32 &&
                     param->state_space == PTX_STATE_REG;
    bool check_param2 = param && param->next && strcmp(param->next->name, "input2") == 0 && 
                     param->next->type == PTX_TYPE_F32 &&
                     param->next->state_space == PTX_STATE_REG;
    
    printf("Checks: directive=%d, name=%d, return=%d, param1=%d, param2=%d\n",
           check_directive, check_name, check_return, check_param1, check_param2);
    
    // Free allocated memory
    free(function.name);
    
    // Free return parameters
    ptx_parameter_t* current_ret = function.return_parameters;
    while (current_ret) {
        ptx_parameter_t* next = current_ret->next;
        free(current_ret->name);
        free(current_ret);
        current_ret = next;
    }
    
    // Free input parameters
    ptx_parameter_t* current_param = function.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return check_directive && check_name && check_return && check_param1 && check_param2;
}

/**
 * Test parsing a device function with mixed .reg and .param inputs
 */
bool test_device_function_with_mixed_inputs() {
    // Function with both .reg and .param input parameters
    const char* input = ".func (.reg .u32 result) mixedInputFunction(.reg .b32 input1, .param .align 8 .b8 buffer[12])";
    size_t consumed = 0;
    ptx_function_t function;
    
    printf("Parsing: %s\n", input);
    bool result = ptx_parse_function_declaration(input, &consumed, &function);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result) {
        return false;
    }
    
    // Check function attributes
    bool check_directive = function.directive == PTX_FUNC_FUNC;
    bool check_name = strcmp(function.name, "mixedInputFunction") == 0;
    
    printf("Function name: %s, directive: %d\n", function.name, function.directive);
    
    // Check return parameters
    ptx_parameter_t* ret_param = function.return_parameters;
    printf("Return parameters: %p\n", (void*)ret_param);
    if (ret_param) {
        printf("Return parameter: %s, type: %d, state space: %d\n", 
               ret_param->name, ret_param->type, ret_param->state_space);
    }
    
    bool check_return = ret_param && strcmp(ret_param->name, "result") == 0 && 
                     ret_param->type == PTX_TYPE_U32 &&
                     ret_param->state_space == PTX_STATE_REG;
    
    // Check input parameters
    ptx_parameter_t* param = function.parameters;
    printf("Input parameters: %p\n", (void*)param);
    if (param) {
        printf("First input parameter: %s, type: %d, state space: %d\n", 
               param->name, param->type, param->state_space);
    }
    if (param && param->next) {
        printf("Second input parameter: %s, type: %d, state space: %d, has_alignment: %d, alignment: %u\n", 
               param->next->name, param->next->type, param->next->state_space,
               param->next->has_alignment, param->next->alignment);
    }
    
    bool check_param1 = param && strcmp(param->name, "input1") == 0 && 
                     param->type == PTX_TYPE_B32 &&
                     param->state_space == PTX_STATE_REG;
    bool check_param2 = param && param->next && strcmp(param->next->name, "buffer") == 0 && 
                     param->next->type == PTX_TYPE_B8 &&
                     param->next->state_space == PTX_STATE_PARAM &&
                     param->next->has_alignment &&
                     param->next->alignment == 8;
    
    printf("Checks: directive=%d, name=%d, return=%d, param1=%d, param2=%d\n",
           check_directive, check_name, check_return, check_param1, check_param2);
    
    // Free allocated memory
    free(function.name);
    
    // Free return parameters
    ptx_parameter_t* current_ret = function.return_parameters;
    while (current_ret) {
        ptx_parameter_t* next = current_ret->next;
        free(current_ret->name);
        free(current_ret);
        current_ret = next;
    }
    
    // Free input parameters
    ptx_parameter_t* current_param = function.parameters;
    while (current_param) {
        ptx_parameter_t* next = current_param->next;
        free(current_param->name);
        free(current_param);
        current_param = next;
    }
    
    return check_directive && check_name && check_return && check_param1 && check_param2;
}

int main() {
    TEST("Basic Kernel Declaration", test_basic_kernel());
    TEST("Kernel with PTR Parameters", test_kernel_with_ptr_params());
    TEST("Kernel with Visible Attribute", test_kernel_with_visible());
    TEST("Device Function with Return Type", test_device_function_with_return());
    TEST("Device Function with Multiple Returns", test_device_function_with_multiple_returns());
    TEST("Device Function with .reg Inputs", test_device_function_with_reg_inputs());
    TEST("Device Function with Mixed Inputs", test_device_function_with_mixed_inputs());
    
    printf("\nSummary: %d/%d tests passed\n", test_pass, test_count);
    return test_pass == test_count ? 0 : 1;
} 