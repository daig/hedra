#include <ptx_parse/ptx_attribute_parse.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Helper function to skip whitespace
 */
static const char* skip_whitespace(const char* str) {
    while (str && *str && isspace(*str)) {
        str++;
    }
    return str;
}

bool parse_attribute_tag(const char* str, ptx_attribute_tag_t* tag, const char** str_end) {
    if (!str || !tag || !str_end) {
        return false;
    }
    
    // Skip whitespace
    str = skip_whitespace(str);
    
    // Check for attribute tags
    if (strncmp(str, ".managed", 8) == 0) {
        *tag = PTX_ATTR_MANAGED;
        *str_end = str + 8;
        return true;
    } else if (strncmp(str, ".unified", 8) == 0) {
        *tag = PTX_ATTR_UNIFIED;
        *str_end = str + 8;
        return true;
    }
    
    return false;
}

bool parse_attribute(const char* str, ptx_attribute_t* attr, const char** str_end) {
    if (!str || !attr || !str_end) {
        return false;
    }
    
    // Initialize
    memset(attr, 0, sizeof(ptx_attribute_t));
    
    // Skip whitespace
    str = skip_whitespace(str);
    
    // Check for directive
    if (strncmp(str, ".attribute", 10) != 0) {
        return false;
    }
    str += 10;
    
    // Skip whitespace after directive
    str = skip_whitespace(str);
    
    // Parse attribute tag
    ptx_attribute_tag_t tag;
    const char* tag_end = NULL;
    if (!parse_attribute_tag(str, &tag, &tag_end)) {
        return false;
    }
    
    attr->tag = tag;
    str = tag_end;
    
    // Handle unified attribute with UUID parameters
    if (tag == PTX_ATTR_UNIFIED) {
        // Skip whitespace
        str = skip_whitespace(str);
        
        // Check for opening parenthesis
        if (*str != '(') {
            return false;
        }
        str++;
        
        // Skip whitespace after opening parenthesis
        str = skip_whitespace(str);
        
        // Parse first UUID (can be hex or decimal)
        char* end_ptr;
        if (strncmp(str, "0x", 2) == 0 || strncmp(str, "0X", 2) == 0) {
            attr->unified.uuid1 = strtoull(str, &end_ptr, 16);
        } else {
            attr->unified.uuid1 = strtoull(str, &end_ptr, 10);
        }
        
        if (end_ptr == str) {
            return false; // Failed to parse number
        }
        
        str = end_ptr;
        
        // Skip whitespace after first UUID
        str = skip_whitespace(str);
        
        // Check for comma
        if (*str != ',') {
            return false;
        }
        str++;
        
        // Skip whitespace after comma
        str = skip_whitespace(str);
        
        // Parse second UUID (can be hex or decimal)
        if (strncmp(str, "0x", 2) == 0 || strncmp(str, "0X", 2) == 0) {
            attr->unified.uuid2 = strtoull(str, &end_ptr, 16);
        } else {
            attr->unified.uuid2 = strtoull(str, &end_ptr, 10);
        }
        
        if (end_ptr == str) {
            return false; // Failed to parse number
        }
        
        str = end_ptr;
        
        // Skip whitespace after second UUID
        str = skip_whitespace(str);
        
        // Check for closing parenthesis
        if (*str != ')') {
            return false;
        }
        str++;
    }
    
    *str_end = str;
    return true;
} 