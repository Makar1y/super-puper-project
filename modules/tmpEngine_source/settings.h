#ifndef SETTINGS_H
#define SETTINGS_H

// Sizes
#define LINE_BUFFER_SIZE 256
#define MAX_REQUEST_URL_LENGTH 256

// Other
#define ERROR_PREFIX  "!!! ERROR: "

// Blocks settings
#define BLOCK_PREFIX "{%"
#define BLOCK_POSTFIX "%}"
#define END_NAME "end"
#define BLOCK_NAME_SCAN_FORMAT "{%% %[^%{} ] %%}"

// Tags settings
#define TAG_PREFIX "{{"
#define TAG_POSTFIX "}}"
#define TAG_NAME_SCAN_FORMAT "{{ %[^{} ] }}"

// If you change code and something is not working :)
#define DEBUG

#endif