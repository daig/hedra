# PTX Parser Specification

## 1. Introduction

This document specifies the grammar and syntax for parsing the NVIDIA Parallel Thread Execution (PTX) language through section 5.4.4 (Initializers). This specification is designed in a bottom-up fashion to facilitate incremental parser implementation and testing.

The grammar is presented in a format suitable for C implementation, with clear definitions of tokens, expressions, and syntactic structures. Each section provides formal grammar rules followed by examples where appropriate.

## 2. Lexical Elements

### 2.1 Character Set

PTX source programs are written using the ASCII character set. Comments may contain UTF-8 encoded characters.

### 2.2 Comments

PTX supports C and C++ style comments:

```
// Single-line comment
/* Multi-line
   comment */
```

### 2.3 Whitespace

Whitespace (spaces, tabs, newlines) is ignored except as a token separator.

### 2.4 Tokens

The basic lexical units of PTX are:

#### 2.4.1 Keywords

Keywords are prefixed with a period (`.`):

```
.address_size  .global      .ptr
.align         .loc         .reg
.branchtargets .local       .sreg
.callprototype .maxnreg     .struct
.calltargets   .maxntid     .target
.const         .minnctapersm .tex
.entry         .noreturn    .u16
.extern        .param       .u32
.file          .pragma      .u64
.func          .s16         .version
// ... and many more
```

#### 2.4.2 Identifiers

Identifiers follow this pattern:

```
identifier ::= [a-zA-Z_$%] [a-zA-Z0-9_$]*
```

Valid identifiers start with a letter, underscore, dollar sign, or percent sign, followed by zero or more letters, digits, underscores, or dollar signs.

#### 2.4.3 Integer Literals

```
integer_literal ::= decimal_literal | hex_literal | octal_literal | binary_literal
decimal_literal ::= [0-9]+ ('U')?
hex_literal     ::= '0' [xX] [0-9a-fA-F]+ ('U')?
octal_literal   ::= '0' [0-7]+ ('U')?
binary_literal  ::= '0' [bB] [0-1]+ ('U')?
```

Examples:
- Decimal: `42`, `123U`
- Hexadecimal: `0x2A`, `0xFFU`
- Octal: `052`, `077U`
- Binary: `0b101010`, `0b1111U`

#### 2.4.4 Floating-Point Literals

```
float_literal ::= decimal_float | hex_float
decimal_float ::= [0-9]* '.' [0-9]+ ([eE] [+-]? [0-9]+)?
                | [0-9]+ ([eE] [+-]? [0-9]+)
hex_float_f32 ::= '0' [fF] [0-9a-fA-F]{8}
hex_float_f64 ::= '0' [dD] [0-9a-fA-F]{16}
```

Examples:
- Decimal: `3.14159`, `1.0e-10`, `5e3`
- Hexadecimal (32-bit): `0F3f800000` (represents 1.0f)
- Hexadecimal (64-bit): `0D3ff0000000000000` (represents 1.0)

#### 2.4.5 Predicate Literals

```
predicate_literal ::= 'true' | 'false'
```

#### 2.4.6 Operators and Punctuators

```
+  -  *  /  %  &  |  ^  ~  !  =  <  >  ,  ;  :  (  )  [  ]  {  }
+=  -=  *=  /=  %=  &=  |=  ^=  <<  >>  ==  !=  <=  >=  &&  ||  ?:
```

## 3. Basic Grammar Elements

### 3.1 Statements

PTX code consists of directive statements and instruction statements.

```
statement ::= directive_statement
            | instruction_statement
```

### 3.2 Directive Statements

Directive statements provide information to the PTX compiler, including variable declarations, function declarations, and other metadata.

```
directive_statement ::= directive_token directive_params ';'
directive_token     ::= '.' identifier
```

### 3.3 Type System

#### 3.3.1 Fundamental Types

```
fundamental_type ::= '.pred'                            // Predicate type
                   | '.b' ('8' | '16' | '32' | '64' | '128')  // Bit types
                   | '.s' ('8' | '16' | '32' | '64')    // Signed integer types
                   | '.u' ('8' | '16' | '32' | '64')    // Unsigned integer types
                   | '.f' ('16' | '32' | '64')          // Floating-point types
                   | '.f16x2'                           // Packed float
```

#### 3.3.2 Vector Types

```
vector_type ::= '.v' ('2' | '4') space fundamental_type
```

Examples:
- `.v2 .f32`: Vector of 2 floats
- `.v4 .u8`: Vector of 4 unsigned bytes

### 3.4 State Spaces

```
state_space ::= '.reg'       // Register space
              | '.sreg'      // Special register space
              | '.const'     // Constant memory space
              | '.global'    // Global memory space
              | '.local'     // Local memory space
              | '.param'     // Parameter memory space
              | '.shared'    // Shared memory space
```

## 4. Variable Declarations

### 4.1 Basic Variable Declaration

```
variable_declaration ::= state_space space type_spec space variable_name initializer? ';'
type_spec            ::= vector_type | fundamental_type
variable_name        ::= identifier
```

Examples:
```
.reg .u32 r1;
.global .f32 gvar;
.const .s32 cval;
```

### 4.2 Array Declarations

```
array_declaration ::= state_space space type_spec space variable_name array_spec initializer? ';'
array_spec        ::= '[' array_dim ']' ('[' array_dim ']')*
array_dim         ::= integer_constant_expression | /* empty */
```

Examples:
```
.global .u32 array[5];             // 1D array of 5 elements
.const .f32 matrix[3][4];          // 2D array, 3×4 matrix
.global .s32 dynamic[];            // 1D array with unspecified size
```

## 5. Expressions

### 5.1 Constant Expressions

```
constant_expression ::= conditional_expression
```

### 5.2 Primary Expressions

```
primary_expression ::= identifier
                     | integer_literal
                     | float_literal
                     | predicate_literal
                     | '(' expression ')'
```

### 5.3 Unary Expressions

```
unary_expression ::= primary_expression
                   | '+' unary_expression
                   | '-' unary_expression
                   | '!' unary_expression
                   | '~' unary_expression
```

### 5.4 Binary Expressions

```
multiplicative_expression ::= unary_expression
                           | multiplicative_expression '*' unary_expression
                           | multiplicative_expression '/' unary_expression
                           | multiplicative_expression '%' unary_expression

additive_expression ::= multiplicative_expression
                      | additive_expression '+' multiplicative_expression
                      | additive_expression '-' multiplicative_expression

shift_expression ::= additive_expression
                   | shift_expression '<<' additive_expression
                   | shift_expression '>>' additive_expression

relational_expression ::= shift_expression
                        | relational_expression '<' shift_expression
                        | relational_expression '>' shift_expression
                        | relational_expression '<=' shift_expression
                        | relational_expression '>=' shift_expression

equality_expression ::= relational_expression
                      | equality_expression '==' relational_expression
                      | equality_expression '!=' relational_expression

and_expression ::= equality_expression
                 | and_expression '&' equality_expression

exclusive_or_expression ::= and_expression
                          | exclusive_or_expression '^' and_expression

inclusive_or_expression ::= exclusive_or_expression
                          | inclusive_or_expression '|' exclusive_or_expression

logical_and_expression ::= inclusive_or_expression
                         | logical_and_expression '&&' inclusive_or_expression

logical_or_expression ::= logical_and_expression
                        | logical_or_expression '||' logical_and_expression
```

### 5.5 Conditional Expression

```
conditional_expression ::= logical_or_expression
                         | logical_or_expression '?' expression ':' conditional_expression
```

### 5.6 Assignment Expression

```
assignment_expression ::= conditional_expression
                        | unary_expression assignment_operator assignment_expression

assignment_operator ::= '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '|=' | '^='
```

## 6. Initializers (Section 5.4.4)

Initializers in PTX allow variables to specify an initial value, using syntax similar to C/C++.

### 6.1 Basic Initializer Syntax

```
initializer ::= '=' initializer_value
              | /* empty */

initializer_value ::= scalar_initializer
                    | vector_initializer
                    | array_initializer
```

### 6.2 Scalar Initializers

```
scalar_initializer ::= constant_expression
                     | variable_addr_expression
                     | function_addr_expression
                     | masked_expression
```

Examples:
```
.const .u32 foo = 42;
.global .f32 pi = 3.14159;
```

### 6.3 Vector Initializers

```
vector_initializer ::= '{' initializer_list '}'
initializer_list  ::= scalar_initializer (',' scalar_initializer)*
```

Example:
```
.global .v2 .f32 v2 = {1.0, 2.0};
```

### 6.4 Array Initializers

```
array_initializer ::= '{' array_initializer_list '}'
array_initializer_list ::= array_element (',' array_element)* ','?
array_element ::= scalar_initializer
                | array_initializer
                | vector_initializer
```

Examples:
```
.const .f32 vals[8] = {0.33, 0.25, 0.125};
.global .s32 x[3][2] = {{1,2}, {3}};
```

### 6.5 Variable Address Expressions

Variable names in initializers represent the address of the variable.

```
variable_addr_expression ::= identifier                // Base address
                           | identifier '+' integer_constant_expression  // Address with offset
                           | 'generic' '(' identifier ')'               // Generic address
                           | 'generic' '(' identifier ')' '+' integer_constant_expression // Generic address with offset
```

Examples:
```
.const .u32 foo = 42;
.global .u32 bar[] = {2, 3, 5};
.global .u32 p1 = foo;          // offset of foo in .const space
.global .u32 p2 = generic(foo); // generic address of foo
```

### 6.6 Function Address Expressions

Function names in initializers represent the address of the first instruction in the function.

```
function_addr_expression ::= function_identifier
```

Example:
```
.global .u32 func_ptr = my_kernel;  // Address of my_kernel function
```

### 6.7 Masked Expressions

The `mask()` operator extracts a specific byte from an address or integer value.

```
masked_expression ::= mask '(' masked_operand ')'
mask ::= '0xFF' | '0xFF00' | '0xFF0000' | '0xFF000000' | '0xFF00000000' | '0xFF0000000000' | '0xFF000000000000' | '0xFF00000000000000'
masked_operand ::= variable_addr_expression
                 | integer_constant_expression
```

Examples:
```
.global .u8 byte0 = 0xFF(foo);        // Extract byte 0 of foo's address
.global .u8 byte1 = 0xFF00(foo);      // Extract byte 1 of foo's address
.global .u8 val0 = 0xFF(0x12345678);  // Extract byte 0 (0x78)
```

### 6.8 Initializer Limitations

1. Initializers are only allowed for variables in `.const` and `.global` state spaces.
2. Variables in `.const` and `.global` spaces with no explicit initializer are initialized to zero by default.
3. Initializers are not allowed in external variable declarations.
4. Labels cannot be used in initializers.
5. Variables that hold addresses of variables or functions should be of type `.u8`, `.u32`, or `.u64`.
6. Type `.u8` is allowed only if the `mask()` operator is used.
7. Initializers are not allowed for types `.f16`, `.f16x2`, and `.pred`.

## 7. Implementation Recommendations

### 7.1 Parsing Strategy

1. Implement a lexical analyzer (lexer) that converts the input character stream into tokens.
2. Implement a recursive descent parser for the grammar rules.
3. For expressions, use the precedence climbing or operator precedence parsing algorithm.
4. Build an abstract syntax tree (AST) representing the parsed program structure.

### 7.2 Testing Approach

Given the bottom-up specification, testing should proceed in this order:

1. Test lexical analysis (tokenization) with simple inputs.
2. Test basic expressions and constant expressions.
3. Test variable declarations without initializers.
4. Test scalar initializers.
5. Test vector and array initializers.
6. Test complex initializers with variable addresses and functions.
7. Test a complete PTX module with various constructs.

Each stage should only proceed once the previous stage is working correctly.
