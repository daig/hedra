#include <prelude/>prelude.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void test_shape(size_t ndims, size_t *dims, size_t expected_elements) {
    // Create shape from array
    ptx_array_shape_t shape = (ptx_array_shape_t)malloc((ndims + 1) * sizeof(size_t));
    for (size_t i = 0; i < ndims; i++) {
        shape[i] = dims[i];
    }
    shape[ndims] = 0;
    
    // Test properties
    assert(ptx_array_shape_ndims(shape) == ndims);
    assert(ptx_array_shape_elements(shape) == expected_elements);
    
    // Test with create function
    ptx_array_shape_t shape2;
    switch (ndims) {
        case 0:
            shape2 = ptx_array_shape_create(0);
            break;
        case 1:
            shape2 = ptx_array_shape_create(1, dims[0]);
            break;
        case 2:
            shape2 = ptx_array_shape_create(2, dims[0], dims[1]);
            break;
        case 3:
            shape2 = ptx_array_shape_create(3, dims[0], dims[1], dims[2]);
            break;
        case 4:
            shape2 = ptx_array_shape_create(4, dims[0], dims[1], dims[2], dims[3]);
            break;
        default:
            printf("Test case for %zu dimensions not implemented\n", ndims);
            shape2 = NULL;
            break;
    }
    
    if (shape2 != NULL) {
        assert(ptx_array_shape_equals(shape, shape2));
        ptx_array_shape_free(shape2);
    }
    
    ptx_array_shape_free(shape);
}

int main() {
    printf("Testing ptx_array_shape functions...\n");
    
    // Test empty shape
    {
        size_t dims[] = {};
        test_shape(0, dims, 0);
        printf("✓ Empty shape test passed\n");
    }
    
    // Test 1D shape
    {
        size_t dims[] = {5};
        test_shape(1, dims, 5);
        printf("✓ 1D shape test passed\n");
    }
    
    // Test 2D shape
    {
        size_t dims[] = {3, 4};
        test_shape(2, dims, 12);
        printf("✓ 2D shape test passed\n");
    }
    
    // Test 3D shape
    {
        size_t dims[] = {2, 3, 4};
        test_shape(3, dims, 24);
        printf("✓ 3D shape test passed\n");
    }
    
    // Test 4D shape
    {
        size_t dims[] = {2, 2, 2, 2};
        test_shape(4, dims, 16);
        printf("✓ 4D shape test passed\n");
    }
    
    // Test copy and equality
    {
        ptx_array_shape_t shape1 = ptx_array_shape_create(3, 2, 3, 4);
        ptx_array_shape_t shape2 = ptx_array_shape_copy(shape1);
        assert(ptx_array_shape_equals(shape1, shape2));
        
        ptx_array_shape_t shape3 = ptx_array_shape_create(3, 2, 3, 5); // Different last dimension
        assert(!ptx_array_shape_equals(shape1, shape3));
        
        ptx_array_shape_free(shape1);
        ptx_array_shape_free(shape2);
        ptx_array_shape_free(shape3);
        printf("✓ Copy and equality tests passed\n");
    }
    
    printf("All tests passed!\n");
    return 0;
} 