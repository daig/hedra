# Tests for PTX Project

# Tests for ptx_ast
add_executable(expr_test ptx_ast/test_constant_expr.c)
target_link_libraries(expr_test ptx_ast)
install(TARGETS expr_test DESTINATION bin)

add_executable(ptx_type_test ptx_ast/test_ptx_type.c)
target_link_libraries(ptx_type_test ptx_ast)
target_include_directories(ptx_type_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS ptx_type_test DESTINATION bin)

add_executable(ptx_packed_float_test ptx_ast/test_ptx_packed_float.c)
target_link_libraries(ptx_packed_float_test ptx_ast)
target_include_directories(ptx_packed_float_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS ptx_packed_float_test DESTINATION bin)

add_executable(graphics_types_test ptx_ast/test_graphics_types.c)
target_link_libraries(graphics_types_test ptx_ast)
target_include_directories(graphics_types_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS graphics_types_test DESTINATION bin)

add_executable(ast_test ptx_ast/test_ast.c)
target_link_libraries(ast_test ptx_ast ptx_parse)
target_include_directories(ast_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include 
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
)
install(TARGETS ast_test DESTINATION bin)

# Tests for prelude
add_executable(prelude_test prelude/test_prelude.c)
target_link_libraries(prelude_test prelude)
install(TARGETS prelude_test DESTINATION bin)

add_executable(initializer_array_test prelude/test_initializer_array.c)
target_link_libraries(initializer_array_test prelude)
install(TARGETS initializer_array_test DESTINATION bin)

# Tests for ptx_parse
add_executable(constant_parse_test ptx_parse/test_constant_parse.c)
target_link_libraries(constant_parse_test ptx_parse)
target_include_directories(constant_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS constant_parse_test DESTINATION bin)

# New test for constant expression parsing
add_executable(constant_expr_parse_test ptx_parse/test_constant_expr_parse.c)
target_link_libraries(constant_expr_parse_test ptx_parse)
target_include_directories(constant_expr_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS constant_expr_parse_test DESTINATION bin)

# New test for directive parsing
add_executable(directive_parse_test ptx_parse/test_directive_parse.c)
target_link_libraries(directive_parse_test ptx_parse)
target_include_directories(directive_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS directive_parse_test DESTINATION bin)

# New test for instruction parsing
add_executable(instruction_parse_test ptx_parse/test_instruction_parse.c)
target_link_libraries(instruction_parse_test ptx_parse)
target_include_directories(instruction_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS instruction_parse_test DESTINATION bin)

# New test for label parsing
add_executable(label_parse_test ptx_parse/test_label_parse.c)
target_link_libraries(label_parse_test ptx_parse)
target_include_directories(label_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS label_parse_test DESTINATION bin)

# New test for identifier parsing
add_executable(identifier_parse_test ptx_parse/test_identifier_parse.c)
target_link_libraries(identifier_parse_test ptx_parse)
target_include_directories(identifier_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS identifier_parse_test DESTINATION bin)

# New test for type parsing
add_executable(type_parse_test ptx_parse/test_type_parse.c)
target_link_libraries(type_parse_test ptx_parse)
target_include_directories(type_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS type_parse_test DESTINATION bin)

# New test for statement parsing
add_executable(statement_parse_test ptx_parse/test_statement_parse.c)
target_link_libraries(statement_parse_test ptx_parse)
target_include_directories(statement_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS statement_parse_test DESTINATION bin)

# New test for comment parsing
add_executable(comment_parse_test ptx_parse/test_comment_parse.c)
target_link_libraries(comment_parse_test ptx_parse)
target_include_directories(comment_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS comment_parse_test DESTINATION bin)

# New test for C directive parsing
add_executable(c_directive_parse_test ptx_parse/test_c_directive_parse.c)
target_link_libraries(c_directive_parse_test ptx_parse)
target_include_directories(c_directive_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS c_directive_parse_test DESTINATION bin)

# Tests for ptx_print
add_executable(directive_print_test ptx_print/test_directive_print.c)
target_link_libraries(directive_print_test ptx_print ptx_parse)
target_include_directories(directive_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS directive_print_test DESTINATION bin)

# New test for instruction printing
add_executable(instruction_print_test ptx_print/test_instruction_print.c)
target_link_libraries(instruction_print_test ptx_print ptx_parse)
target_include_directories(instruction_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS instruction_print_test DESTINATION bin)

# New test for label printing
add_executable(label_print_test ptx_print/test_label_print.c)
target_link_libraries(label_print_test ptx_print ptx_parse)
target_include_directories(label_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS label_print_test DESTINATION bin)

# New test for constant printing
add_executable(constant_print_test ptx_print/test_constant_print.c)
target_link_libraries(constant_print_test ptx_print ptx_parse)
target_include_directories(constant_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS constant_print_test DESTINATION bin)

# New test for expression printing
add_executable(expr_print_test ptx_print/test_expr_print.c)
target_link_libraries(expr_print_test ptx_print ptx_parse)
target_include_directories(expr_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS expr_print_test DESTINATION bin)

# New test for comment printing
add_executable(comment_print_test ptx_print/test_comment_print.c)
target_link_libraries(comment_print_test ptx_print ptx_parse)
target_include_directories(comment_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS comment_print_test DESTINATION bin)

# New test for identifier printing
add_executable(identifier_print_test ptx_print/test_identifier_print.c)
target_link_libraries(identifier_print_test ptx_print ptx_parse)
target_include_directories(identifier_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS identifier_print_test DESTINATION bin)

# New test for type printing
add_executable(type_print_test ptx_print/test_type_print.c)
target_link_libraries(type_print_test ptx_print ptx_parse)
target_include_directories(type_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS type_print_test DESTINATION bin)

# New test for statement printing
add_executable(statement_print_test ptx_print/test_statement_print.c)
target_link_libraries(statement_print_test ptx_print ptx_parse)
target_include_directories(statement_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS statement_print_test DESTINATION bin)

# New test for C directive printing
add_executable(c_directive_print_test ptx_print/test_c_directive_print.c)
target_link_libraries(c_directive_print_test ptx_print ptx_parse)
target_include_directories(c_directive_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS c_directive_print_test DESTINATION bin)

# Add the new test for initializer value printing
add_executable(initializer_value_print_test ptx_print/test_initializer_value_print.c)
target_link_libraries(initializer_value_print_test ptx_print ptx_parse)
target_include_directories(initializer_value_print_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS initializer_value_print_test DESTINATION bin)

# Add after other parse tests
add_executable(initializer_value_parse_test ptx_parse/test_initializer_value_parse.c)
target_link_libraries(initializer_value_parse_test ptx_parse ptx_print)
target_include_directories(initializer_value_parse_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/ptx_ast/include
    ${CMAKE_SOURCE_DIR}/ptx_parse/include
    ${CMAKE_SOURCE_DIR}/ptx_print/include
)
install(TARGETS initializer_value_parse_test DESTINATION bin) 