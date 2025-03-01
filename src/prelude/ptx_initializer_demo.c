/**
 * @file ptx_initializer_demo.c
 * @brief Demonstration program for ptx_initializer helper functions
 */

#include "ast/ptx_initializer_examples.h"
#include "ast/ptx_initializer_helpers.h"
#include "ast/ptx_initializer_value_helpers.h"
#include <stdio.h>
#include <stdlib.h>

// Function to print a 2D array of integers
void print_2d_array(const ptx_initializer_t* initializer, size_t rows, size_t cols) {
    printf("2D Array Contents:\n");
    
    for (size_t i = 0; i < rows; i++) {
        printf("  [ ");
        for (size_t j = 0; j < cols; j++) {
            size_t indices[2] = {i, j};
            ptx_initializer_value_t value;
            
            if (ptx_initializer_get_value(initializer, indices, &value) == 0 && 
                value.kind == INIT_SCALAR_INT) {
                printf("%ld ", value.value.int_val);
            } else {
                printf("? ");
            }
        }
        printf("]\n");
    }
}

// Function to print a vector with mixed values
void print_vector(const ptx_initializer_t* initializer, size_t size) {
    printf("Vector Contents:\n");
    printf("  [ ");
    
    for (size_t i = 0; i < size; i++) {
        size_t indices[1] = {i};
        ptx_initializer_value_t value;
        
        if (ptx_initializer_get_value(initializer, indices, &value) == 0) {
            switch (value.kind) {
                case INIT_SCALAR_INT:
                    printf("%ld ", value.value.int_val);
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

// Function to create and demonstrate various initializers
void demonstrate_initializers(void) {
    printf("=== PTX Initializer Demonstrations ===\n\n");
    
    // Demonstrate scalar initializers
    printf("1. Scalar Initializers:\n");
    
    // Integer initializer
    ptx_initializer_t int_init = ptx_initializer_create_int(42);
    printf("  Integer initializer: %ld\n", int_init.data.scalar.value.int_val);
    
    // Float initializer
    ptx_initializer_t float_init = ptx_initializer_create_float(3.14159);
    printf("  Float initializer: %f\n", float_init.data.scalar.value.float_val);
    
    // Variable address initializer
    ptx_initializer_t var_addr_init = ptx_initializer_create_var_addr("global_var", 8, true);
    printf("  Variable address initializer: generic(&%s)+%ld\n", 
           var_addr_init.data.scalar.value.var_addr.var_name,
           var_addr_init.data.scalar.value.var_addr.offset);
    
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
    printf("  Masked integer initializer: mask(0x%lx, 0x%llx)\n", 
           masked_int_init.data.scalar.value.masked.value.int_val,
           (unsigned long long)masked_int_init.data.scalar.value.masked.mask);
    
    printf("\n");
    
    // Demonstrate array initializer
    printf("2. Array Example:\n");
    ptx_type_decl_t* array_decl = create_example_global_s32_array_with_initializer();
    if (array_decl) {
        printf("  Created 2D array '%s' of size 4x2\n", array_decl->name);
        print_2d_array(&array_decl->initializer, 4, 2);
        free_example_type_decl(array_decl);
    } else {
        printf("  Failed to create array example\n");
    }
    
    printf("\n");
    
    // Demonstrate vector initializer
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
    
    // Demonstrate masked initializers
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
                printf("    mask(0x%lx, 0x%llx)\n",
                       decl->initializer.data.scalar.value.masked.value.int_val,
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
}

// Main entry point (for demonstration purposes)
int main(void) {
    demonstrate_initializers();
    return 0;
} 