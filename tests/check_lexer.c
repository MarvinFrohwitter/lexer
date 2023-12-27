#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "../src/lexer.h"
#include <check.h>

START_TEST(test_lexer_new) {

  char *content_to_parse = "int j = 0;";
  size_t len = strlen(content_to_parse);
  size_t pos = 0;

  Lexer base_lexer;
  base_lexer.content = content_to_parse;
  base_lexer.content_lenght = len;
  base_lexer.position = pos;

  Lexer new_lexer = lexer_new(content_to_parse, len, pos);

  ck_assert(base_lexer.content == new_lexer.content);
  ck_assert(base_lexer.content_lenght == new_lexer.content_lenght);
  ck_assert(base_lexer.position != new_lexer.position);
  ck_assert(1 != 2);

assert(lexer_check_boundery(&new_lexer) != 1);

}
END_TEST

int main(void) { return 0; }
