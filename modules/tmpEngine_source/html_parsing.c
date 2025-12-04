#include <string.h>
#include <stdio.h>

#include "settings.h"
#include "html_parsing.h"


char *find_block_start(char *line) {
    return strstr(line, BLOCK_PREFIX);
}

char *find_block_end(char *block_start) {
    return strstr(block_start, BLOCK_POSTFIX);
}

int parse_block_name(const char *block_start, char *output_buffer) {
    return sscanf(block_start, BLOCK_NAME_SCAN_FORMAT, output_buffer);
}

char *find_tag_start(char *line) {
    return strstr(line, TAG_PREFIX);
}

char *find_tag_end(char *tag_start) {
    return strstr(tag_start, TAG_POSTFIX);
}

int parse_tag_name(const char *tag_start, char *output_buffer) {
    return sscanf(tag_start, TAG_NAME_SCAN_FORMAT, output_buffer);
}
