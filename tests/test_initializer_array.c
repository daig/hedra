/**
 * @file test_initializer_array.c
 * @brief Test for the dynamic multi-dimensional array implementation
 */

#include "ptx_initializer_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test creation and basic properties
void test_create() {
    printf("Testing array creation and basic properties...\n");
    
    // Create a 2D array shape (3x4)
    ptx_array_shape_t shape = ptx_array_shape_create(2, 3, 4);
    assert(shape != NULL);
    
    // Create array of integers
    ptx_initializer_array_t* array = ptx_initializer_array_create(shape, sizeof(int));
    assert(array != NULL);
    
    // Check properties
    assert(ptx_array_shape_ndims(array->shape) == 2);
    assert(array->element_size == sizeof(int));
    assert(ptx_initializer_array_elements(array) == 12); // 3x4
    
    // Clean up
    ptx_initializer_array_free(array);
    ptx_array_shape_free(shape);
    
    printf("PASS: Array creation and basic properties\n");
}

// Test setting and getting elements
void test_set_get() {
    printf("Testing setting and getting array elements...\n");
    
    // Create a 2D array shape (3x4)
    ptx_array_shape_t shape = ptx_array_shape_create(2, 3, 4);
    
    // Create array of integers
    ptx_initializer_array_t* array = ptx_initializer_array_create(shape, sizeof(int));
    
    // Set some values
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            int value = i * 10 + j;
            size_t indices[2] = {i, j};
            int result = ptx_initializer_array_set(array, indices, &value);
            assert(result == 0);
        }
    }
    
    // Get and verify values
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            size_t indices[2] = {i, j};
            int* value_ptr = (int*)ptx_initializer_array_get(array, indices);
            assert(value_ptr != NULL);
            assert(*value_ptr == i * 10 + j);
        }
    }
    
    // Test out of bounds access
    size_t out_of_bounds[2] = {3, 0}; // First dimension is out of bounds
    assert(ptx_initializer_array_get(array, out_of_bounds) == NULL);
    
    int value = 100;
    assert(ptx_initializer_array_set(array, out_of_bounds, &value) == -1);
    
    // Clean up
    ptx_initializer_array_free(array);
    ptx_array_shape_free(shape);
    
    printf("PASS: Setting and getting array elements\n");
}

// Test resizing the array
void test_resize() {
    printf("Testing array resizing...\n");
    
    // Create a 1D array shape (5)
    ptx_array_shape_t shape1 = ptx_array_shape_create(1, 5);
    
    // Create array of doubles
    ptx_initializer_array_t* array = ptx_initializer_array_create(shape1, sizeof(double));
    
    // Set some values
    for (int i = 0; i < 5; i++) {
        double value = i * 1.5;
        size_t indices[1] = {i};
        ptx_initializer_array_set(array, indices, &value);
    }
    
    // Create a new 2D shape (2x3)
    ptx_array_shape_t shape2 = ptx_array_shape_create(2, 2, 3);
    
    // Resize the array
    int result = ptx_initializer_array_resize(array, shape2);
    assert(result == 0);
    
    // Check new properties
    assert(ptx_array_shape_ndims(array->shape) == 2);
    assert(ptx_initializer_array_elements(array) == 6); // 2x3
    
    // Set new values in the resized array
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            double value = i + j / 10.0;
            size_t indices[2] = {i, j};
            result = ptx_initializer_array_set(array, indices, &value);
            assert(result == 0);
        }
    }
    
    // Get and verify values
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            size_t indices[2] = {i, j};
            double* value_ptr = (double*)ptx_initializer_array_get(array, indices);
            assert(value_ptr != NULL);
            assert(*value_ptr == i + j / 10.0);
        }
    }
    
    // Clean up
    ptx_initializer_array_free(array);
    ptx_array_shape_free(shape1);
    ptx_array_shape_free(shape2);
    
    printf("PASS: Array resizing\n");
}

// Test with different element types
void test_different_types() {
    printf("Testing array with different element types...\n");
    
    // Test with a struct
    typedef struct {
        int id;
        float value;
        char name[20];
    } TestStruct;
    
    // Create a 1D array shape (3)
    ptx_array_shape_t shape = ptx_array_shape_create(1, 3);
    
    // Create array of TestStruct
    ptx_initializer_array_t* array = ptx_initializer_array_create(shape, sizeof(TestStruct));
    
    // Set some values
    for (int i = 0; i < 3; i++) {
        TestStruct item;
        item.id = i + 100;
        item.value = i * 2.5f;
        sprintf(item.name, "Item %d", i);
        
        size_t indices[1] = {i};
        int result = ptx_initializer_array_set(array, indices, &item);
        assert(result == 0);
    }
    
    // Get and verify values
    for (int i = 0; i < 3; i++) {
        size_t indices[1] = {i};
        TestStruct* item_ptr = (TestStruct*)ptx_initializer_array_get(array, indices);
        assert(item_ptr != NULL);
        assert(item_ptr->id == i + 100);
        assert(item_ptr->value == i * 2.5f);
        
        char expected_name[20];
        sprintf(expected_name, "Item %d", i);
        assert(strcmp(item_ptr->name, expected_name) == 0);
    }
    
    // Clean up
    ptx_initializer_array_free(array);
    ptx_array_shape_free(shape);
    
    printf("PASS: Array with different element types\n");
}

int main() {
    printf("=== Testing ptx_initializer_array ===\n");
    
    test_create();
    test_set_get();
    test_resize();
    test_different_types();
    
    printf("All tests passed!\n");
    return 0;
} 