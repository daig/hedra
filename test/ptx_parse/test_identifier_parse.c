#include <ptx_parse/ptx_identifier_parse.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void test_valid_user_defined_identifiers() {
    printf("Testing valid user-defined identifier parsing...\n");
    
    ptx_identifier_t* identifier = NULL;
    
    // Test basic identifier
    printf("Parsing 'myvar'\n");
    assert(parse_identifier("myvar", &identifier) == true);
    assert(identifier != NULL);
    assert(identifier->tag == PTX_IDENTIFIER_USER_DEFINED);
    printf("Parsed identifier: '%s'\n", identifier->user_defined);
    assert(strcmp(identifier->user_defined, "myvar") == 0);
    free(identifier->user_defined);
    free(identifier);
    
    // Test identifier with underscore
    printf("Parsing 'my_var'\n");
    assert(parse_identifier("my_var", &identifier) == true);
    assert(identifier != NULL);
    assert(identifier->tag == PTX_IDENTIFIER_USER_DEFINED);
    printf("Parsed identifier: '%s'\n", identifier->user_defined);
    assert(strcmp(identifier->user_defined, "my_var") == 0);
    free(identifier->user_defined);
    free(identifier);
    
    // Test identifier with numbers
    printf("Parsing 'var123'\n");
    assert(parse_identifier("var123", &identifier) == true);
    assert(identifier != NULL);
    assert(identifier->tag == PTX_IDENTIFIER_USER_DEFINED);
    printf("Parsed identifier: '%s'\n", identifier->user_defined);
    assert(strcmp(identifier->user_defined, "var123") == 0);
    free(identifier->user_defined);
    free(identifier);
    
    // Test identifier with leading dollar sign
    printf("Parsing '$var'\n");
    assert(parse_identifier("$var", &identifier) == true);
    assert(identifier != NULL);
    assert(identifier->tag == PTX_IDENTIFIER_USER_DEFINED);
    printf("Parsed identifier: '%s'\n", identifier->user_defined);
    assert(strcmp(identifier->user_defined, "$var") == 0);
    free(identifier->user_defined);
    free(identifier);
    
    // Test identifier with leading underscore
    printf("Parsing '_var'\n");
    assert(parse_identifier("_var", &identifier) == true);
    assert(identifier != NULL);
    assert(identifier->tag == PTX_IDENTIFIER_USER_DEFINED);
    printf("Parsed identifier: '%s'\n", identifier->user_defined);
    assert(strcmp(identifier->user_defined, "_var") == 0);
    free(identifier->user_defined);
    free(identifier);
    
    // Test identifier with leading whitespace
    printf("Parsing '  var'\n");
    assert(parse_identifier("  var", &identifier) == true);
    assert(identifier != NULL);
    assert(identifier->tag == PTX_IDENTIFIER_USER_DEFINED);
    printf("Parsed identifier: '%s'\n", identifier->user_defined);
    assert(strcmp(identifier->user_defined, "var") == 0);
    free(identifier->user_defined);
    free(identifier);
    
    printf("Valid user-defined identifier parsing tests passed!\n");
}

void test_valid_predefined_identifiers() {
    printf("Testing valid predefined identifier parsing...\n");
    
    ptx_identifier_t* identifier = NULL;
    
    // Test predefined identifier %clock
    printf("Parsing '%%clock'\n");
    assert(parse_identifier("%clock", &identifier) == true);
    assert(identifier != NULL);
    assert(identifier->tag == PTX_IDENTIFIER_PREDEFINED);
    assert(identifier->predefined == CLOCK);
    free(identifier);
    
    // Test predefined identifier %tid
    printf("Parsing '%%tid'\n");
    assert(parse_identifier("%tid", &identifier) == true);
    assert(identifier != NULL);
    assert(identifier->tag == PTX_IDENTIFIER_PREDEFINED);
    assert(identifier->predefined == TID);
    free(identifier);
    
    printf("Valid predefined identifier parsing tests passed!\n");
}

void test_invalid_identifiers() {
    printf("Testing invalid identifier parsing...\n");
    
    ptx_identifier_t* identifier = NULL;
    
    // Test empty string
    assert(parse_identifier("", &identifier) == false);
    
    // Test just whitespace
    assert(parse_identifier("   ", &identifier) == false);
    
    // Test invalid start character (digit)
    assert(parse_identifier("123var", &identifier) == false);
    
    // Test invalid predefined identifier
    assert(parse_identifier("%notapredefined", &identifier) == false);
    
    // Test NULL input
    assert(parse_identifier(NULL, &identifier) == false);
    
    // Test NULL output pointer
    assert(parse_identifier("var", NULL) == false);
    
    printf("Invalid identifier parsing tests passed!\n");
}

int main() {
    printf("Running identifier parser tests...\n");
    
    test_valid_user_defined_identifiers();
    test_valid_predefined_identifiers();
    test_invalid_identifiers();
    
    printf("All identifier parser tests passed!\n");
    return 0;
} 