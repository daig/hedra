#include <ptx_print/ptx_comment_print.h>
#include <string.h>

/**
 * Print a comment AST node to a file.
 */
int print_comment(const comment_t* comment, FILE* out) {
    if (!comment || !out) {
        return -1;
    }
    
    // When parsing, the text inside the comment is stored without the comment markers.
    // We need to check the format of the source comment to determine which style to use.
    // If there's a newline in the comment content, use C-style to preserve multiline format.
    if (strchr(comment->text, '\n') != NULL) {
        // C-style comment (/* ... */) for multiline
        return fprintf(out, "/*%s*/", comment->text);
    } else {
        // C++-style comment (// ...)
        return fprintf(out, "//%s", comment->text);
    }
}

/**
 * Print a comment AST node to a string.
 */
int sprint_comment(const comment_t* comment, char* buffer, size_t size) {
    if (!comment || !buffer || size == 0) {
        return -1;
    }
    
    // Use a temporary file stream to write to the buffer
    FILE* stream = fmemopen(buffer, size, "w");
    if (!stream) {
        return -1;
    }
    
    int result = print_comment(comment, stream);
    fclose(stream);
    
    return result;
} 