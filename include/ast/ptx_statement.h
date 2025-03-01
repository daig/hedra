#pragma once
#include "ptx_directive.h"
#include "ptx_instruction.h"
#include "ptx_label.h"


enum ptx_statement_tag { DIRECTIVE, INSTRUCTION, };

struct ptx_statement_t { 
    ptx_statement_tag tag;
    ptx_label_t label; //optional
    union {
        ptx_directive_t directive;
        ptx_instruction_t instruction;
    };
};