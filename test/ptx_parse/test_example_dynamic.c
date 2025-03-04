#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ptx_parse/ptx_code_block_parse.h"
#include "ptx_print/ptx_code_block_print.h"
#include "ptx_ast/ptx_code_block.h"
#include "prelude/prelude.h"

/**
 * Test parsing the code block from example_dynamic.ptx specifically for label handling
 */
bool test_example_dynamic_ptx() {
    // This is the actual code from example_dynamic.ptx
    const char* input = "{\n"
                        "        // Load function parameters\n"
                        "        ld.param.u64    %rd_grid_ptr, [grid_param];  // Load grid pointer\n"
                        "        ld.param.u32    %r_num_rows, [N_param];      // Load number of rows\n"
                        "        ld.param.u32    %r_num_cols, [M_param];      // Load number of columns\n"
                        "\n"
                        "        // Calculate thread indices for 2D grid\n"
                        "        mov.u32         %r_block_dim_y, %ntid.y;        // Thread block size in Y dimension\n"
                        "        mov.u32         %r_block_idx_y, %ctaid.y;       // Block index in Y dimension\n"
                        "        mov.u32         %r_thread_idx_y, %tid.y;        // Thread index within block in Y dimension\n"
                        "        mad.lo.s32      %r_thread_y, %r_block_idx_y, %r_block_dim_y, %r_thread_idx_y;  // Global thread index in Y dimension: blockIdx.y * blockDim.y + threadIdx.y\n"
                        "        add.s32         %r_thread_y_plus1, %r_thread_y, 1;         // Y index + 1 (for accessing next row)\n"
                        "        \n"
                        "        mov.u32         %r_block_dim_x, %ntid.x;        // Thread block size in X dimension\n"
                        "        mov.u32         %r_block_idx_x, %ctaid.x;       // Block index in X dimension\n"
                        "        mov.u32         %r_thread_idx_x, %tid.x;        // Thread index within block in X dimension\n"
                        "        mad.lo.s32      %r_thread_x, %r_block_idx_x, %r_block_dim_x, %r_thread_idx_x;  // Global thread index in X dimension: blockIdx.x * blockDim.x + threadIdx.x\n"
                        "        add.s32         %r_thread_x_plus1, %r_thread_x, 1;        // X index + 1 (for accessing next column)\n"
                        "        \n"
                        "        // Check if this thread is out of bounds (i.e., if the indices are beyond grid dimensions)\n"
                        "        setp.ge.s32     %p_out_of_bounds_y, %r_thread_y_plus1, %r_num_rows;        // Check if Y+1 is out of bounds (>= N)\n"
                        "        setp.ge.s32     %p_out_of_bounds_x, %r_thread_x_plus1, %r_num_cols;        // Check if X+1 is out of bounds (>= M)\n"
                        "        or.pred         %p_out_of_bounds, %p_out_of_bounds_y, %p_out_of_bounds_x;  // Combine both boundary checks\n"
                        "        @%p_out_of_bounds bra        $RETURN;           // If out of bounds, skip computation and exit\n"
                        "\n"
                        "        // Convert grid pointer to global memory address\n"
                        "        cvta.to.global.u64      %rd_global_grid_ptr, %rd_grid_ptr;\n"
                        "        \n"
                        "        // Calculate 1D indices for accessing the 2D grid which is stored linearly in memory\n"
                        "        mad.lo.s32      %r_linear_idx_current, %r_thread_y, %r_num_cols, %r_thread_x;  // Current position index: y * M + x\n"
                        "        mad.lo.s32      %r_linear_idx_next_row, %r_thread_y_plus1, %r_num_cols, %r_thread_x;  // Next row position index: (y+1) * M + x\n"
                        "        \n"
                        "        // Calculate memory addresses for reading/writing\n"
                        "        mul.wide.s32    %rd_current_offset, %r_linear_idx_current, 4;       // Current position address offset (float = 4 bytes)\n"
                        "        add.s64         %rd_current_addr, %rd_global_grid_ptr, %rd_current_offset;    // Current position address\n"
                        "        mul.wide.s32    %rd_next_row_offset, %r_linear_idx_next_row, 4;     // Next row position address offset\n"
                        "        add.s64         %rd_next_row_addr, %rd_global_grid_ptr, %rd_next_row_offset;  // Next row position address\n"
                        "\n"
                        "        // Perform computation - appears to be a weighted averaging or stencil operation\n"
                        "        ld.global.f32   %f_val_right, [%rd_current_addr+4];       // Load value at (y, x+1)\n"
                        "        ld.global.f32   %f_val_bottom, [%rd_next_row_addr];       // Load value at (y+1, x)\n"
                        "        mul.f32         %f_weighted_bottom, %f_val_bottom, 0f3ECCCCCD;  // Multiply (y+1, x) by 0.4 (0x3ECCCCCD = 0.4 in float)\n"
                        "        fma.rn.f32      %f_weighted_sum_rb, %f_val_right, 0f3ECCCCCD, %f_weighted_bottom;  // Multiply (y, x+1) by 0.4 and add to previous result\n"
                        "        ld.global.f32   %f_val_current, [%rd_current_addr];       // Load value at (y, x)\n"
                        "        fma.rn.f32      %f_weighted_result, %f_val_current, 0f3E4CCCCD, %f_weighted_sum_rb;  // Multiply (y, x) by 0.2 (0x3E4CCCCD = 0.2 in float) and add to previous result\n"
                        "        st.global.f32   [%rd_next_row_addr+4], %f_weighted_result;       // Store result at (y+1, x+1)\n"
                        "        // Final computation: grid[y+1][x+1] = 0.2*grid[y][x] + 0.4*grid[y][x+1] + 0.4*grid[y+1][x]\n"
                        "\n"
                        "$RETURN:\n"
                        "        ret;  // Return from the kernel function\n"
                        "}";
    
    size_t consumed = 0;
    ptx_code_block_t* block = NULL;
    
    printf("Parsing full example_dynamic.ptx code block...\n");
    bool result = ptx_parse_code_block(input, &consumed, &block);
    printf("Parse result: %s, consumed: %zu\n", result ? "true" : "false", consumed);
    
    if (!result || !block) {
        printf("Failed to parse code block\n");
        return false;
    }
    
    // Check the number of statements
    printf("Number of statements: %zu\n", block->num_statements);
    
    // Look for the statement with the RETURN label
    bool found_return_label = false;
    for (size_t i = 0; i < block->num_statements; i++) {
        if (block->statements[i]->label.name && strcmp(block->statements[i]->label.name, "$RETURN") == 0) {
            found_return_label = true;
            printf("Found $RETURN label in statement %zu: %s\n", 
                   i, block->statements[i]->original_text);
            
            // Check that this is a return statement
            if (block->statements[i]->tag == INSTRUCTION) {
                printf("Statement with $RETURN label is an INSTRUCTION (expected)\n");
            } else {
                printf("Statement with $RETURN label is NOT an INSTRUCTION (unexpected)\n");
                found_return_label = false;
            }
            
            // Print the original source statement with the label
            printf("Original statement with label: %s\n", block->statements[i]->original_text);
        }
    }
    
    // Print out all statements to verify the label handling
    printf("\nAll parsed statements:\n");
    for (size_t i = 0; i < block->num_statements; i++) {
        printf("Statement %zu: %s\n", i, block->statements[i]->original_text);
        if (block->statements[i]->label.name) {
            printf("  Label: %s\n", block->statements[i]->label.name);
        }
    }
    
    // Free the block
    ptx_code_block_free(block);
    
    return found_return_label;
}

int main() {
    bool test_passed = test_example_dynamic_ptx();
    
    printf("\nTest %s\n", test_passed ? "PASSED" : "FAILED");
    return test_passed ? 0 : 1;
} 