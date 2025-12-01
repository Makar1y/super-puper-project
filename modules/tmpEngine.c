#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO Add documentation
#define LINE_BUFFER_SIZE 256 // Max buffer length (while scaning template file)
#define MAX_REQUEST_URL_LENGTH 256

#define END_NAME "end"
#define ERROR_PREFIX  "!!! ERROR: "
#define BLOCK_PREFIX "{%"
#define BLOCK_POSTFIX "%}"
#define BLOCK_NAME_SCAN_FORMAT "{%% %[^%{} ] %%}"
#define TAG_PREFIX "{{"
#define TAG_POSTFIX "}}"
#define TAG_NAME_SCAN_FORMAT "{{ %[^{} ] }}"

typedef struct {
    unsigned int line;
    char *start;
    unsigned int left_repeats;
} Block;

typedef struct {
    Block *blocks;
    unsigned int size;
} Block_manager;

Block_manager block_manager;
char request_url[MAX_REQUEST_URL_LENGTH];



void serve_block(char *block_name_start, unsigned int line) {
    char *block_name_end = strstr(block_name_start, BLOCK_POSTFIX);

    if (block_name_end) {
        char block_name[LINE_BUFFER_SIZE];

        if (sscanf(block_name_start, BLOCK_NAME_SCAN_FORMAT, block_name)) {
            if ( strcmp(block_name, END_NAME) ) {
                block_manager.size++;
                block_manager.blocks = realloc(block_manager.blocks, block_manager.size * sizeof(Block));
                if (block_manager.blocks) {
                    block_manager.blocks[block_manager.size - 1].line = line;
                    block_manager.blocks[block_manager.size - 1].start = block_name_end;
                    block_manager.blocks[block_manager.size - 1].left_repeats = 3; // FIXME Implement counts from json parser
                    if (request_url[0] != '\0') {
                        if ( !strcat(request_url, ".") ) ; // TODO Error
                    }
                    if ( !strcat(request_url, block_name) ) {
                        ; // TODO Error 
                    }
                } else { // TODO Error
                    ;
                }
            } else {
                // TODO End of block
                if (block_manager.blocks[block_manager.size - 1].left_repeats) {
                    ; // TODO Write repeat func
                } else {
                    block_manager.size--;
                }
            }
        } else { // Error
            fprintf(stderr, "%sNo block name provided after opened '%s' on line %u !\n", ERROR_PREFIX, BLOCK_PREFIX, line);
        }
    } else { // Error
        fprintf(stderr, "%sBlock not closed with '%s' !\n", ERROR_PREFIX, BLOCK_POSTFIX, line);
    }
}


int template_to_html(FILE *html_template, char *target_filename) {
    char *buffer = malloc(sizeof(char) * LINE_BUFFER_SIZE);
    unsigned int line = 0;

    while (fgets(buffer, LINE_BUFFER_SIZE, html_template)) {
        ++line;
        char *block_name_start = strstr(buffer, BLOCK_PREFIX); // If start of block in line
        char *tag_name_start = strstr(buffer, TAG_PREFIX); // if start of tag

        if (*request_url)
        printf("%u, %s \n", line, request_url);

        if (block_name_start) {
            serve_block(block_name_start, line);
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

    free(buffer);
    free(block_manager.blocks);
    block_manager.blocks = NULL;
    block_manager.size = 0;
}

int main() {
    FILE *test_template = fopen("../templates/template.html", "r+");
    
    if (test_template) {
        template_to_html(test_template, "index.html");
    }
    fclose(test_template);
    return 0;
}