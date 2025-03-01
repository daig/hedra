#pragma once
#include <string.h>

typedef enum predefined_identifier {
    CLOCK,
    LANEID,
    LANEMASK_GT,
    PM0, PM1, PM2, PM3, PM4, PM5, PM6, PM7,
    CLOCK64,
    LANEMASK_EQ,
    NCTAID,
    SMID,
    CTAID,
    LANEMASK_LE,
    NTID,
    TID,
    ENVREG32,
    LANEMASK_LT,
    NSMID,
    WARPID,
    GRIDID,
    LANEMASK_GE,
    NWARPID,
    WARP_SZ
} predefined_identifier_t;

// Function prototype
predefined_identifier_t get_predefined_identifier(const char* str);

