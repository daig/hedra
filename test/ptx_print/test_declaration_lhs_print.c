#include <ptx_print/ptx_declaration_lhs_print.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <prelude/ptx_array_shape.h>

#define BUFFER_SIZE 256

void test_scalar_declaration() {
    printf("Testing scalar declaration printing...\n");
    
    ptx_declaration_type_t decl = {
        .statespace = PTX_STATE_GLOBAL,
        .type = PTX_TYPE_U32,
        .shape = {
            .kind = SHAPE_SCALAR
        },
        .name = "loc",
        .has_initializer = false
    };
    
    char buffer[BUFFER_SIZE] = {0};
    int result = print_declaration_lhs_to_buffer(buffer, BUFFER_SIZE, &decl);
    assert(result > 0);
    
    printf("Expected: \".global .u32 loc\"\n");
    printf("Got:      \"%s\"\n", buffer);
    assert(strcmp(buffer, ".global .u32 loc") == 0);
    
    printf("Scalar declaration test passed!\n\n");
}

void test_vector_declaration() {
    printf("Testing vector declaration printing...\n");
    
    ptx_declaration_type_t decl = {
        .statespace = PTX_STATE_REG,
        .type = PTX_TYPE_F32,
        .shape = {
            .kind = SHAPE_VECTOR,
            .vector_size = VECTOR_SIZE_V4
        },
        .name = "accel",
        .has_initializer = false
    };
    
    char buffer[BUFFER_SIZE] = {0};
    int result = print_declaration_lhs_to_buffer(buffer, BUFFER_SIZE, &decl);
    assert(result > 0);
    
    printf("Expected: \".reg .v4 .f32 accel\"\n");
    printf("Got:      \"%s\"\n", buffer);
    assert(strcmp(buffer, ".reg .v4 .f32 accel") == 0);
    
    printf("Vector declaration test passed!\n\n");
}

void test_array_declaration() {
    printf("Testing array declaration printing...\n");
    
    ptx_array_shape_t array_shape = ptx_array_shape_create(2, 19, 19);
    
    ptx_declaration_type_t decl = {
        .statespace = PTX_STATE_LOCAL,
        .type = PTX_TYPE_U16,
        .shape = {
            .kind = SHAPE_ARRAY,
            .array_shape = array_shape
        },
        .name = "kernel",
        .has_initializer = false
    };
    
    char buffer[BUFFER_SIZE] = {0};
    int result = print_declaration_lhs_to_buffer(buffer, BUFFER_SIZE, &decl);
    assert(result > 0);
    
    printf("Expected: \".local .u16 kernel[19][19]\"\n");
    printf("Got:      \"%s\"\n", buffer);
    assert(strcmp(buffer, ".local .u16 kernel[19][19]") == 0);
    
    ptx_array_shape_free(array_shape);
    printf("Array declaration test passed!\n\n");
}

/**
 * Test printing a declaration with alignment
 */
void test_print_with_alignment() {
    printf("Testing printing declaration with alignment...\n");
    
    ptx_declaration_type_t decl = {
        .statespace = PTX_STATE_CONST,
        .type = PTX_TYPE_B8,
        .shape = {
            .kind = SHAPE_ARRAY,
            .array_shape = ptx_array_shape_create(1, 8)
        },
        .name = "bar",
        .alignment = 4
    };
    
    char buffer[BUFFER_SIZE];
    int result = print_declaration_lhs_to_buffer(buffer, BUFFER_SIZE, &decl);
    
    printf("Printed declaration: \"%s\"\n", buffer);
    printf("Expected: \".const .align 4 .b8 bar[8]\"\n");
    
    assert(result > 0);
    assert(strcmp(buffer, ".const .align 4 .b8 bar[8]") == 0);
    
    // Cleanup
    ptx_array_shape_free(decl.shape.array_shape);
    
    printf("Alignment printing test passed!\n\n");
}

int main() {
    printf("Running PTX declaration LHS printer tests\n");
    printf("=========================================\n\n");
    
    test_scalar_declaration();
    test_vector_declaration();
    test_array_declaration();
    test_print_with_alignment();
    
    printf("All tests passed!\n");
    return 0;
} 