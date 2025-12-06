#ifndef MODULE_H
#define MODULE_H

int countBlocks(char* filename_json);
void copyHTMLBlocks(char* template_html, char* output_html, char* filename_json);
void copyHTMLThatAreArrays(char* template_html, char* output_html, char* filename_json);

#endif