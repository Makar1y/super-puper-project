#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"
#include "block_manager.h"
#include "html_parsing.h"
#include "block_serving.h"


void serve_block(Block_manager *block_manager, FILE *html_template, FILE *output_file, unsigned int line, char *buffer, \
                    char *block_name_start, char* tag_start, char *request_url) {
    char block_name[LINE_BUFFER_SIZE];
    char *block_name_end = find_block_end(block_name_start);

    if ( !block_name_end ) {
        fprintf(stderr, "%s Block not closed with '%s'!\n (html line %u)\n", ERROR_PREFIX, BLOCK_POSTFIX, line);
        return 0;
    }

    if ( !parse_block_name(block_name_start, block_name) ) {
        fprintf(stderr, "%s Missing block name inside '%s %s' block! (html line %u)\n", ERROR_PREFIX, BLOCK_PREFIX, BLOCK_POSTFIX, line);
        return 0;
    }

    if (strcmp(block_name, END_NAME) == 0) {
        handle_block_end(block_manager, html_template, output_file, line, block_name_end, request_url);
    } else {
        Block *current_block = block_top(block_manager);
        if (current_block && strcmp(current_block->name, block_name) == 0) {
            handle_block_repeat(output_file, line, buffer, block_name_end);
        } else {
            if ( !handle_block_open(block_manager, output_file, line, buffer, block_name_start, block_name, block_name_end, request_url) ) {
                error_serving_block();
            }
        }
    }
}


void serve_tag(Block_manager *block_manager, FILE *html_template, FILE *output_file, unsigned int line, char *buffer, \
                    char *block_name_start, char* tag_start, char *request_url) {
    char block_name[LINE_BUFFER_SIZE];
    char *block_name_end = find_block_end(block_name_start);

    if ( !block_name_end ) {
        fprintf(stderr, "%s Block not closed with '%s'!\n (html line %u)\n", ERROR_PREFIX, BLOCK_POSTFIX, line);
        return 0;
    }

    if ( !parse_block_name(block_name_start, block_name) ) {
        fprintf(stderr, "%s Missing block name inside '%s %s' block! (html line %u)\n", ERROR_PREFIX, BLOCK_PREFIX, BLOCK_POSTFIX, line);
        return 0;
    }

    if (strcmp(block_name, END_NAME) == 0) {
        handle_block_end(block_manager, html_template, output_file, line, block_name_end, request_url);
    } else {
        Block *current_block = block_top(block_manager);
        if (current_block && strcmp(current_block->name, block_name) == 0) {
            handle_block_repeat(output_file, line, buffer, block_name_end);
        } else {
            if ( !handle_block_open(block_manager, output_file, line, buffer, block_name_start, block_name, block_name_end, request_url) ) {
                error_serving_block();
            }
        }
    }
}


void process_line(Block_manager *block_manager, FILE *html_template, FILE *output_file, unsigned int line, char *buffer, char *request_url) {
    char *block_start = find_block_start(buffer);
    char *tag_start   = find_tag_start(buffer);

    if (block_start)
        serve_block(block_manager, html_template, output_file, line, buffer, block_start, tag_start, request_url);
    else if (tag_start)
        serve_tag(buffer, tag_start);
    else
        fputs(buffer, output_file);
}



void template_to_html(FILE *html_template, char *target_filename) {
    FILE *output_file = fopen(target_filename, "w");
    if ( !output_file ) {
        perror(target_filename);
        return;
    }

    char buffer[LINE_BUFFER_SIZE];
    char request_url[MAX_REQUEST_URL_LENGTH] = {0};
    Block_manager block_manager = {0};

    unsigned line = 0;
    while (fgets(buffer, LINE_BUFFER_SIZE, html_template)) {
        ++line;
        process_line(&block_manager, html_template, output_file, line, buffer, request_url);
    }

    destroy_block_manager(&block_manager);
    fclose(output_file);
}

int main() {
    FILE *test_template = fopen("../templates/template.html", "r");
    
    if (test_template) {
        template_to_html(test_template, "index.html");
        fclose(test_template);
    }
    
    return 0;
}