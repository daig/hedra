/**
 * @file ptx_initializer_examples.c
 * @brief Implementation of example functions demonstrating how to use the helper functions
 */

#include <ptx_ast/ptx_initializer_examples.h>
#include <ptx_ast/ptx_initializer_helpers.h>
#include <ptx_ast/ptx_initializer_value_helpers.h>
#include <stdlib.h>
#include <string.h>

ptx_type_decl_t* create_example_global_s32_array_with_initializer(void) {
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
    
    // Using the new helper functions for creating values
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

ptx_type_decl_t* create_example_global_v4_u32_mixed(void) {
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
    
    // Set values using the new helper functions
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

ptx_type_decl_t** create_example_masked_initializers(void) {
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

void free_example_type_decl(ptx_type_decl_t* decl) {
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