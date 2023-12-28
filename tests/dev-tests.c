#include "../src/lexer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {

  char *content_to_parse = "int "
                           // "4567L "
                           "4567ULL "
                           "47 "
                           // "long "
                           "long "
                           "4567. "
                           "4567.2345 "

                           // "1234ULLH "
                           // "long "
                           // "void \n "

                           "\" Das ist ein string\"hallo "

                           // "0xB "
                           // "0xB4 "
                           // "0XCCBBA "
                           // "0xCCBBAA88 "
                           // "0xB4812ABDBDF "
                           // // Debug fail tests:
                           // "1e+4 "
                           // "3e-6 "
                           // "3e+662337 "
                           // "3e-637 "
                           // "33e+z "
                           // "4567A "

                           // "3e-6623e7 "
                           // "0xM "
                           // "0xAAZYXW "

                           "# \n"
                           "## \n"
                           "### vaois das is\n"
                           "hallo \n"
                           // Debug wrong token tests:
                           // "/* BOB*/"
                           "// \"Haus Hans\"   lasst\n"
                           "hallo \n"
                           "420";

  size_t len = strlen(content_to_parse);
  Lexer lexer = lexer_new(content_to_parse, len, 0);

  while (lexer.content_lenght - 1 >= lexer.position) {
    Token t = lexer_next(&lexer);
    char *temp;
    char *tofree_temp = temp = malloc(t.size * sizeof(char));
    assert(temp != NULL && "Buy more RAM !!");
    strncpy(temp, t.content, t.size);
    temp[t.size] = 0;
    printf("[%s] form type %u\n", temp, t.kind);
    free(tofree_temp);
  }
  char *h = "Succses";
  printf("%s\n", h);

  return 0;
}

