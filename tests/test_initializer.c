/**
 * @file test_initializer.c
 * @brief Test program for PTX initializer helper functions
 * 
 * This test demonstrates the usage of ptx_initializer_helpers.h and ptx_initializer_value_helpers.h
 * to create and manipulate various types of PTX initializers.
 */

#include "ast/ptx_initializer_helpers.h"
#include "ast/ptx_initializer_value_helpers.h"
#include "ast/ptx_type_decl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration
static void free_example_type_decl(ptx_type_decl_t* decl);

/** 
 * @brief Create an example of a global 2D array of 32-bit signed integers with initializer
 * 
 * This represents the PTX declaration: .global .s32 offset[][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
 * 
 * @return ptx_type_decl_t* A dynamically allocated type declaration (caller must free)
 */
static ptx_type_decl_t* create_example_global_s32_array(void) {
    // Allocate memory for the type declaration
    ptx_type_decl_t* decl = (ptx_type_decl_t*)malloc(sizeof(ptx_type_decl_t));
    if (decl == NULL) {
        return NULL;
    }
    
    // Set basic properties
    decl->type = PTX_TYPE_S32;
    decl->statespace = PTX_STATE_GLOBAL;
    decl->shape.kind = SHAPE_ARRAY;
    decl->shape.array.size = ARRAY_SIZE_2D;
    decl->shape.array.sizes[0] = 4;  // 4 rows
    decl->shape.array.sizes[1] = 2;  // 2 columns
    
    // Set name
    decl->name = strdup("offset");
    
    // Create a 2D array shape for the initializer
    ptx_array_shape_t shape = ptx_array_shape_create(2, 4, 2);
    
    // Create the array initializer
    decl->initializer = ptx_initializer_create_array(shape);
    
    // Clean up the shape since it's been copied
    ptx_array_shape_free(shape);
    
    // Set values for {-1, 0}
    ptx_initializer_value_t value;
    size_t indices[2];
    
    // Using helper functions for creating values
    indices[0] = 0;
    indices[1] = 0;
    value = ptx_initializer_value_int(-1);
    ptx_initializer_set_value(&decl->initializer, indices, &value);
    
    indices[0] = 0;
    indices[1] = 1;
    value = ptx_initializer_value_int(0);
    ptx_initializer_set_value(&decl->initializer, indices, &value);
    
    // Set values for {0, -1}
    indices[0] = 1;
    indices[1] = 0;
    value = ptx_initializer_value_int(0);
    ptx_initializer_set_value(&decl->initializer, indices, &value);
    
    indices[0] = 1;
    indices[1] = 1;
    value = ptx_initializer_value_int(-1);
    ptx_initializer_set_value(&decl->initializer, indices, &value);
    
    // Set values for {1, 0}
    indices[0] = 2;
    indices[1] = 0;
    value = ptx_initializer_value_int(1);
    ptx_initializer_set_value(&decl->initializer, indices, &value);
    
    indices[0] = 2;
    indices[1] = 1;
    value = ptx_initializer_value_int(0);
    ptx_initializer_set_value(&decl->initializer, indices, &value);
    
    // Set values for {0, 1}
    indices[0] = 3;
    indices[1] = 0;
    value = ptx_initializer_value_int(0);
    ptx_initializer_set_value(&decl->initializer, indices, &value);
    
    indices[0] = 3;
    indices[1] = 1;
    value = ptx_initializer_value_int(1);
    ptx_initializer_set_value(&decl->initializer, indices, &value);
    
    return decl;
}

/**
 * @brief Create an example of a global vector of mixed values
 * 
 * This represents the PTX declaration: .global .v4 .u32 mixed = {10, bar, add_func, 0};
 * 
 * @return ptx_type_decl_t* A dynamically allocated type declaration (caller must free)
 */
static ptx_type_decl_t* create_example_global_v4_u32_mixed(void) {
    // Allocate memory for the type declaration
    ptx_type_decl_t* decl = (ptx_type_decl_t*)malloc(sizeof(ptx_type_decl_t));
    if (decl == NULL) {
        return NULL;
    }
    
    // Set basic properties
    decl->type = PTX_TYPE_U32;
    decl->statespace = PTX_STATE_GLOBAL;
    decl->shape.kind = SHAPE_VECTOR;
    decl->shape.vector_size = VECTOR_SIZE_V4;
    
    // Set name
    decl->name = strdup("mixed");
    
    // Create vector initializer (with size 4)
    decl->initializer = ptx_initializer_create_vector(VECTOR_SIZE_V4, NULL);
    
    // Set values using the helper functions
    ptx_initializer_value_t value;
    size_t index[1];
    
    // Set value for {10}
    index[0] = 0;
    value = ptx_initializer_value_int(10);
    ptx_initializer_set_value(&decl->initializer, index, &value);
    
    // Set value for {bar}
    index[0] = 1;
    value = ptx_initializer_value_var_addr("bar", 0, false);
    ptx_initializer_set_value(&decl->initializer, index, &value);
    
    // Set value for {add_func}
    index[0] = 2;
    value = ptx_initializer_value_func_addr("add_func");
    ptx_initializer_set_value(&decl->initializer, index, &value);
    
    // Set value for {0}
    index[0] = 3;
    value = ptx_initializer_value_int(0);
    ptx_initializer_set_value(&decl->initializer, index, &value);
    
    return decl;
}

/**
 * @brief Create an example of masked address and masked integer initializers
 * 
 * This represents the PTX declarations:
 * .global .u8 masked_byte = mask(foo, 0xFF);  // Extract byte 0 of the address of foo
 * .global .u8 masked_int = mask(0x12345678, 0xFF00);  // Extract byte 1 of the integer
 * 
 * @return ptx_type_decl_t** Array of two dynamically allocated type declarations (caller must free each)
 */
static ptx_type_decl_t** create_example_masked_initializers(void) {
    // Allocate memory for an array of two type declarations
    ptx_type_decl_t** decls = (ptx_type_decl_t**)malloc(2 * sizeof(ptx_type_decl_t*));
    if (decls == NULL) {
        return NULL;
    }
    
    // Create the first declaration: masked variable address
    decls[0] = (ptx_type_decl_t*)malloc(sizeof(ptx_type_decl_t));
    if (decls[0] == NULL) {
        free(decls);
        return NULL;
    }
    
    // Set basic properties
    decls[0]->type = PTX_TYPE_U8;
    decls[0]->statespace = PTX_STATE_GLOBAL;
    decls[0]->shape.kind = SHAPE_SCALAR;
    
    // Set name
    decls[0]->name = strdup("masked_byte");
    
    // Create the masked variable address initializer using the helper function
    ptx_initializer_value_t val = ptx_initializer_value_masked_var_addr("foo", 0, false, PTX_MASK_BYTE0);
    decls[0]->initializer = ptx_initializer_create_scalar(INIT_MASKED_VAR_ADDR, &val);
    
    // Create the second declaration: masked integer
    decls[1] = (ptx_type_decl_t*)malloc(sizeof(ptx_type_decl_t));
    if (decls[1] == NULL) {
        free_example_type_decl(decls[0]);
        free(decls);
        return NULL;
    }
    
    // Set basic properties
    decls[1]->type = PTX_TYPE_U8;
    decls[1]->statespace = PTX_STATE_GLOBAL;
    decls[1]->shape.kind = SHAPE_SCALAR;
    
    // Set name
    decls[1]->name = strdup("masked_int");
    
    // Create the masked integer initializer using the helper function
    val = ptx_initializer_value_masked_int(0x12345678, PTX_MASK_BYTE1);
    decls[1]->initializer = ptx_initializer_create_scalar(INIT_MASKED_INT, &val);
    
    return decls;
}

/**
 * @brief Free a dynamically allocated type declaration
 * 
 * @param decl The type declaration to free
 */
static void free_example_type_decl(ptx_type_decl_t* decl) {
    if (decl == NULL) {
        return;
    }
    
    // Free the name
    if (decl->name != NULL) {
        free(decl->name);
    }
    
    // Free initializer resources
    if (decl->initializer.kind == INIT_VECTOR || decl->initializer.kind == INIT_ARRAY) {
        ptx_initializer_free(&decl->initializer);
    }
    
    // Free the declaration
    free(decl);
}

/**
 * @brief Function to print a 2D array of integers
 * 
 * @param initializer The initializer containing the 2D array
 * @param rows Number of rows
 * @param cols Number of columns
 */
static void print_2d_array(const ptx_initializer_t* initializer, size_t rows, size_t cols) {
    printf("2D Array Contents:\n");
    
    for (size_t i = 0; i < rows; i++) {
        printf("  [ ");
        for (size_t j = 0; j < cols; j++) {
            size_t indices[2] = {i, j};
            ptx_initializer_value_t value;
            
            if (ptx_initializer_get_value(initializer, indices, &value) == 0 && 
                value.kind == INIT_SCALAR_INT) {
                printf("%lld ", (long long)value.value.int_val);
            } else {
                printf("? ");
            }
        }
        printf("]\n");
    }
}

/**
 * @brief Function to print a vector with mixed values
 * 
 * @param initializer The initializer containing the vector
 * @param size Size of the vector
 */
static void print_vector(const ptx_initializer_t* initializer, size_t size) {
    printf("Vector Contents:\n");
    printf("  [ ");
    
    for (size_t i = 0; i < size; i++) {
        size_t indices[1] = {i};
        ptx_initializer_value_t value;
        
        if (ptx_initializer_get_value(initializer, indices, &value) == 0) {
            switch (value.kind) {
                case INIT_SCALAR_INT:
                    printf("%lld ", (long long)value.value.int_val);
                    break;
                case INIT_VAR_ADDR:
                    printf("&%s ", value.value.var_addr.var_name);
                    break;
                case INIT_FUNC_ADDR:
                    printf("&%s() ", value.value.func_name);
                    break;
                default:
                    printf("? ");
                    break;
            }
        } else {
            printf("? ");
        }
    }
    
    printf("]\n");
}

/**
 * @brief Main test function for PTX initializers
 */
int main(void) {
    printf("=== PTX Initializer Test ===\n\n");
    
    // Test scalar initializers
    printf("1. Scalar Initializers:\n");
    
    // Integer initializer
    ptx_initializer_t int_init = ptx_initializer_create_int(42);
    printf("  Integer initializer: %lld\n", (long long)int_init.data.scalar.value.int_val);
    
    // Float initializer
    ptx_initializer_t float_init = ptx_initializer_create_float(3.14159);
    printf("  Float initializer: %f\n", float_init.data.scalar.value.float_val);
    
    // Variable address initializer
    ptx_initializer_t var_addr_init = ptx_initializer_create_var_addr("global_var", 8, true);
    printf("  Variable address initializer: generic(&%s)+%lld\n", 
           var_addr_init.data.scalar.value.var_addr.var_name,
           (long long)var_addr_init.data.scalar.value.var_addr.offset);
    
    // Function address initializer
    ptx_initializer_t func_addr_init = ptx_initializer_create_func_addr("my_function");
    printf("  Function address initializer: &%s()\n", 
           func_addr_init.data.scalar.value.func_name);
    
    // Masked initializers
    ptx_initializer_t masked_var_init = ptx_initializer_create_masked_var_addr("foo", 0, false, PTX_MASK_BYTE0);
    printf("  Masked variable initializer: mask(%s, 0x%llx)\n", 
           masked_var_init.data.scalar.value.masked.value.var_addr.var_name,
           (unsigned long long)masked_var_init.data.scalar.value.masked.mask);
    
    ptx_initializer_t masked_int_init = ptx_initializer_create_masked_int(0x12345678, PTX_MASK_BYTE1);
    printf("  Masked integer initializer: mask(0x%llx, 0x%llx)\n", 
           (unsigned long long)masked_int_init.data.scalar.value.masked.value.int_val,
           (unsigned long long)masked_int_init.data.scalar.value.masked.mask);
    
    printf("\n");
    
    // Test array initializer
    printf("2. Array Example:\n");
    ptx_type_decl_t* array_decl = create_example_global_s32_array();
    if (array_decl) {
        printf("  Created 2D array '%s' of size 4x2\n", array_decl->name);
        print_2d_array(&array_decl->initializer, 4, 2);
        free_example_type_decl(array_decl);
    } else {
        printf("  Failed to create array example\n");
    }
    
    printf("\n");
    
    // Test vector initializer
    printf("3. Vector Example:\n");
    ptx_type_decl_t* vector_decl = create_example_global_v4_u32_mixed();
    if (vector_decl) {
        printf("  Created vector '%s' of size %d\n", vector_decl->name, 
               vector_decl->shape.vector_size);
        print_vector(&vector_decl->initializer, vector_decl->shape.vector_size);
        free_example_type_decl(vector_decl);
    } else {
        printf("  Failed to create vector example\n");
    }
    
    printf("\n");
    
    // Test masked initializers
    printf("4. Masked Initializer Examples:\n");
    ptx_type_decl_t** masked_decls = create_example_masked_initializers();
    if (masked_decls) {
        for (int i = 0; i < 2; i++) {
            ptx_type_decl_t* decl = masked_decls[i];
            printf("  Variable '%s':\n", decl->name);
            
            if (decl->initializer.kind == INIT_MASKED_VAR_ADDR) {
                printf("    mask(%s, 0x%llx)\n",
                       decl->initializer.data.scalar.value.masked.value.var_addr.var_name,
                       (unsigned long long)decl->initializer.data.scalar.value.masked.mask);
            } else if (decl->initializer.kind == INIT_MASKED_INT) {
                printf("    mask(0x%llx, 0x%llx)\n",
                       (unsigned long long)decl->initializer.data.scalar.value.masked.value.int_val,
                       (unsigned long long)decl->initializer.data.scalar.value.masked.mask);
            }
            
            free_example_type_decl(decl);
        }
        free(masked_decls);
    } else {
        printf("  Failed to create masked initializer examples\n");
    }
    
    // Clean up scalar initializers (they don't have dynamic data)
    ptx_initializer_free(&int_init);
    ptx_initializer_free(&float_init);
    ptx_initializer_free(&var_addr_init);
    ptx_initializer_free(&func_addr_init);
    ptx_initializer_free(&masked_var_init);
    ptx_initializer_free(&masked_int_init);
    
    printf("\nAll tests completed!\n");
    return 0;
} 