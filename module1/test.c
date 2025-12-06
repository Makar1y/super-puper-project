#include <stdio.h>
#include <stdlib.h>
#include "module1.h"

int main() {
  copyHTMLBlocks("../templates/template.html", "test.html", "../templates/data.json");
  copyHTMLThatAreArrays("test.html", "test.html", "../templates/data.json");
}

/* to compile: 
cd module1
gcc module1.c test.c -o test.exe
.\test.exe
*/