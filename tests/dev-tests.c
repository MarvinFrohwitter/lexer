#define GUTILS_IMPLEMENTATION
#include "../../gutils/gutils.h"
#include "../src/lexer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FileSring {
  char *items;
  size_t count;
  size_t capacity;
} FileString;

int main(void) {

  char *content_to_parse = "int "
                           // "4567L "
                           // "4567ULL "
                           // "47 "
                           // "437; "
                           // "long "
                           // "long "
                           // "4567. "
                           // "4567.2345 "

                           // "1234ULLH "
                           // "long "
                           "long 'a'"
                           // "void \n "

                           // "\" Das ist ein string\"hallo "

                           // "0xB "
                           // "0xB4 "
                           "0XCCBBA "
                           // "0xCCBBAA88 "
                           // "0xB4812ABDBDF "
                           // // Debug fail tests:
                           "1e+4"
                           // "3e-6 "
                           // "3e+662337 "
                           // "3e-637 "
                           // "33e+z "
                           // "4567A "

                           // "3e-6623e7 "
                           // "0xM "
                           // "0xAAZYXW "

                           // "# \n"
                           // "## \n"
                           // "### vaois das is\n"

                           // "hallo \n"
                           // "/**/    int\n"
                           // // // Debug wrong token tests:
                           // "/* BOB*/a"
                           // "/*/*\" */    int\n"

                           // "/*/*\"\"*/  "

                           // "hallo \n"
                           // "// \" */ \n   int"

                           // "// /* luchs*/ \n"
                           // "/* BOB \"was ist los\n\"*/\n"
                           // "/* Luchs   */"
                           // "// \"Haus Hans\"   lasst\" IM string \"\n"
                           // "// \"Haus Hans\"   lasst\n"
                           // "// \"Haus Hans\"   Hase \n  \n"
                           "// \"Haus Hans\"   lasst\n"
                           "size_t \n"
                           "0xA\n";

  size_t len = strlen(content_to_parse);

  FileString fs = {0};
  char *filename = "/home/marvin/Entwicklung/c/lexer/src/lexer.c";

  FILE *fd = fopen(filename, "rb");
  if (!fd) {
    perror("fopen");
    return 1;
  }

  int c;
  while ((c = fgetc(fd)) != EOF) {
    char character = c;
    da_append(&fs, character);
    // fprintf(stderr, "THE CHARS : [%c]\n", character);
  }
  fclose(fd);

  // Lexer lexer = lexer_new(fs.items, fs.count, 0);
  Lexer lexer = lexer_new(content_to_parse, len, 0);

  FileString output = {0};

  while (lexer.content_lenght - 1 >= lexer.position) {
   Token t = lexer_next(&lexer);

    da_append_many(&output, t.content, t.size);
    da_append(&output, '\0');

    fprintf(stdout, "[%s] form type %u\n", output.items, t.kind);

    output.count = 0;
  }
  printf("Succses\n");

  return 0;
}
