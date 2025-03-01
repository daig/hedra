#include <ptx_parse/ptx_type_parse.h>
#include <ptx_ast/ptx_type_fundamental.h>
#include <ptx_ast/ptx_type_graphics.h>
#include <ptx_ast/ptx_alternate_float_format.h>
#include <ptx_ast/ptx_type_packed_float.h>
#include <ptx_ast/ptx_scalar_float_type.h>
#include <string.h>
#include <ctype.h>

// Struct to map type strings to enum values
typedef struct {
    const char* name;
    ptx_type_t type;
} type_map_entry;

// Array of fundamental type mappings
static const type_map_entry fundamental_type_map[] = {
    {"s8", PTX_TYPE_S8},
    {"s16", PTX_TYPE_S16},
    {"s32", PTX_TYPE_S32},
    {"s64", PTX_TYPE_S64},
    {"u8", PTX_TYPE_U8},
    {"u16", PTX_TYPE_U16},
    {"u32", PTX_TYPE_U32},
    {"u64", PTX_TYPE_U64},
    {"f16", PTX_TYPE_F16},
    {"f32", PTX_TYPE_F32},
    {"f64", PTX_TYPE_F64},
    {"b8", PTX_TYPE_B8},
    {"b16", PTX_TYPE_B16},
    {"b32", PTX_TYPE_B32},
    {"b64", PTX_TYPE_B64},
    {"b128", PTX_TYPE_B128},
    {"pred", PTX_TYPE_PRED},
    {"f16x2", PTX_TYPE_F16X2}
};

// Array of alternate float format mappings
static const type_map_entry alt_float_format_map[] = {
    {"bf16", PTX_TYPE_BF16},
    {"e4m3", PTX_TYPE_E4M3},
    {"e5m2", PTX_TYPE_E5M2},
    {"tf32", PTX_TYPE_TF32},
    {"ue4m3", PTX_TYPE_UE4M3},
    {"e2m1", PTX_TYPE_E2M1},
    {"e2m3", PTX_TYPE_E2M3},
    {"e3m2", PTX_TYPE_E3M2},
    {"ue8m0", PTX_TYPE_UE8M0}
};

// Array of packed float type mappings
static const type_map_entry packed_float_type_map[] = {
    {"f32x2", PTX_TYPE_F32X2},
    {"bf16x2", PTX_TYPE_BF16X2},
    {"e4m3x2", PTX_TYPE_E4M3X2},
    {"e5m2x2", PTX_TYPE_E5M2X2},
    {"e2m3x2", PTX_TYPE_E2M3X2},
    {"e3m2x2", PTX_TYPE_E3M2X2},
    {"ue8m0x2", PTX_TYPE_UE8M0X2},
    {"e2m1x2", PTX_TYPE_E2M1X2},
    {"e4m3x4", PTX_TYPE_E4M3X4},
    {"e5m2x4", PTX_TYPE_E5M2X4},
    {"e2m3x4", PTX_TYPE_E2M3X4},
    {"e3m2x4", PTX_TYPE_E3M2X4},
    {"e2m1x4", PTX_TYPE_E2M1X4}
};

// Array of packed integer type mappings
static const type_map_entry packed_int_type_map[] = {
    {"u16x2", PTX_TYPE_U16X2},
    {"s16x2", PTX_TYPE_S16X2}
};

// Array of graphics type mappings
static const type_map_entry graphics_type_map[] = {
    {"texref", PTX_TYPE_TEXREF},
    {"samplerref", PTX_TYPE_SAMPLERREF},
    {"surfref", PTX_TYPE_SURFREF}
};

// Helper function for case-insensitive string comparison
static int str_case_cmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) {
        return -1;
    }
    
    for (; tolower((unsigned char)*s1) == tolower((unsigned char)*s2); s1++, s2++) {
        if (*s1 == '\0') {
            return 0;
        }
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

// Forward declaration for recursive calls
bool parse_type(const char* str, ptx_type_t* type);

// Function to parse a vector type
// Format: .vN.type (e.g., .v2.f32, .v4.u8)
static bool parse_vector_type(const char* str, ptx_type_t* type) {
    if (str == NULL || type == NULL) {
        return false;
    }
    
    // Skip the leading '.'
    if (str[0] != '.') {
        return false;
    }
    str++;

    // Check for vector start
    if (tolower((unsigned char)str[0]) != 'v') {
        return false;
    }
    str++;

    // Check for vector size
    char size = str[0];
    if (size != '2' && size != '4') {
        return false;
    }
    str++;

    // Must be followed by a period
    if (str[0] != '.') {
        return false;
    }
    
    // For vector types like .v2.f32, we just map to the corresponding packed type
    // Currently only implementing a few common vector types
    if (size == '2') {
        if (str_case_cmp(str, ".f32") == 0) {
            *type = PTX_TYPE_F32X2;
            return true;
        }
        // Add other vector types as needed
    } else if (size == '4') {
        // Handle v4 types as they become necessary
    }
    
    return false;
}

// Main type parsing function
bool parse_type(const char* str, ptx_type_t* type) {
    // Check for NULL inputs
    if (str == NULL || type == NULL) {
        return false;
    }
    
    // Types in PTX always start with a period
    if (str[0] != '.') {
        return false;
    }
    
    // Skip the leading period
    const char* type_str = str + 1;
    
    // Check for vector type
    if (tolower((unsigned char)type_str[0]) == 'v') {
        return parse_vector_type(str, type);
    }
    
    // Try to match against each type map

    // Check fundamental types
    for (size_t i = 0; i < sizeof(fundamental_type_map) / sizeof(fundamental_type_map[0]); i++) {
        if (str_case_cmp(type_str, fundamental_type_map[i].name) == 0) {
            *type = fundamental_type_map[i].type;
            return true;
        }
    }
    
    // Check alternate float formats
    for (size_t i = 0; i < sizeof(alt_float_format_map) / sizeof(alt_float_format_map[0]); i++) {
        if (str_case_cmp(type_str, alt_float_format_map[i].name) == 0) {
            *type = alt_float_format_map[i].type;
            return true;
        }
    }
    
    // Check packed float types
    for (size_t i = 0; i < sizeof(packed_float_type_map) / sizeof(packed_float_type_map[0]); i++) {
        if (str_case_cmp(type_str, packed_float_type_map[i].name) == 0) {
            *type = packed_float_type_map[i].type;
            return true;
        }
    }
    
    // Check packed integer types
    for (size_t i = 0; i < sizeof(packed_int_type_map) / sizeof(packed_int_type_map[0]); i++) {
        if (str_case_cmp(type_str, packed_int_type_map[i].name) == 0) {
            *type = packed_int_type_map[i].type;
            return true;
        }
    }
    
    // Check graphics types
    for (size_t i = 0; i < sizeof(graphics_type_map) / sizeof(graphics_type_map[0]); i++) {
        if (str_case_cmp(type_str, graphics_type_map[i].name) == 0) {
            *type = graphics_type_map[i].type;
            return true;
        }
    }
    
    // If we get here, no matching type was found
    return false;
} 