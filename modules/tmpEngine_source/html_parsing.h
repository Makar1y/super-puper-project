#ifndef PARSING_H
#define PARSING_H

char *find_block_start(char *line);
char *find_block_end(char *block_start);
int parse_block_name(const char *block_start, char *output_buffer);
char *find_tag_start(char *line);
char *find_tag_end(char *tag_start);
int parse_tag_name(const char *tag_start, char *output_buffer);

#endif