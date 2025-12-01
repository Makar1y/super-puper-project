#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO Add documentation
#define LINE_BUFFER_SIZE 256 // Max buffer length (while scaning template file)
#define MAX_REQUEST_URL_LENGTH 256

#define ERROR_PREFIX  "!!! ERROR: "
#define BLOCK_PREFIX "{%"
#define BLOCK_POSTFIX "%}"
#define BLOCK_NAME_SCAN_FORMAT "{%% %[^%{}] %%}"
#define TAG_PREFIX "{{"
#define TAG_POSTFIX "}}"
#define TAG_NAME_SCAN_FORMAT "{{ %[^{}] }}"


int template_to_html(FILE *html_template, char *target_filename) {
    unsigned int line = 0;
    char *buffer = malloc(sizeof(char) * LINE_BUFFER_SIZE);
    char *request_url = calloc(MAX_REQUEST_URL_LENGTH, sizeof(char));
    *request_url = '.';

    while (fgets(buffer, LINE_BUFFER_SIZE, html_template)) {
        ++line;
        char *block_name_start = strstr(buffer, BLOCK_PREFIX); // If start of block in line
        char *tag_name_start = strstr(buffer, TAG_PREFIX); // if start of tag
        fseek(html_template, -20, SEEK_CUR);

        if (block_name_start) { // block serving
            char *block_name_end = strstr(buffer, BLOCK_POSTFIX);

            if (block_name_end) {
                char block_name[LINE_BUFFER_SIZE];
                if (sscanf(block_name_start, BLOCK_NAME_SCAN_FORMAT, block_name)) {
                    printf("%s%u, ", block_name, line);

                } else { fprintf(stderr, "%sNo block name provided after opened '%s' on line %u !\n", ERROR_PREFIX, BLOCK_PREFIX, line); } // Error
            } else { fprintf(stderr, "%sBlock not closed with '%s' !\n", ERROR_PREFIX, BLOCK_POSTFIX, line); } // Error
        }

        if (tag_name_start) { // tag serving
            char *tag_name_end = strstr(buffer, TAG_POSTFIX);

            if (tag_name_end) {
                char tag_name[LINE_BUFFER_SIZE];
                if (sscanf(tag_name_start, TAG_NAME_SCAN_FORMAT, tag_name)) {
                    printf("%s %u, ", tag_name, line);
                    // FIXME Add checks for array elements
                } else { fprintf(stderr, "%sNo tag name provided after opened '%s' on line %u!\n", ERROR_PREFIX, TAG_PREFIX, line); } // Error 
            } else { fprintf(stderr, "%sTag not closed with '%s' on line %u !\n", ERROR_PREFIX, TAG_POSTFIX, line); } // Error
        }
    }
}

int main() {
    FILE *fptr = fopen("../templates/template.html", "r+");
    
    if (fptr) {
        template_to_html(fptr, "");
    }
    return 0;
}