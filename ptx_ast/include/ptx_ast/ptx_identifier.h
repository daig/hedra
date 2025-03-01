#pragma once
#include <ptx_ast/>ptx_predefined_identifier.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum ptx_identifier_tag {
    PTX_IDENTIFIER_PREDEFINED,
    PTX_IDENTIFIER_USER_DEFINED
} ptx_identifier_tag_t;

typedef struct ptx_identifier {
    ptx_identifier_tag_t tag;
    union {
        predefined_identifier_t predefined;
        char* user_defined;
    };
} ptx_identifier_t;

// Function prototypes
bool is_followsym(char c);
bool is_valid_user_defined_identifier(const char* str);
