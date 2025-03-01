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

# Test for initializer helpers (uses both prelude and ptx_ast)
add_executable(initializer_test prelude/test_initializer.c)
target_link_libraries(initializer_test prelude ptx_ast)
target_include_directories(initializer_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS initializer_test DESTINATION bin)

# Tests for ptx_parse
add_executable(constant_parse_test ptx_parse/test_constant_parse.c)
target_link_libraries(constant_parse_test ptx_parse)
target_include_directories(constant_parse_test PRIVATE ${CMAKE_SOURCE_DIR}/ptx_ast/include)
install(TARGETS constant_parse_test DESTINATION bin)

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