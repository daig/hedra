#include <ptx_print/ptx_comment_print.h>
#include <ptx_parse/ptx_comment_parse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// Helper function to trim whitespace from the beginning and end of a string
char* trim_whitespace(char* str) {
    if (!str) return NULL;
    
    // Trim leading space
    while (isspace(*str)) str++;
    
    if (*str == 0) return str;  // All spaces
    
    // Trim trailing space
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    
    // Write new null terminator
    *(end + 1) = 0;
    
    return str;
}

// Helper function to ensure a comment can be printed and reparsed with content intact
void test_comment_preservation(const char* original_comment) {
    comment_t* comment = NULL;
    int pos = 0;
    
    printf("Testing: %s\n", original_comment);
    
    // Parse the original comment
    assert(parse_comment(original_comment, &pos, &comment));
    printf("Parsed comment content: '%s'\n", comment->text);
    
    // Print the comment back to a string
    char buffer[1024];
    int result = sprint_comment(comment, buffer, sizeof(buffer));
    assert(result > 0);
    printf("Printed as: '%s'\n", buffer);
    
    // Parse the printed result again
    comment_t* reparsed_comment = NULL;
    int reparse_pos = 0;
    assert(parse_comment(buffer, &reparse_pos, &reparsed_comment));
    printf("Reparsed comment content: '%s'\n", reparsed_comment->text);
    
    // The core requirement: the comment content must be preserved
    assert(strcmp(comment->text, reparsed_comment->text) == 0);
    
    free_comment(comment);
    free_comment(reparsed_comment);
    printf("Test passed: content preserved correctly\n\n");
}

void test_cpp_style_comments() {
    printf("Testing C++-style comments...\n");
    
    // Test cases for C++-style comments
    const char* test_cases[] = {
        "// Simple comment",
        "// Comment with special chars: !@#$%^&*()",
        "//Empty content",
        "// Comment with trailing space   "
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        test_comment_preservation(test_cases[i]);
    }
    
    printf("All C++-style comment tests passed!\n\n");
}

void test_c_style_comments() {
    printf("Testing C-style comments...\n");
    
    // Test cases for C-style comments
    const char* test_cases[] = {
        "/* Simple C-style comment */",
        "/* Multi-line\n comment */",
        "/* Comment with special chars: !@#$%^&*() */",
        "/**/",  // Empty content
        "/* Comment with trailing space   */"
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        test_comment_preservation(test_cases[i]);
    }
    
    printf("All C-style comment tests passed!\n\n");
}

void test_mixed_comments() {
    printf("Testing mixed comment types...\n");
    
    // Test mixed cases
    const char* test_cases[] = {
        "// Single-line comment",
        "/* Single-line C-style comment */",
        "/* Multi-line\n   C-style\n   comment */"
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        test_comment_preservation(test_cases[i]);
    }
    
    printf("All mixed comment tests passed!\n\n");
}

int main() {
    printf("Running comment printer tests...\n\n");
    
    test_cpp_style_comments();
    test_c_style_comments();
    test_mixed_comments();
    
    printf("All comment printer tests passed!\n");
    return 0;
} 