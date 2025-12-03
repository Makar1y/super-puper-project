#ifndef PARSING_H
#define PARSING_H

char *find_block_start(char *line);
char *find_block_end(char *start);
int parse_block_name(const char *start, char *out);

#endif