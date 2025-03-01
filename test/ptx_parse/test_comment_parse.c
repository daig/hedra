#include <ptx_parse/ptx_comment_parse.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void test_cpp_style_comments() {
    printf("Testing C++-style comment parsing...\n");
    
    comment_t* comment = NULL;
    int pos = 0;
    
    // Test basic C++-style comment
    const char* input1 = "// This is a C++ style comment";
    printf("Parsing '%s'\n", input1);
    assert(parse_comment(input1, &pos, &comment) == true);
    assert(comment != NULL);
    printf("Parsed comment: '%s'\n", comment->text);
    assert(strcmp(comment->text, " This is a C++ style comment") == 0);
    assert(pos == strlen(input1));
    free_comment(comment);
    
    // Test C++-style comment with newline
    comment = NULL;
    pos = 0;
    const char* input2 = "// Comment with newline\n";
    printf("Parsing '%s'\n", input2);
    assert(parse_comment(input2, &pos, &comment) == true);
    assert(comment != NULL);
    printf("Parsed comment: '%s'\n", comment->text);
    assert(strcmp(comment->text, " Comment with newline") == 0);
    assert(pos == strlen(input2));
    free_comment(comment);
    
    // Test C++-style comment with trailing text
    comment = NULL;
    pos = 0;
    const char* input3 = "// First comment\nNext line";
    printf("Parsing '%s'\n", input3);
    assert(parse_comment(input3, &pos, &comment) == true);
    assert(comment != NULL);
    printf("Parsed comment: '%s'\n", comment->text);
    assert(strcmp(comment->text, " First comment") == 0);
    assert(pos == 17); // Position after the newline
    free_comment(comment);
    
    // Test C++-style comment with leading whitespace
    comment = NULL;
    pos = 0;
    const char* input4 = "   // Comment with leading whitespace";
    printf("Parsing '%s'\n", input4);
    assert(parse_comment(input4, &pos, &comment) == true);
    assert(comment != NULL);
    printf("Parsed comment: '%s'\n", comment->text);
    assert(strcmp(comment->text, " Comment with leading whitespace") == 0);
    assert(pos == strlen(input4));
    free_comment(comment);
    
    printf("C++-style comment parsing tests passed!\n");
}

void test_c_style_comments() {
    printf("Testing C-style comment parsing...\n");
    
    comment_t* comment = NULL;
    int pos = 0;
    
    // Test basic C-style comment
    const char* input1 = "/* This is a C style comment */";
    printf("Parsing '%s'\n", input1);
    assert(parse_comment(input1, &pos, &comment) == true);
    assert(comment != NULL);
    printf("Parsed comment: '%s'\n", comment->text);
    assert(strcmp(comment->text, " This is a C style comment ") == 0);
    assert(pos == strlen(input1));
    free_comment(comment);
    
    // Test multi-line C-style comment
    comment = NULL;
    pos = 0;
    const char* input2 = "/* This is a\n multi-line\n comment */";
    printf("Parsing '%s'\n", input2);
    assert(parse_comment(input2, &pos, &comment) == true);
    assert(comment != NULL);
    printf("Parsed comment: '%s'\n", comment->text);
    assert(strcmp(comment->text, " This is a\n multi-line\n comment ") == 0);
    assert(pos == strlen(input2));
    free_comment(comment);
    
    // Test C-style comment with leading whitespace
    comment = NULL;
    pos = 0;
    const char* input3 = "   /* Comment with leading whitespace */";
    printf("Parsing '%s'\n", input3);
    assert(parse_comment(input3, &pos, &comment) == true);
    assert(comment != NULL);
    printf("Parsed comment: '%s'\n", comment->text);
    assert(strcmp(comment->text, " Comment with leading whitespace ") == 0);
    assert(pos == strlen(input3));
    free_comment(comment);
    
    // Test C-style comment with trailing text
    comment = NULL;
    pos = 0;
    const char* input4 = "/* First comment */Next text";
    printf("Parsing '%s'\n", input4);
    assert(parse_comment(input4, &pos, &comment) == true);
    assert(comment != NULL);
    printf("Parsed comment: '%s'\n", comment->text);
    assert(strcmp(comment->text, " First comment ") == 0);
    printf("Position after parsing: %d\n", pos);
    assert(pos == strlen("/* First comment */")); // Position after the closing */
    free_comment(comment);
    
    printf("C-style comment parsing tests passed!\n");
}

void test_invalid_comments() {
    printf("Testing invalid comment parsing...\n");
    
    comment_t* comment = NULL;
    int pos = 0;
    
    // Test string without comment
    const char* input1 = "This is not a comment";
    assert(parse_comment(input1, &pos, &comment) == false);
    
    // Test empty string
    pos = 0;
    const char* input2 = "";
    assert(parse_comment(input2, &pos, &comment) == false);
    
    // Test just whitespace
    pos = 0;
    const char* input3 = "   ";
    assert(parse_comment(input3, &pos, &comment) == false);
    
    // Test incomplete C++-style comment (just //)
    pos = 0;
    const char* input4 = "//";
    assert(parse_comment(input4, &pos, &comment) == true); // This should still parse, just with empty text
    assert(comment != NULL);
    assert(strcmp(comment->text, "") == 0);
    free_comment(comment);
    
    // Test incomplete C-style comment (missing closing */)
    pos = 0;
    comment = NULL;
    const char* input5 = "/* Unclosed comment";
    assert(parse_comment(input5, &pos, &comment) == false);
    
    // Test NULL input
    pos = 0;
    assert(parse_comment(NULL, &pos, &comment) == false);
    
    // Test NULL output pointer
    pos = 0;
    assert(parse_comment("// Comment", &pos, NULL) == false);
    
    // Test NULL position pointer
    assert(parse_comment("// Comment", NULL, &comment) == false);
    
    printf("Invalid comment parsing tests passed!\n");
}

int main() {
    printf("Running comment parser tests...\n");
    
    test_cpp_style_comments();
    test_c_style_comments();
    test_invalid_comments();
    
    printf("All comment parser tests passed!\n");
    return 0;
} 