#pragma once

#include <stdbool.h>
#include <ptx_ast/ptx_instruction.h>
#include <ptx_ast/ptx_type.h>

/**
 * Enum for fundamental types that references the combined enum.
 */
enum ptx_fundamental_type {
    PTX_FUNDAMENTAL_TYPE_S8 = PTX_TYPE_S8,
    PTX_FUNDAMENTAL_TYPE_S16 = PTX_TYPE_S16,
    PTX_FUNDAMENTAL_TYPE_S32 = PTX_TYPE_S32,
    PTX_FUNDAMENTAL_TYPE_S64 = PTX_TYPE_S64,

    PTX_FUNDAMENTAL_TYPE_U8 = PTX_TYPE_U8,
    PTX_FUNDAMENTAL_TYPE_U16 = PTX_TYPE_U16,
    PTX_FUNDAMENTAL_TYPE_U32 = PTX_TYPE_U32,
    PTX_FUNDAMENTAL_TYPE_U64 = PTX_TYPE_U64,

    PTX_FUNDAMENTAL_TYPE_F16 = PTX_TYPE_F16,
    PTX_FUNDAMENTAL_TYPE_F32 = PTX_TYPE_F32,
    PTX_FUNDAMENTAL_TYPE_F64 = PTX_TYPE_F64,
    PTX_FUNDAMENTAL_TYPE_F16X2 = PTX_TYPE_F16X2,

    PTX_FUNDAMENTAL_TYPE_B8 = PTX_TYPE_B8,
    PTX_FUNDAMENTAL_TYPE_B16 = PTX_TYPE_B16,
    PTX_FUNDAMENTAL_TYPE_B32 = PTX_TYPE_B32,
    PTX_FUNDAMENTAL_TYPE_B64 = PTX_TYPE_B64,
    PTX_FUNDAMENTAL_TYPE_B128 = PTX_TYPE_B128,

    PTX_FUNDAMENTAL_TYPE_PRED = PTX_TYPE_PRED
};

// Type compatibility checking functions
// Check if a given type is restricted to only ld, st, and cvt instructions
bool ptx_type_restricted_to_load_store_convert(ptx_fundamental_type_t type);

// Check if a type is compatible with a specific instruction
bool ptx_type_compatible_with_instruction(ptx_fundamental_type_t type, ptx_instruction_t instruction);

// Check if a type is a floating point half-precision type (f16 or f16x2)
bool ptx_type_is_half_precision(ptx_fundamental_type_t type);

// Check if an instruction is a half-precision floating point instruction
bool ptx_instruction_is_half_precision(ptx_instruction_t instruction);

// Check if an instruction is a texture fetch instruction
bool ptx_instruction_is_texture_fetch(ptx_instruction_t instruction);

// Check if an instruction is a conversion instruction
bool ptx_instruction_is_conversion(ptx_instruction_t instruction);

// Check if type is allowed for conversion to/from f32, f64
bool ptx_type_allowed_for_float_conversion(ptx_fundamental_type_t type);

// Helper function to convert from general ptx_type_t to ptx_fundamental_type_t
// Returns the fundamental type if the input is a fundamental type, otherwise returns -1
ptx_fundamental_type_t ptx_type_to_fundamental_type(ptx_type_t type);

