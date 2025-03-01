#include <ptx_ast/ptx_statespace.h>

// Check if a state space is addressable
bool is_addressable(ptx_state_space_t space) {
    switch (space) {
        case PTX_STATE_CONST:
        case PTX_STATE_GLOBAL:
        case PTX_STATE_LOCAL:
        case PTX_STATE_PARAM:
        case PTX_STATE_SHARED:
            return true;
        default:
            return false;
    }
}

// Check if a state space is initializable
bool is_initializable(ptx_state_space_t space) {
    switch (space) {
        case PTX_STATE_CONST:
        case PTX_STATE_GLOBAL:
        case PTX_STATE_TEX:
            return true;
        default:
            return false;
    }
}

// Check if a state space is read-only
bool is_read_only(ptx_state_space_t space) {
    switch (space) {
        case PTX_STATE_SREG:
        case PTX_STATE_CONST:
        case PTX_STATE_TEX:
            return true;
        default:
            return false;
    }
}

// Get the sharing scope of a state space
sharing_scope_t get_sharing_scope(ptx_state_space_t space) {
    switch (space) {
        case PTX_STATE_REG:
        case PTX_STATE_LOCAL:
            return SHARE_THREAD;
        case PTX_STATE_SREG:
            return SHARE_CTA;
        case PTX_STATE_SHARED:
            return SHARE_CLUSTER;
        case PTX_STATE_CONST:
        case PTX_STATE_PARAM:
            return SHARE_GRID;
        case PTX_STATE_GLOBAL:
        case PTX_STATE_TEX:
            return SHARE_CONTEXT;
        default:
            return SHARE_THREAD; // Default to most restrictive
    }
} 