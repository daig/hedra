#pragma once
#include <ptx_ast/ptx_directive.h>
#include <ptx_ast/ptx_instruction.h>
#include <ptx_ast/ptx_label.h>


typedef enum ptx_statement_tag { DIRECTIVE, INSTRUCTION, } ptx_statement_tag;

typedef struct ptx_statement_t { 
    ptx_statement_tag tag;
    ptx_label_t label; //optional
    union {
        ptx_directive_t directive;
        ptx_instruction_t instruction;
    };
} ptx_statement_t;