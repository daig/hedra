#include <ptx_print/ptx_type_print.h>
#include <stdio.h>
#include <string.h>

// Prints a PTX type to a file stream
bool print_type_to_file(FILE* file, ptx_type_t type) {
    if (!file) {
        return false;
    }
    
    int chars_written = 0;
    
    // All PTX types start with a period
    fprintf(file, ".");
    
    // Print the type based on its enum value
    switch (type) {
        // Signed integer types
        case PTX_TYPE_S8:
            chars_written = fprintf(file, "s8");
            break;
        case PTX_TYPE_S16:
            chars_written = fprintf(file, "s16");
            break;
        case PTX_TYPE_S32:
            chars_written = fprintf(file, "s32");
            break;
        case PTX_TYPE_S64:
            chars_written = fprintf(file, "s64");
            break;
            
        // Unsigned integer types
        case PTX_TYPE_U8:
            chars_written = fprintf(file, "u8");
            break;
        case PTX_TYPE_U16:
            chars_written = fprintf(file, "u16");
            break;
        case PTX_TYPE_U32:
            chars_written = fprintf(file, "u32");
            break;
        case PTX_TYPE_U64:
            chars_written = fprintf(file, "u64");
            break;
            
        // Standard floating-point types
        case PTX_TYPE_F16:
            chars_written = fprintf(file, "f16");
            break;
        case PTX_TYPE_F32:
            chars_written = fprintf(file, "f32");
            break;
        case PTX_TYPE_F64:
            chars_written = fprintf(file, "f64");
            break;
            
        // Bits (untyped)
        case PTX_TYPE_B8:
            chars_written = fprintf(file, "b8");
            break;
        case PTX_TYPE_B16:
            chars_written = fprintf(file, "b16");
            break;
        case PTX_TYPE_B32:
            chars_written = fprintf(file, "b32");
            break;
        case PTX_TYPE_B64:
            chars_written = fprintf(file, "b64");
            break;
        case PTX_TYPE_B128:
            chars_written = fprintf(file, "b128");
            break;
            
        // Predicate type
        case PTX_TYPE_PRED:
            chars_written = fprintf(file, "pred");
            break;
            
        // Packed fundamental floating-point type
        case PTX_TYPE_F16X2:
            chars_written = fprintf(file, "f16x2");
            break;
            
        // Alternate non-packed floating-point formats
        case PTX_TYPE_BF16:
            chars_written = fprintf(file, "bf16");
            break;
        case PTX_TYPE_E4M3:
            chars_written = fprintf(file, "e4m3");
            break;
        case PTX_TYPE_E5M2:
            chars_written = fprintf(file, "e5m2");
            break;
        case PTX_TYPE_TF32:
            chars_written = fprintf(file, "tf32");
            break;
        case PTX_TYPE_UE4M3:
            chars_written = fprintf(file, "ue4m3");
            break;
        case PTX_TYPE_E2M1:
            chars_written = fprintf(file, "e2m1");
            break;
        case PTX_TYPE_E2M3:
            chars_written = fprintf(file, "e2m3");
            break;
        case PTX_TYPE_E3M2:
            chars_written = fprintf(file, "e3m2");
            break;
        case PTX_TYPE_UE8M0:
            chars_written = fprintf(file, "ue8m0");
            break;
            
        // Non-fundamental packed floating-point types
        case PTX_TYPE_F32X2:
            chars_written = fprintf(file, "f32x2");
            break;
        case PTX_TYPE_BF16X2:
            chars_written = fprintf(file, "bf16x2");
            break;
        case PTX_TYPE_E4M3X2:
            chars_written = fprintf(file, "e4m3x2");
            break;
        case PTX_TYPE_E5M2X2:
            chars_written = fprintf(file, "e5m2x2");
            break;
        case PTX_TYPE_E2M3X2:
            chars_written = fprintf(file, "e2m3x2");
            break;
        case PTX_TYPE_E3M2X2:
            chars_written = fprintf(file, "e3m2x2");
            break;
        case PTX_TYPE_UE8M0X2:
            chars_written = fprintf(file, "ue8m0x2");
            break;
        case PTX_TYPE_E2M1X2:
            chars_written = fprintf(file, "e2m1x2");
            break;
        case PTX_TYPE_E4M3X4:
            chars_written = fprintf(file, "e4m3x4");
            break;
        case PTX_TYPE_E5M2X4:
            chars_written = fprintf(file, "e5m2x4");
            break;
        case PTX_TYPE_E2M3X4:
            chars_written = fprintf(file, "e2m3x4");
            break;
        case PTX_TYPE_E3M2X4:
            chars_written = fprintf(file, "e3m2x4");
            break;
        case PTX_TYPE_E2M1X4:
            chars_written = fprintf(file, "e2m1x4");
            break;
            
        // Packed integer types
        case PTX_TYPE_U16X2:
            chars_written = fprintf(file, "u16x2");
            break;
        case PTX_TYPE_S16X2:
            chars_written = fprintf(file, "s16x2");
            break;
            
        // Graphics-related types
        case PTX_TYPE_TEXREF:
            chars_written = fprintf(file, "texref");
            break;
        case PTX_TYPE_SAMPLERREF:
            chars_written = fprintf(file, "samplerref");
            break;
        case PTX_TYPE_SURFREF:
            chars_written = fprintf(file, "surfref");
            break;
            
        default:
            return false;
    }
    
    return (chars_written > 0);
}

// Prints a PTX type to a string buffer
int print_type_to_buffer(char* buffer, size_t buffer_size, ptx_type_t type) {
    if (!buffer || buffer_size == 0) {
        return -1;
    }
    
    // Create a temporary file stream using a memory buffer
    FILE* memstream = fmemopen(buffer, buffer_size, "w");
    if (!memstream) {
        return -1;
    }
    
    // Use the file printing function
    bool success = print_type_to_file(memstream, type);
    
    // Get the number of bytes written
    int chars_written = ftell(memstream);
    
    // Close the stream
    fclose(memstream);
    
    return success ? chars_written : -1;
} 