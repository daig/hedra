# PTX Pretty Printer Checklist

This document provides a checklist of all the pretty printing functions required for the "hedra" project's PTX (Parallel Thread Execution) AST back to text. These functions are the inverse of the parsing functions in the original checklist.

Each pretty printer should:
1. Take an AST node as input and produce the corresponding PTX text representation
2. Have a test that verifies parsing followed by printing yields the same string (modulo whitespace)

## Pretty Printer Functions

- [x] **print_directive**
  - **Purpose**: Prints a `ptx_directive_t` AST node as a PTX directive text (e.g., `.version`, `.target`, `.func`, `.entry`).
  - **Test**: Parse directive text → Print AST → Compare (modulo whitespace).
  - **Files**: 
    - Header: `ptx_print/include/ptx_print/ptx_directive_print.h`
    - Implementation: `ptx_print/src/ptx_directive_print.c`
    - Test: `test/ptx_print/test_directive_print.c`

- [x] **print_instruction**
  - **Purpose**: Prints a `ptx_instruction_t` AST node as a PTX instruction text (e.g., `add`, `mul`, `ld`).
  - **Test**: Parse instruction text → Print AST → Compare (modulo whitespace).
  - **Files**: 
    - Header: `ptx_print/include/ptx_print/ptx_instruction_print.h`
    - Implementation: `ptx_print/src/ptx_instruction_print.c`
    - Test: `test/ptx_print/test_instruction_print.c`

- [x] **print_label**
  - **Purpose**: Prints a `ptx_label_t` AST node as a label text (e.g., `my_label:`).
  - **Test**: Parse label text → Print AST → Compare (modulo whitespace).
  - **Files**: 
    - Header: `ptx_print/include/ptx_print/ptx_label_print.h`
    - Implementation: `ptx_print/src/ptx_label_print.c`
    - Test: `test/ptx_print/test_label_print.c`

- [x] **print_constant**
  - **Purpose**: Prints a `ptx_constant_t` AST node as a constant value (e.g., integers, floats, predicates).
  - **Test**: Parse constant text → Print AST → Compare (modulo whitespace).
  - **Files**: 
    - Header: `ptx_print/include/ptx_print/ptx_constant_print.h`
    - Implementation: `ptx_print/src/ptx_constant_print.c`
    - Test: `test/ptx_print/test_constant_print.c`

- [x] **print_identifier**
  - **Purpose**: Prints a `ptx_identifier_t` AST node as an identifier text (e.g., variable names, register names).
  - **Test**: Parse identifier text → Print AST → Compare (modulo whitespace).
  - **Files**: 
    - Header: `ptx_print/include/ptx_print/ptx_identifier_print.h`
    - Implementation: `ptx_print/src/ptx_identifier_print.c`
    - Test: `test/ptx_print/test_identifier_print.c`

- [x] **print_type**
  - **Purpose**: Prints a `ptx_type_t` AST node as a type specifier text (e.g., `.u32`, `.f64`, `.texref`).
  - **Test**: Parse type text → Print AST → Compare (modulo whitespace).
  - **Files**: 
    - Header: `ptx_print/include/ptx_print/ptx_type_print.h`
    - Implementation: `ptx_print/src/ptx_type_print.c`
    - Test: `test/ptx_print/test_type_print.c`

- [ ] **print_statement**
  - **Purpose**: Prints a `ptx_statement_t` AST node as a complete PTX statement text.
  - **Test**: Parse statement text → Print AST → Compare (modulo whitespace).

- [ ] **print_expression**
  - **Purpose**: Prints a `ptx_expr_t` AST node as a constant expression text (e.g., `1 + 2`, `!0`).
  - **Test**: Parse expression text → Print AST → Compare (modulo whitespace).

- [ ] **print_comment**
  - **Purpose**: Prints a `comment_t` AST node as a comment text (e.g., `// my comment`).
  - **Test**: Parse comment text → Print AST → Compare (modulo whitespace).

- [ ] **print_c_directive**
  - **Purpose**: Prints a `c_directive_t` AST node as a C-style directive text (e.g., `#include`, `#define`).
  - **Test**: Parse C directive text → Print AST → Compare (modulo whitespace).

## Implementation Plan

1. Create a new directory `ptx_print` with similar structure to `ptx_parse`
2. Implement header files in `include/ptx_print/`
3. Implement source files in `src/`
4. Create tests in the `test/` directory
5. Update CMake files to include the new directory and build the tests 