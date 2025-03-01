#include <ptx_ast/ptx_type_graphics.h>

bool ptx_is_graphics_type(ptx_type_t type) {
    return (type == PTX_TYPE_TEXREF || 
            type == PTX_TYPE_SAMPLERREF || 
            type == PTX_TYPE_SURFREF);
}

bool ptx_is_texture_ref(ptx_type_t type) {
    return type == PTX_TYPE_TEXREF;
}

bool ptx_is_sampler_ref(ptx_type_t type) {
    return type == PTX_TYPE_SAMPLERREF;
}

bool ptx_is_surface_ref(ptx_type_t type) {
    return type == PTX_TYPE_SURFREF;
}

ptx_graphics_type_t ptx_type_to_graphics_type(ptx_type_t type) {
    if (ptx_is_graphics_type(type)) {
        return (ptx_graphics_type_t)type;
    }
    return -1; // Indicate not a graphics type
} 