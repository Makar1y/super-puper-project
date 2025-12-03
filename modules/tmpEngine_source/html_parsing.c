#include <string.h>
#include <stdio.h>

#include "settings.h"
#include "html_parsing.h"


char *find_block_start(char *line) {
    return strstr(line, BLOCK_PREFIX);
}

char *find_block_end(char *start) {
    return strstr(start, BLOCK_POSTFIX);
}

int parse_block_name(const char *start, char *out) {
    return sscanf(start, BLOCK_NAME_SCAN_FORMAT, out);
}
