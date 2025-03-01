#include <ptx_print/c_directive_print.h>
#include <ptx_parse/c_directive_parse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * Test parsing and printing a C directive roundtrip.
 * 
 * @param original_str The original C directive string.
 * @param expected_directive The expected directive type.
 * @param expected_content The expected content (or NULL if no specific content is expected).
 */
void test_directive_roundtrip(const char* original_str, enum c_directive_t expected_directive, const char* expected_content) {
    // Parse the directive
    int pos = 0;
    enum c_directive_t directive;
    char* content = NULL;
    
    bool parse_result = parse_c_directive(original_str, &pos, &directive, &content);
    assert(parse_result == true);
    assert(directive == expected_directive);
    
    if (expected_content != NULL) {
        assert(strcmp(content, expected_content) == 0);
    }
    
    // Print the directive back to a string
    char printed_str[256];
    int print_result = sprint_c_directive(printed_str, sizeof(printed_str), directive, content);
    assert(print_result > 0);
    
    // Parse the printed string
    int new_pos = 0;
    enum c_directive_t new_directive;
    char* new_content = NULL;
    
    bool new_parse_result = parse_c_directive(printed_str, &new_pos, &new_directive, &new_content);
    assert(new_parse_result == true);
    assert(new_directive == directive);
    
    // Content might have different whitespace, so we just make sure
    // that parsing the printed result gives us back the same directive type
    
    // Clean up
    free_c_directive_content(content);
    free_c_directive_content(new_content);
    
    printf("Roundtrip test passed for: %s\n", original_str);
}

void test_include_directive() {
    test_directive_roundtrip("#include <stdio.h>", C_DIRECTIVE_INCLUDE, "<stdio.h>");
}

void test_define_directive() {
    test_directive_roundtrip("#define MAX_SIZE 100", C_DIRECTIVE_DEFINE, "MAX_SIZE 100");
}

void test_if_directive() {
    test_directive_roundtrip("#if NDEBUG", C_DIRECTIVE_IF, "NDEBUG");
}

void test_ifdef_directive() {
    test_directive_roundtrip("#ifdef DEBUG", C_DIRECTIVE_IFDEF, "DEBUG");
}

void test_else_directive() {
    test_directive_roundtrip("#else", C_DIRECTIVE_ELSE, NULL);
}

void test_endif_directive() {
    test_directive_roundtrip("#endif", C_DIRECTIVE_ENDIF, NULL);
}

void test_whitespace_handling() {
    test_directive_roundtrip("#   include    <stdlib.h>   ", C_DIRECTIVE_INCLUDE, "<stdlib.h>   ");
}

int main() {
    test_include_directive();
    test_define_directive();
    test_if_directive();
    test_ifdef_directive();
    test_else_directive();
    test_endif_directive();
    test_whitespace_handling();
    
    printf("All tests passed!\n");
    return 0;
} 