#include <stdlib.h>
#include <stdio.h>

#include "settings.h"
#include "block_manager.h"


int handle_block_open(Block_manager *block_manager, FILE *output_file, unsigned int line, char *buffer, \
                      char *block_name_start, char *block_name, char *block_name_end, char *request_url) {
    if (block_push(block_manager, block_name, line, 3)) { // TODO Replace repeats with function to get data from json
        strcat(request_url, "."); // [ ] Adapt for json functions
        strcat(request_url, block_name);
            
        if ( fwrite(buffer, sizeof(char), block_name_start - buffer, output_file) || !fputs(block_name_end  + strlen(BLOCK_POSTFIX), output_file) ) {
            return 1;
        }
        fprintf(stderr, "%s Writing to output file unsuccessful! (html line %u)\n", ERROR_PREFIX, line);
    }
    fprintf(stderr, "%s Block pushing unsuccessful! (html line %u)\n", ERROR_PREFIX, line);
    return 0;
}

int handle_block_repeat(FILE *output_file, unsigned int line, char *buffer, char *block_name_end) {
    if ( fputs(block_name_end  + strlen(BLOCK_POSTFIX), output_file) ) {
        return 1;
    }
    fprintf(stderr, "%s Repeating writing to output file unsuccessful! (html line %u)\n", ERROR_PREFIX, line);
    return 0;
}

int handle_block_end(Block_manager *block_manager, FILE *template_file, FILE *output_file, unsigned int line, \
                     char *block_name_end, char *request_url) {
    if (block_manager->size > 0) {
        if (block_manager->blocks) {
            Block *block = block_top(block_manager);
            if (block->left_repeats) {
                fseek(template_file, 0, SEEK_SET);
                char unused_buffer[LINE_BUFFER_SIZE];
                
                for (unsigned int i = 0; i < block->line - 1; ++i) {
                    fgets(unused_buffer, LINE_BUFFER_SIZE, template_file);
                }
                block->left_repeats--;
                return 1;
            } else {
                request_url[strlen(request_url) - strlen(block->name) - 1] = '\0' ;
                block_pop(block_manager);
                if (fputs(block_name_end + strlen(BLOCK_POSTFIX), output_file)) {
                    return 1;
                }
                fprintf(stderr, "%s Writing to output file unsuccessful! (html line %u)\n", ERROR_PREFIX, line);
            }
        } else {
            fprintf(stderr, "%s Blocks memory points to NULL! (html line %u)\n", ERROR_PREFIX, line);
        }
    } else {
        fprintf(stderr, "%s Ending block without opened one! (html line %u)\n", ERROR_PREFIX, line);
    }
    return 0;
}
