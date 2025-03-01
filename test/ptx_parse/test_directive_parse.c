#include <ptx_parse/ptx_directive_parse.h>
#include <ptx_ast/ptx_directive.h>
#include <stdio.h>
#include <assert.h>

void test_basic_directives() {
    printf("Testing basic directive parsing...\n");
    
    ptx_directive_t directive;
    
    // Test common directives
    assert(parse_directive("version", &directive) == true);
    assert(directive == PTX_DIRECTIVE_VERSION);
    
    assert(parse_directive("target", &directive) == true);
    assert(directive == PTX_DIRECTIVE_TARGET);
    
    assert(parse_directive("entry", &directive) == true);
    assert(directive == PTX_DIRECTIVE_ENTRY);
    
    assert(parse_directive("func", &directive) == true);
    assert(directive == PTX_DIRECTIVE_FUNC);
    
    printf("Basic directive parsing tests passed!\n");
}

void test_case_insensitivity() {
    printf("Testing case-insensitive directive parsing...\n");
    
    ptx_directive_t directive;
    
    // Test various case combinations
    assert(parse_directive("VERSION", &directive) == true);
    assert(directive == PTX_DIRECTIVE_VERSION);
    
    assert(parse_directive("Target", &directive) == true);
    assert(directive == PTX_DIRECTIVE_TARGET);
    
    assert(parse_directive("gLoBAl", &directive) == true);
    assert(directive == PTX_DIRECTIVE_GLOBAL);
    
    printf("Case-insensitive directive parsing tests passed!\n");
}

void test_memory_directives() {
    printf("Testing memory-related directive parsing...\n");
    
    ptx_directive_t directive;
    
    // Test memory-related directives
    assert(parse_directive("shared", &directive) == true);
    assert(directive == PTX_DIRECTIVE_SHARED);
    
    assert(parse_directive("local", &directive) == true);
    assert(directive == PTX_DIRECTIVE_LOCAL);
    
    assert(parse_directive("global", &directive) == true);
    assert(directive == PTX_DIRECTIVE_GLOBAL);
    
    assert(parse_directive("const", &directive) == true);
    assert(directive == PTX_DIRECTIVE_CONST);
    
    printf("Memory-related directive parsing tests passed!\n");
}

void test_cluster_directives() {
    printf("Testing cluster-related directive parsing...\n");
    
    ptx_directive_t directive;
    
    // Test cluster-related directives
    assert(parse_directive("maxclusterrank", &directive) == true);
    assert(directive == PTX_DIRECTIVE_MAXCLUSTERRANK);
    
    assert(parse_directive("reqnctapercluster", &directive) == true);
    assert(directive == PTX_DIRECTIVE_REQNCTAPERCLUSTER);
    
    assert(parse_directive("explicitcluster", &directive) == true);
    assert(directive == PTX_DIRECTIVE_EXPLICITCLUSTER);
    
    printf("Cluster-related directive parsing tests passed!\n");
}

void test_all_directives() {
    printf("Testing all remaining directives...\n");
    
    ptx_directive_t directive;
    
    // Test all other directives not covered in previous tests
    assert(parse_directive("address_size", &directive) == true);
    assert(directive == PTX_DIRECTIVE_ADDRESS_SIZE);
    
    assert(parse_directive("maxnreg", &directive) == true);
    assert(directive == PTX_DIRECTIVE_MAXNREG);
    
    assert(parse_directive("section", &directive) == true);
    assert(directive == PTX_DIRECTIVE_SECTION);
    
    assert(parse_directive("alias", &directive) == true);
    assert(directive == PTX_DIRECTIVE_ALIAS);
    
    assert(parse_directive("extern", &directive) == true);
    assert(directive == PTX_DIRECTIVE_EXTERN);
    
    assert(parse_directive("maxntid", &directive) == true);
    assert(directive == PTX_DIRECTIVE_MAXNTID);
    
    assert(parse_directive("align", &directive) == true);
    assert(directive == PTX_DIRECTIVE_ALIGN);
    
    assert(parse_directive("file", &directive) == true);
    assert(directive == PTX_DIRECTIVE_FILE);
    
    assert(parse_directive("minnctapersm", &directive) == true);
    assert(directive == PTX_DIRECTIVE_MINNCTAPERSM);
    
    assert(parse_directive("sreg", &directive) == true);
    assert(directive == PTX_DIRECTIVE_SREG);
    
    printf("All remaining directives parsing tests passed!\n");
}

void test_invalid_directives() {
    printf("Testing invalid directive parsing...\n");
    
    ptx_directive_t directive;
    
    // Test invalid directives
    assert(parse_directive("nonexistent", &directive) == false);
    assert(parse_directive("", &directive) == false);
    assert(parse_directive(NULL, &directive) == false);
    assert(parse_directive("version", NULL) == false);
    
    // Test with leading/trailing spaces (should not be valid in our parser)
    assert(parse_directive(" version", &directive) == false);
    assert(parse_directive("version ", &directive) == false);
    
    printf("Invalid directive parsing tests passed!\n");
}

int main() {
    printf("Running PTX directive parsing tests...\n");
    
    test_basic_directives();
    test_case_insensitivity();
    test_memory_directives();
    test_cluster_directives();
    test_all_directives();
    test_invalid_directives();
    
    printf("All directive parsing tests passed!\n");
    return 0;
} 