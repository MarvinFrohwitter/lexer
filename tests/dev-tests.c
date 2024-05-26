#define GUTILS_IMPLEMENTATION
#include "gutils.h"

// #define INTERNAL 1
#ifdef INTERNAL
#define EXLEX_IMPLEMENTATION
#endif
#include "lexer.h"
#include <stdio.h>

typedef struct FileSring {
  char *elements;
  size_t count;
  size_t capacity;
} FileString;

int main(int argc, char **argv) {

  char *programm = shift_args(&argc, &argv);
  // fprintf(stderr, "INFO: The %s is executing.\n", programm);

#ifdef INTERNAL
#include <string.h>

  char *content_to_parse = "G "
                           "._HAUS,"
                           "uint32_t "
                           "int "
                           "while_t "
                           "int8_t "
                           "uint8_t "
                           "((102/10));"
                           // "'\0' "
                           // "'\\' "
                           // "'0' "

                           // "12.3e10F/123e10F"
                           // "7ULL "
                           // "7LL "
                           // "a*(b*c+2)*PI "
                           // "7L "
                           // "4567ULL, "
                           // "(836.12f*123.4L), "
                           // "836.12ULL, "
                           // "836.12L "
                           // "836.12f, "
                           // "456.7f "
                           // "863.4f, "
                           // "8634f, "
                           // "4567ULL "
                           // "47 "
                           // "437; "
                           // "long "
                           // "long "
                           // "4567. "
                           // "4567.2345 "

                           // "1234ULLH "
                           // "long "
                           // "= .,"
                           // "long int = 'a' "
                           // "void \n "

                           // "\" Das ist ein string\"hallo "

                           // "0xB "
                           // "0xB4 "
                           // "0XCCBBA "
                           // "0xCCBBAA88 "
                           // "0xB4812ABDBDF "
                           // // Debug fail tests:
                           // "1e+4"
                           // "3e-6 "
                           // "3e8 "
                           // "3e8L "
                           // "33.2e8F "
                           // "..2e8F "
                           // "1.3.2e8F "
                           // "1.e8F "
                           // "3e+662337 "
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
                           // // Debug wrong token tests:
                           // "/* BOB*/a"
                           // "/*/*\" */    int\n"

                           // "/*/*\"\"*/  "

                           // "hallo \n"
                           // "// \" */ \n   int"

                           // "// /* luchs*/ \n"
                           // "/* BOB \"was ist los\n\"*/\n"
                           // "/* Luchs   */"
                           // "\"Haus Hans\"   lasst\" IM string \"\n"
                           // "// \"Haus Hans\"   lasst\n"
                           // "// \"Haus Hans\"   Hase \n  \n"
                           // "// \"Haus Hans\"   lasst\n"
                           "size_t \n"
                           "234.;";

  size_t len = strlen(content_to_parse);
  Lexer *lexer = lexer_new("INTERNAL", content_to_parse, len, 0);

#else
  FileString fs = {0};
  if (argc < 1 || argc > 1) {
    fprintf(stderr, "ERROR: Usage %s <filepath>\n", programm);
    return 1;
  }
  char *filename = shift_args(&argc, &argv);
  // char *filename = "/home/marvin/Entwicklung/c/lexer/src/lexer.c";
  // FILE *fd = fopen(filename, "rb");
  // if (!fd) {
  //   perror("fopen");
  //   return 1;
  // }

  read_file(filename, (Content *)&fs);

  // int c;
  // int k = 0;
  // while ((c = fgetc(fd)) != EOF && k < 10) {
  //   k++;
  //   char character = c;
  //   dap(&fs, character);
  //   // fprintf(stderr, "THE CHARS : [%c]\n", character);
  // }

  // fclose(fd);
  Lexer *lexer = lexer_new(filename, fs.elements, fs.count, 0);
#endif

  FileString output = {0};

  Token t = {0};
  size_t counter = 0;
  int error_count = 0;
  while (t.kind != EOF_TOKEN) {

    t = lexer_next(lexer);
    counter++;

    dapc(&output, t.content, t.size);
    dap(&output, '\0');

    fprintf(stdout, "[%s] form type %u\n", output.elements, t.kind);
    if (t.kind == ERROR) {
      error_count += 1;
    }
    if (error_count > 0) {
      break;
    }

    output.count = 0;
  }
  fprintf(stderr, "There are %zu tokens\n", counter);

  if (error_count) {
    fprintf(stderr, "There are %d errors\n", error_count);

  } else {
    fprintf(stderr, "Success\n");
  }

  free(output.elements);
  lexer_del(lexer);
  return 0;
}
