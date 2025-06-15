#define CASSERT_IMPLEMENTATION
#define PRINT_OPERATION_AND_DESCRIPTION
#include "cassert.h"

#define LEXER_IMPLEMENTATION
#define EXLEX_IMPLEMENTATION
#define LEXLOOKAHEAD
#define LEX_NUMBER_SIGN
#include "../../lexer.h"
#include "../../src/lexer_helper_utils.c"

void add_description(Test *test, Lexer *lexer, Token *t) {
  cassert_set_last_cassert_description(
      test, strdup(tmp_sprintf("token content: `%s`",
                               lexer_token_to_cstr(lexer, t))));
}

Test cassert_lexer_new() {
  Test test = cassert_init_test("lexer_new");

  char *content = "0";
  size_t len = strlen(content);
  size_t pos = 0;

  Lexer base_lexer;
  base_lexer.content = content;
  base_lexer.content_length = len;
  base_lexer.position = pos;

  Lexer *lexer = lexer_new(__FILE__, content, len, pos);

  cassert_ptr_eq(base_lexer.content, lexer->content);
  cassert_size_t_eq(base_lexer.content_length, lexer->content_length);
  cassert_size_t_eq(base_lexer.position, lexer->position);

  cassert_bool_neq((bool)lexer_check_boundary(lexer), 0);
  cassert_bool_eq((bool)lexer_check_boundary(lexer), 1);

  return test;
}

Test lexer_line_and_column() {
  Test test = cassert_init_test("lexer_line_and_column");

  char *content = "0 1\n2 3";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);
  while (lexer_next(lexer).kind != EOF_TOKEN) {
  }

  cassert_int_eq(lexer->line_count, 2);
  cassert_int_eq(lexer->column_count, 3);
  cassert_ptr_eq(lexer->line_start, &content[4]);

  return test;
}

Test lexer_line_and_column_empty_content() {
  Test test = cassert_init_test("lexer_line_and_column");

  char *content = "";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);
  while (lexer_next(lexer).kind != EOF_TOKEN) {
  }

  cassert_int_eq(lexer->line_count, 0);
  cassert_int_eq(lexer->column_count, 0);
  cassert_ptr_eq(lexer->line_start, content);

  return test;
}

Test lexer_line_and_column_NULL_content() {
  Test test = cassert_init_test("lexer_line_and_column_NULL_content");

  Lexer *lexer = lexer_new(__FILE__, NULL, -1, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  cassert_int_eq(lexer->line_count, 0);
  cassert_int_eq(lexer->column_count, 0);
  cassert_ptr_eq(lexer->line_start, NULL);

  return test;
}

Test lexer_basic_number() {
  Test test = cassert_init_test("lexer_basic_number");

  char *content = "1";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  cassert_set_last_cassert_description(
      &test, tmp_sprintf("kind = %s", lexer_kind_to_str(NUMBER)));
  cassert_string_eq(lexer_token_to_cstr(lexer, &t), content);

  return test;
}

Test lexer_numbers() {
  Test test = cassert_init_test("lexer_numbers");

  char *content = "0 1 2 3 4 5 6 7 8 9";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  while (t.kind != EOF_TOKEN) {
    cassert_int_eq(t.kind, NUMBER);
    cassert_set_last_cassert_description(
        &test, strdup(tmp_sprintf("token content: `%s`",
                                  lexer_token_to_cstr(lexer, &t))));
    t = lexer_next(lexer);
  }

  return test;
}

Test lexer_negative_numbers() {
  Test test = cassert_init_test("lexer_negative_numbers");

  char *content = "-0 -1 -2 -3 -4 -5 -6 -7 -8 -9";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  while (t.kind != EOF_TOKEN) {
    cassert_int_eq(t.kind, NUMBER);
    cassert_set_last_cassert_description(
        &test, strdup(tmp_sprintf("token content: `%s`",
                                  lexer_token_to_cstr(lexer, &t))));
    t = lexer_next(lexer);
  }

  return test;
}

Test lexer_positive_numbers() {
  Test test = cassert_init_test("lexer_positive_numbers");

  char *content = "+0 +1 +2 +3 +4 +5 +6 +7 +8 +9";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  while (t.kind != EOF_TOKEN) {
    cassert_int_eq(t.kind, NUMBER);
    cassert_set_last_cassert_description(
        &test, strdup(tmp_sprintf("token content: `%s`",
                                  lexer_token_to_cstr(lexer, &t))));
    t = lexer_next(lexer);
  }

  return test;
}

Test lexer_negative_floating_point_numbers() {
  Test test = cassert_init_test("lexer_negative_floating_point_numbers");

  char *content = "-0.0 -1.1 -2.2 -3.3 -4.4 -5.5 -6.6 -7.7 -8.8 -9.9";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  while (t.kind != EOF_TOKEN) {
    cassert_int_eq(t.kind, NUMBER);
    cassert_set_last_cassert_description(
        &test, strdup(tmp_sprintf("token content: `%s`",
                                  lexer_token_to_cstr(lexer, &t))));
    t = lexer_next(lexer);
  }

  return test;
}

Test lexer_positivie_floating_point_numbers() {
  Test test = cassert_init_test("lexer_positivie_floating_point_numbers");

  char *content = "+0.0 +1.1 +2.2 +3.3 +4.4 +5.5 +6.6 +7.7 +8.8 +9.9";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  while (t.kind != EOF_TOKEN) {
    cassert_int_eq(t.kind, NUMBER);
    cassert_set_last_cassert_description(
        &test, strdup(tmp_sprintf("token content: `%s`",
                                  lexer_token_to_cstr(lexer, &t))));
    t = lexer_next(lexer);
  }

  return test;
}

Test lexer_suffix_floating_point_numbers() {
  Test test = cassert_init_test("lexer_suffix_floating_point_numbers");

  char *content = "0.0f 1.1F 2.2l 3.3L .4 -.5 +.6 -.7f +.8L";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  while (t.kind != EOF_TOKEN) {
    cassert_int_eq(t.kind, NUMBER);
    cassert_set_last_cassert_description(
        &test, strdup(tmp_sprintf("token content: `%s`",
                                  lexer_token_to_cstr(lexer, &t))));
    t = lexer_next(lexer);
  }

  return test;
}

Test lexer_wrong_suffix_floating_point_numbers() {
  Test test = cassert_init_test("lexer_wrong_suffix_floating_point_numbers");

  char *content = "0.0lf 1.1LF 2.2fl 3.3FL 4.4ll 5.5LL 6.6ff 7.7FF";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  while (t.kind != EOF_TOKEN) {
    cassert_int_eq(t.kind, NUMBER);
    add_description(&test, lexer, &t);

    t = lexer_next(lexer);
    cassert_int_eq(t.kind, IDENTIFIER);
    add_description(&test, lexer, &t);

    t = lexer_next(lexer);
  }

  return test;
}

Test lexer_hex_floating_point_number_error() {
  Test test = cassert_init_test("lexer_hex_floating_point_number_error");

  char *content = "0xf.p1L";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, ERROR);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_hex_floating_point_numbers() {
  Test test = cassert_init_test("lexer_hex_floating_point_numbers");

  char *content = "0x0f21L,"
                  "0x0f.2p1L,"
                  "0x1p1F,"
                  "0x0f.2p1F,"
                  "0x1p1F,"
                  "0x0f.2p001L,"
                  "0x0f.2p1L,";

  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  while (t.kind != EOF_TOKEN) {
    cassert_int_eq(t.kind, NUMBER);
    add_description(&test, lexer, &t);

    t = lexer_next(lexer);
    cassert_int_eq(t.kind, PUNCT_COMMA);
    add_description(&test, lexer, &t);

    t = lexer_next(lexer);
  }

  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_hex_floating_point_numbers_arithmetic() {
  Test test = cassert_init_test("lexer_hex_floating_point_numbers_arithmetic");

  char *content = "0x0p-1-2";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_hex_floating_point_numbers_no_suffix() {
  Test test = cassert_init_test("lexer_hex_floating_point_numbers_no_suffix");

  char *content = "0x.fp1,";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_COMMA);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_hex_floating_point_numbers_no_suffix_eof() {
  Test test =
      cassert_init_test("lexer_hex_floating_point_numbers_no_suffix_eof");

  char *content = "0x.fp1";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_hex_floating_point_number_addition_eof() {
  Test test = cassert_init_test("lexer_hex_floating_point_number_addition_eof");

  char *content = "0x0F1+m";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_ADD);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}
void lexer_all_numbers(Tests *tests) {
  cassert_dap(tests, lexer_basic_number());
  cassert_dap(tests, lexer_numbers());
  cassert_dap(tests, lexer_negative_numbers());
  cassert_dap(tests, lexer_positive_numbers());

  cassert_dap(tests, lexer_negative_floating_point_numbers());
  cassert_dap(tests, lexer_positivie_floating_point_numbers());

  cassert_dap(tests, lexer_suffix_floating_point_numbers());
  cassert_dap(tests, lexer_wrong_suffix_floating_point_numbers());

  cassert_dap(tests, lexer_hex_floating_point_number_error());
  cassert_dap(tests, lexer_hex_floating_point_numbers());

  cassert_dap(tests, lexer_hex_floating_point_numbers_arithmetic());
  cassert_dap(tests, lexer_hex_floating_point_numbers_no_suffix());
  cassert_dap(tests, lexer_hex_floating_point_numbers_no_suffix_eof());

  cassert_dap(tests, lexer_hex_floating_point_number_addition_eof());
}

void lexer_basic(Tests *tests) {
  cassert_dap(tests, cassert_lexer_new());
  cassert_dap(tests, lexer_line_and_column());
  cassert_dap(tests, lexer_line_and_column_empty_content());
  cassert_dap(tests, lexer_line_and_column_NULL_content());
  lexer_all_numbers(tests);
}

Test lexer_string_and_number() {
  Test test = cassert_init_test("lexer_string_and_number");

  char *content = "\"Hello\"0.0f"
                  "\"World\"123456789"
                  "\"foo bar baz\"123456789";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  add_description(&test, lexer, &t);

  return test;
}

void lexer_combination(Tests *tests) {
  cassert_dap(tests, lexer_string_and_number());
}

Test lexer_mixed() {
  Test test = cassert_init_test("lexer_mixed");

  char *content = "._HAUS,"
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
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_POINT);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_COMMA);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, KEYWORD_INT);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_LPAREN);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_LPAREN);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_DIV);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_RPAREN);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_RPAREN);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SEMICOLON);
  add_description(&test, lexer, &t);

  // ----------------------
  // t = lexer_next(lexer);
  // cassert_int_eq(t.kind, PUNCT_SINGLEQUOTE);
  // add_description(&test, lexer, &t);

  // t = lexer_next(lexer);
  // cassert_int_eq(t.kind, EOF_TOKEN);
  // add_description(&test, lexer, &t);

  // t = lexer_next(lexer);
  // cassert_int_eq(t.kind, PUNCT_SINGLEQUOTE);
  // add_description(&test, lexer, &t);

  // TODO: Enable and implement the cases in between.

  // ----------------------

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, KEYWORD_SIZE_T);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SEMICOLON);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_null_terminator() {
  Test test = cassert_init_test("lexer_null_terminator");

  char *content = "\"Hello\" '\0' 1";
  size_t len = 13;
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SINGLEQUOTE);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, NULL_TERMINATOR);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SINGLEQUOTE);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, NUMBER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_eof_null_terminator() {
  Test test = cassert_init_test("lexer_eof_null_terminator");

  char *content = "\"Hello\" \0";
  size_t len = 9;
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, NULL_TERMINATOR);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_string_semicolon() {
  Test test = cassert_init_test("lexer_string_semicolon");
  char *content = "const char *port = \"8080\";";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, KEYWORD_CONST);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, KEYWORD_CHAR);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_MUL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_ASSIGN);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SEMICOLON);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_char_keyword() {
  Test test = cassert_init_test("lexer_char_keyword");

  char *content = "char";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, KEYWORD_CHAR);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_string_literal() {
  Test test = cassert_init_test("lexer_string_literal");

  char *content = "\"HELLO\"";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_escape_double_quotes_in_string() {
  Test test = cassert_init_test("lexer_escape_quotes_in_string");

  char *content = "\" \\\"HELLO\\\" \"";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_escape_single_quotes_in_string() {
  Test test = cassert_init_test("lexer_escape_single_quotes_in_string");

  char *content = "\"\\'HELLO\\'\"";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_single_quotes_in_string() {
  Test test = cassert_init_test("lexer_single_quotes_in_string");

  char *content = "\"'HELLO'\"";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_unclosed_string() {
  Test test = cassert_init_test("lexer_unclosed_string");
  char *content = "\"HELLO World";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, ERROR);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_unclosed_string_at_eof() {
  Test test = cassert_init_test("lexer_unclosed_string_at_eof");

  char *content = "\"";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, ERROR);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_char_array_def() {
  Test test = cassert_init_test("lexer_char_array_def");

  char *content = "char *program_name = tkbc_shift_args(&argc, &argv);";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, KEYWORD_CHAR);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_MUL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_ASSIGN);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_LPAREN);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_BAND);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_COMMA);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_BAND);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, IDENTIFIER);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_RPAREN);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SEMICOLON);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_escaped_double_quote_in_single_quote() {
  Test test = cassert_init_test("lexer_escaped_double_quote_in_single_quote");

  char *content = "'\\\"' \"8080\"";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SINGLEQUOTE);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_BACKSLASH);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_DOUBLEQUOTE);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SINGLEQUOTE);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_double_quote_in_single_quote() {
  Test test = cassert_init_test("lexer_double_quote_in_single_quote");

  char *content = "'\"' \"8080\"";
  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SINGLEQUOTE);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_DOUBLEQUOTE);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SINGLEQUOTE);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_string_literal_after_char_literal_as_backslash() {
  Test test =
      cassert_init_test("lexer_string_literal_after_char_literal_as_backslash");

  char *content = "'\\\\'; \"0\";";

  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SINGLEQUOTE);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_BACKSLASH);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_BACKSLASH);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SINGLEQUOTE);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SEMICOLON);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SEMICOLON);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_string_literal_after_string_with_backslashes() {
  Test test =
      cassert_init_test("lexer_string_literal_after_string_with_backslashes");

  char *content = "\"\\\\\"; \"0\";";

  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SEMICOLON);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, PUNCT_SEMICOLON);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

Test lexer_empty_string_literal() {
  Test test = cassert_init_test("lexer_empty_string_literal");

  char *content = "\"\"";

  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token t = lexer_next(lexer);
  cassert_int_eq(t.kind, STRINGLITERAL);
  add_description(&test, lexer, &t);

  t = lexer_next(lexer);
  cassert_int_eq(t.kind, EOF_TOKEN);
  add_description(&test, lexer, &t);

  return test;
}

void lexer_multi_mixed(Tests *tests) {
  cassert_dap(tests, lexer_mixed());
  cassert_dap(tests, lexer_null_terminator());
  cassert_dap(tests, lexer_eof_null_terminator());
  cassert_dap(tests, lexer_string_semicolon());
  cassert_dap(tests, lexer_char_keyword());

  cassert_dap(tests, lexer_string_literal());
  cassert_dap(tests, lexer_single_quotes_in_string());
  cassert_dap(tests, lexer_escape_double_quotes_in_string());
  cassert_dap(tests, lexer_escape_single_quotes_in_string());
  cassert_dap(tests, lexer_unclosed_string());
  cassert_dap(tests, lexer_unclosed_string_at_eof());

  cassert_dap(tests, lexer_char_array_def());
  cassert_dap(tests, lexer_escaped_double_quote_in_single_quote());
  cassert_dap(tests, lexer_double_quote_in_single_quote());
  cassert_dap(tests, lexer_string_literal_after_char_literal_as_backslash());
  cassert_dap(tests, lexer_string_literal_after_string_with_backslashes());

  cassert_dap(tests, lexer_empty_string_literal());
}

Test lexer_all_keywords() {
  Test test = cassert_init_test("lexer_all_keywords");

  char *content = " auto"
                  " break"
                  " case"
                  " char"
                  " const"
                  " continue"
                  " default"
                  " do"
                  " double"
                  " else"
                  " enum"
                  " extern"
                  " float"
                  " for"
                  " goto"
                  " if"
                  " int"
                  " long"
                  " register"
                  " return"
                  " short"
                  " signed"
                  " sizeof"
                  " static"
                  " struct"
                  " switch"
                  " typedef"
                  " union"
                  " unsigned"
                  " void"
                  " volatile"
                  " while"
                  " size_t"
                  "";

  size_t len = strlen(content);
  Lexer *lexer = lexer_new(__FILE__, content, len, 0);

  Token token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_AUTO);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_BREAK);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_CASE);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_CHAR);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_CONST);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_CONTINUE);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_DEFAULT);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_DO);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_DOUBLE);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_ELSE);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_ENUM);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_EXTERN);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_FLOAT);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_FOR);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_GOTO);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_IF);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_INT);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_LONG);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_REGISTER);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_RETURN);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_SHORT);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_SIGNED);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_SIZEOF);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_STATIC);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_STRUCT);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_SWITCH);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_TYPEDEF);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_UNION);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_UNSIGNED);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_VOID);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_VOLATILE);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_WHILE);
  add_description(&test, lexer, &token);
  token = lexer_next(lexer);
  cassert_int_eq(token.kind, KEYWORD_SIZE_T);
  add_description(&test, lexer, &token);

  token = lexer_next(lexer);
  cassert_int_eq(token.kind, EOF_TOKEN);
  add_description(&test, lexer, &token);

  return test;
}

void lexer_keywords(Tests *tests) { cassert_dap(tests, lexer_all_keywords()); }

int main(int argc, char **argv) {
  cassert_tests {
    lexer_basic(&tests);
    lexer_combination(&tests);
    lexer_multi_mixed(&tests);
    lexer_keywords(&tests);
  }

#ifdef SHORT_LOG
  cassert_short_print_tests(&tests);
#else
  cassert_print_tests(&tests);
#endif // SHORT_LOG

  cassert_free_tests(&tests);
  free(tmp_buffer.elements);
  return 0;
}
