#pragma once

#include <stdbool.h>
#include <ptx_ast/>ptx_type.h>

// Forward declaration of the graphics type enum
typedef enum ptx_graphics_type ptx_graphics_type_t;

/**
 * Enum for graphics-related types that references the combined enum.
 * 
 * In PTX, there are two modes for texture operations:
 * 1. Unified mode: texture and sampler information is accessed through a single .texref handle
 * 2. Independent mode: texture (.texref) and sampler (.samplerref) have separate handles
 */
enum ptx_graphics_type {
    PTX_GRAPHICS_TYPE_TEXREF = PTX_TYPE_TEXREF,         // Texture reference type
    PTX_GRAPHICS_TYPE_SAMPLERREF = PTX_TYPE_SAMPLERREF, // Sampler reference type
    PTX_GRAPHICS_TYPE_SURFREF = PTX_TYPE_SURFREF        // Surface reference type
};

/**
 * Check if a type is a graphics-related type (.texref, .samplerref, or .surfref)
 *
 * @param type The type to check
 * @return true if the type is a graphics type, false otherwise
 */
bool ptx_is_graphics_type(ptx_type_t type);

/**
 * Check if the type is a texture reference (.texref)
 *
 * @param type The type to check
 * @return true if the type is a texture reference, false otherwise
 */
bool ptx_is_texture_ref(ptx_type_t type);

/**
 * Check if the type is a sampler reference (.samplerref)
 *
 * @param type The type to check
 * @return true if the type is a sampler reference, false otherwise
 */
bool ptx_is_sampler_ref(ptx_type_t type);

/**
 * Check if the type is a surface reference (.surfref)
 *
 * @param type The type to check
 * @return true if the type is a surface reference, false otherwise
 */
bool ptx_is_surface_ref(ptx_type_t type);

/**
 * Helper function to convert from general ptx_type_t to ptx_graphics_type_t
 * Returns the graphics type if the input is a graphics type, otherwise returns -1
 */
ptx_graphics_type_t ptx_type_to_graphics_type(ptx_type_t type); 