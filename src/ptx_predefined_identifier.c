#include "ptx_predefined_identifier.h"
#include <string.h>

predefined_identifier_t get_predefined_identifier(const char* str) {
    if (strcmp(str, "%clock") == 0) return CLOCK;
    if (strcmp(str, "%laneid") == 0) return LANEID;
    if (strcmp(str, "%lanemask_gt") == 0) return LANEMASK_GT;
    if (strcmp(str, "%pm0") == 0) return PM0;
    if (strcmp(str, "%pm1") == 0) return PM1;
    if (strcmp(str, "%pm2") == 0) return PM2;
    if (strcmp(str, "%pm3") == 0) return PM3;
    if (strcmp(str, "%pm4") == 0) return PM4;
    if (strcmp(str, "%pm5") == 0) return PM5;
    if (strcmp(str, "%pm6") == 0) return PM6;
    if (strcmp(str, "%pm7") == 0) return PM7;
    if (strcmp(str, "%clock64") == 0) return CLOCK64;
    if (strcmp(str, "%lanemask_eq") == 0) return LANEMASK_EQ;
    if (strcmp(str, "%nctaid") == 0) return NCTAID;
    if (strcmp(str, "%smid") == 0) return SMID;
    if (strcmp(str, "%ctaid") == 0) return CTAID;
    if (strcmp(str, "%lanemask_le") == 0) return LANEMASK_LE;
    if (strcmp(str, "%ntid") == 0) return NTID;
    if (strcmp(str, "%tid") == 0) return TID;
    if (strcmp(str, "%envreg32") == 0) return ENVREG32;
    if (strcmp(str, "%lanemask_lt") == 0) return LANEMASK_LT;
    if (strcmp(str, "%nsmid") == 0) return NSMID;
    if (strcmp(str, "%warpid") == 0) return WARPID;
    if (strcmp(str, "%gridid") == 0) return GRIDID;
    if (strcmp(str, "%lanemask_ge") == 0) return LANEMASK_GE;
    if (strcmp(str, "%nwarpid") == 0) return NWARPID;
    if (strcmp(str, "WARP_SZ") == 0) return WARP_SZ;
    return (predefined_identifier_t)-1; // Return invalid if no match found
} 