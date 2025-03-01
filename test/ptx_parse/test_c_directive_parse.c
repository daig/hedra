#include <ptx_parse/c_directive_parse.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void test_include_directive() {
    const char* input = "#include <stdio.h>\nNext line";
    int pos = 0;
    enum c_directive_t directive;
    char* content = NULL;
    
    bool result = parse_c_directive(input, &pos, &directive, &content);
    
    assert(result == true);
    assert(directive == C_DIRECTIVE_INCLUDE);
    assert(strcmp(content, "<stdio.h>") == 0);
    assert(pos == 19); // Position after the newline
    
    free_c_directive_content(content);
    printf("test_include_directive passed\n");
}

void test_define_directive() {
    const char* input = "#define MAX_SIZE 100\nNext line";
    int pos = 0;
    enum c_directive_t directive;
    char* content = NULL;
    
    bool result = parse_c_directive(input, &pos, &directive, &content);
    
    assert(result == true);
    assert(directive == C_DIRECTIVE_DEFINE);
    assert(strcmp(content, "MAX_SIZE 100") == 0);
    assert(pos == 21); // Position after the newline
    
    free_c_directive_content(content);
    printf("test_define_directive passed\n");
}

void test_if_directive() {
    const char* input = "#if NDEBUG\nSome code\n#endif";
    int pos = 0;
    enum c_directive_t directive;
    char* content = NULL;
    
    bool result = parse_c_directive(input, &pos, &directive, &content);
    
    assert(result == true);
    assert(directive == C_DIRECTIVE_IF);
    assert(strcmp(content, "NDEBUG") == 0);
    
    free_c_directive_content(content);
    printf("test_if_directive passed\n");
}

void test_else_directive() {
    const char* input = "#else\nSome other code";
    int pos = 0;
    enum c_directive_t directive;
    char* content = NULL;
    
    bool result = parse_c_directive(input, &pos, &directive, &content);
    
    assert(result == true);
    assert(directive == C_DIRECTIVE_ELSE);
    assert(content != NULL);
    
    free_c_directive_content(content);
    printf("test_else_directive passed\n");
}

void test_endif_directive() {
    const char* input = "#endif\nNext line";
    int pos = 0;
    enum c_directive_t directive;
    char* content = NULL;
    
    bool result = parse_c_directive(input, &pos, &directive, &content);
    
    assert(result == true);
    assert(directive == C_DIRECTIVE_ENDIF);
    assert(content != NULL);
    
    free_c_directive_content(content);
    printf("test_endif_directive passed\n");
}

void test_whitespace_handling() {
    const char* input = "   #   include    <stdlib.h>   \nNext line";
    int pos = 0;
    enum c_directive_t directive;
    char* content = NULL;
    
    bool result = parse_c_directive(input, &pos, &directive, &content);
    
    assert(result == true);
    assert(directive == C_DIRECTIVE_INCLUDE);
    assert(strcmp(content, "<stdlib.h>   ") == 0);
    
    free_c_directive_content(content);
    printf("test_whitespace_handling passed\n");
}

void test_non_directive() {
    const char* input = "This is not a directive";
    int pos = 0;
    enum c_directive_t directive;
    char* content = NULL;
    
    bool result = parse_c_directive(input, &pos, &directive, &content);
    
    assert(result == false);
    assert(pos == 0); // Position should not change
    
    printf("test_non_directive passed\n");
}

void test_unknown_directive() {
    const char* input = "#unknown directive\nNext line";
    int pos = 0;
    enum c_directive_t directive;
    char* content = NULL;
    
    bool result = parse_c_directive(input, &pos, &directive, &content);
    
    assert(result == false);
    
    printf("test_unknown_directive passed\n");
}

int main() {
    test_include_directive();
    test_define_directive();
    test_if_directive();
    test_else_directive();
    test_endif_directive();
    test_whitespace_handling();
    test_non_directive();
    test_unknown_directive();
    
    printf("All tests passed!\n");
    return 0;
} 