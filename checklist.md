# PTX Parser Checklist

This document provides a checklist of all the parsing functions required for the "hedra" project's PTX (Parallel Thread Execution) parser, including those needed for intermediate Abstract Syntax Tree (AST) types. The project is focused on parsing PTX, an assembly language for NVIDIA GPUs, and the checklist is derived from the project’s directory structure, file map, and AST definitions.

The goal is to identify parsing functions that will process PTX source code into corresponding AST nodes, as defined in the `ptx_ast` module. These functions will form the core of the parser, handling various syntactic elements such as directives, instructions, labels, constants, identifiers, types, statements, expressions, initializers, variable declarations, comments, and C-style directives.

Below is the checklist, organized as a series of tasks with descriptions of each parsing function’s purpose. Each item is marked with a checkbox `[ ]` to indicate that implementation is pending, unless otherwise noted.

## Parsing Functions

- [ ] **parse_directive**
  - **Purpose**: Parses PTX directives (e.g., `.version`, `.target`, `.func`, `.entry`) and constructs a `ptx_directive_t` AST node.
  - **Details**: Directives define the environment, scope, or entry points in PTX code (e.g., `.version 7.0`, `.entry kernel()`). The function must recognize directive keywords from the `ptx_directive.h` enum and handle their specific syntax.

- [ ] **parse_instruction**
  - **Purpose**: Parses PTX instructions (e.g., `add`, `mul`, `ld`) and constructs a `ptx_instruction_t` AST node.
  - **Details**: Instructions are the operational core of PTX, such as `add.s32 %r0, %r1, %r2`. This function needs to identify instruction opcodes from `ptx_instruction.h` and parse their operands, which may include registers, constants, or types.

- [ ] **parse_label**
  - **Purpose**: Parses labels (e.g., `my_label:`) and constructs a `ptx_label_t` AST node.
  - **Details**: Labels are identifiers followed by a colon, used for control flow (e.g., branching). The function extracts the label name and ensures it conforms to PTX naming rules.

- [ ] **parse_constant**
  - **Purpose**: Parses constant values (e.g., integers, floats, predicates) and constructs a `ptx_constant_t` AST node.
  - **Details**: Constants appear in instructions or initializers (e.g., `42`, `3.14`, `true`). This function may include sub-functions like:
    - `parse_integer_constant` for signed/unsigned integers (e.g., `.s64 -42`, `.u32 0xFF`).
    - `parse_float_constant` for floating-point values (e.g., `.f32 1.5`, `.f64 3.14159`).
    - `parse_predicate_constant` for predicate values (e.g., `true`, `false`).
  - **Note**: Partially implemented in `ptx_constant_parse.c`, as indicated by the `ptx_parse` directory.

- [ ] **parse_identifier**
  - **Purpose**: Parses identifiers (e.g., variable names, register names) and constructs a `ptx_identifier_t` AST node.
  - **Details**: Identifiers can be user-defined (e.g., `my_var`) or predefined (e.g., `%clock`), as per `ptx_identifier.h` and `ptx_predefined_identifier.h`. The function must distinguish between these categories and validate syntax.

- [ ] **parse_type**
  - **Purpose**: Parses type specifiers (e.g., `.u32`, `.f64`, `.texref`) and constructs a `ptx_type_t` AST node.
  - **Details**: Types include fundamental types (e.g., `.s16`, `.f32`), graphics types (e.g., `.texref`), and alternate float formats (e.g., `.bf16`), as defined in `ptx_type.h`, `ptx_type_fundamental.h`, `ptx_type_graphics.h`, and `ptx_alternate_float_format.h`. This function must handle type keywords and their context.

- [ ] **parse_statement**
  - **Purpose**: Parses a complete PTX statement and constructs a `ptx_statement_t` AST node.
  - **Details**: A statement combines an optional label with either a directive or instruction (e.g., `my_label: add.s32 %r0, %r1, %r2`). This function integrates `parse_label`, `parse_directive`, and `parse_instruction` to build the AST node, as per `ptx_statement.h`.

- [ ] **parse_expression**
  - **Purpose**: Parses constant expressions (e.g., `1 + 2`, `!true`) and constructs a `ptx_expr_t` AST node.
  - **Details**: Expressions are used in initializers or operands, supporting unary, binary, and ternary operators (e.g., `+`, `*`, `?:`), as defined in `ptx_constant_expr.h`. The function must handle operator precedence and build an expression tree.

- [ ] **parse_initializer**
  - **Purpose**: Parses initializers for variable declarations (e.g., `= 42`, `= {1, 2}`) and constructs a `ptx_initializer_t` AST node.
  - **Details**: Initializers can be scalar values, vectors, arrays, variable addresses, or masked values (e.g., `.global .s32 arr[2] = {1, 2}`, `.global .u8 byte = mask(foo, 0xFF)`). This function, based on `ptx_type_decl.h`, must recursively parse nested structures and handle various initializer kinds.

- [ ] **parse_variable_declaration**
  - **Purpose**: Parses variable declarations (e.g., `.global .u32 my_var = 42`) and constructs a `ptx_type_decl_t` AST node.
  - **Details**: Declarations include a state space (e.g., `.global`), type, shape (scalar, vector, or array), name, and optional initializer, as per `ptx_type_decl.h`. This function integrates `parse_type`, `parse_identifier`, and `parse_initializer`, and handles array shapes (e.g., `[19][19]`) and vector sizes (e.g., `.v4`).

- [ ] **parse_comment**
  - **Purpose**: Parses comments (e.g., `// my comment`) and constructs a `comment_t` AST node.
  - **Details**: Comments are informational and typically ignored during execution but preserved in the AST for documentation or analysis, as per `comment.h`. The function extracts the comment text.

- [ ] **parse_c_directive**
  - **Purpose**: Parses C-style directives (e.g., `#include`, `#define`) and constructs a `c_directive_t` AST node.
  - **Details**: If PTX code includes C preprocessor directives (hinted by `c_directive.h`), this function identifies and parses them. This is optional, depending on project requirements.

## Notes

- **Existing Implementation**: The `parse_constant` function is partially implemented in `ptx_constant_parse.c` within the `ptx_parse` directory, likely covering integer, float, and predicate constants, as suggested by `test_constant_parse.c`.
- **Scope Limitation**: This checklist focuses on parsing functions for individual AST types rather than higher-level constructs like entire modules or function bodies, which might be composed from these functions (e.g., `parse_module` could use `parse_statement` repeatedly).
- **Assumptions**: The parser assumes a tokenizer or lexer provides input tokens, which these functions then process into AST nodes. Error handling and syntax validation are implementation details not listed here.
- **PTX Specification**: The functions must align with the PTX language specification (e.g., directives from `ptx_directive.h`, instructions from `ptx_instruction.h`), ensuring all syntactic elements are covered.

This checklist provides a comprehensive foundation for building the PTX parser, ensuring all intermediate AST types in the `hedra` project are accounted for. Each function will need to be implemented with detailed syntax rules and integrated into the broader parsing framework.