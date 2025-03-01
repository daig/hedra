#include <ptx_parse/ptx_label_parse.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void test_valid_labels() {
    printf("Testing valid label parsing...\n");
    
    ptx_label_t* label = NULL;
    
    // Test basic label
    printf("Parsing 'label_1:'\n");
    assert(parse_label("label_1:", &label) == true);
    assert(label != NULL);
    printf("Parsed label: '%s'\n", label->name);
    assert(strcmp(label->name, "label_1") == 0);
    free(label->name);
    free(label);
    
    // Test label with whitespace before colon
    printf("Parsing 'label_2 :'\n");
    assert(parse_label("label_2 :", &label) == true);
    assert(label != NULL);
    printf("Parsed label: '%s'\n", label->name);
    assert(strcmp(label->name, "label_2") == 0);
    free(label->name);
    free(label);
    
    // Test label with leading whitespace
    printf("Parsing '  label_3:'\n");
    assert(parse_label("  label_3:", &label) == true);
    assert(label != NULL);
    printf("Parsed label: '%s'\n", label->name);
    assert(strcmp(label->name, "label_3") == 0);
    free(label->name);
    free(label);
    
    // Test label with underscore start
    printf("Parsing '_label:'\n");
    assert(parse_label("_label:", &label) == true);
    assert(label != NULL);
    printf("Parsed label: '%s'\n", label->name);
    assert(strcmp(label->name, "_label") == 0);
    free(label->name);
    free(label);
    
    // Test label with dollar sign start
    printf("Parsing '$label:'\n");
    assert(parse_label("$label:", &label) == true);
    assert(label != NULL);
    printf("Parsed label: '%s'\n", label->name);
    assert(strcmp(label->name, "$label") == 0);
    free(label->name);
    free(label);
    
    // Test label with percent sign start
    printf("Parsing '%%label:'\n");
    assert(parse_label("%label:", &label) == true);
    assert(label != NULL);
    printf("Parsed label: '%s'\n", label->name);
    assert(strcmp(label->name, "%label") == 0);
    free(label->name);
    free(label);
    
    printf("Valid label parsing tests passed!\n");
}

void test_invalid_labels() {
    printf("Testing invalid label parsing...\n");
    
    ptx_label_t* label = NULL;
    
    // Test string without colon
    assert(parse_label("not_a_label", &label) == false);
    
    // Test empty string
    assert(parse_label("", &label) == false);
    
    // Test just whitespace
    assert(parse_label("   ", &label) == false);
    
    // Test just a colon
    assert(parse_label(":", &label) == false);
    
    // Test invalid start character
    assert(parse_label("123label:", &label) == false);
    
    // Test NULL input
    assert(parse_label(NULL, &label) == false);
    
    // Test NULL output pointer
    assert(parse_label("label:", NULL) == false);
    
    printf("Invalid label parsing tests passed!\n");
}

int main() {
    printf("Running label parser tests...\n");
    
    test_valid_labels();
    test_invalid_labels();
    
    printf("All label parser tests passed!\n");
    return 0;
} 