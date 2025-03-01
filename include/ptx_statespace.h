#pragma once

typedef enum ptx_state_space_t {
    PTX_STATE_REG,      // Registers, fast
    PTX_STATE_SREG,     // Special registers (read-only, pre-defined, platform-specific)
    PTX_STATE_CONST,    // Shared, read-only memory
    PTX_STATE_GLOBAL,   // Global memory, shared by all threads
    PTX_STATE_LOCAL,    // Local memory, private to each thread
    PTX_STATE_PARAM,    // Kernel parameters (per-grid) or function/local parameters (per-thread)
    PTX_STATE_SHARED,   // Addressable memory per CTA, accessible to all threads in cluster
    PTX_STATE_TEX       // Global texture memory (deprecated)
} ptx_state_space_t;

// Check if a state space is addressable
bool is_addressable(ptx_state_space_t space);

// Check if a state space is initializable
bool is_initializable(ptx_state_space_t space);

// Check if a state space is read-only
bool is_read_only(ptx_state_space_t space);

// Get the sharing scope of a state space
typedef enum {
    SHARE_THREAD,  // Private to each thread
    SHARE_CTA,     // Shared within a CTA
    SHARE_CLUSTER, // Shared within a cluster
    SHARE_GRID,    // Shared within a grid
    SHARE_CONTEXT  // Shared across context
} sharing_scope_t;

sharing_scope_t get_sharing_scope(ptx_state_space_t space);
