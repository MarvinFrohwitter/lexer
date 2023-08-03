#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "./lexer.h"

Lexer lexer_new(char *content, size_t size) {
  Lexer lexer;
  lexer = *lexer_set_keywords(&lexer);
  lexer = *lexer_set_punctuator(&lexer);

  lexer.data->size = size;
  lexer.data->content = content;
  return lexer;
}
Lexer *lexer_set_punctuator(Lexer *lexer) {
  lexer->token_varient.token.punctuator.lbracket_t = "[";
  lexer->token_varient.token.punctuator.rbracket_t = "]";
  lexer->token_varient.token.punctuator.lparen_t = "(";
  lexer->token_varient.token.punctuator.rparen_t = ")";
  lexer->token_varient.token.punctuator.lbrace_t = "{";
  lexer->token_varient.token.punctuator.rbrace_t = "}";
  lexer->token_varient.token.punctuator.point_t = ".";
  lexer->token_varient.token.punctuator.pderef_t = "->";
  lexer->token_varient.token.punctuator.increment_t = "++";
  lexer->token_varient.token.punctuator.decrement_t = "--";
  lexer->token_varient.token.punctuator.band_t = "&";
  lexer->token_varient.token.punctuator.mul_t = "*";
  lexer->token_varient.token.punctuator.add_t = "+";
  lexer->token_varient.token.punctuator.sub_t = "-";
  lexer->token_varient.token.punctuator.bnot_t = "~";
  lexer->token_varient.token.punctuator.not_t = "|";
  lexer->token_varient.token.punctuator.div_t = "/";
  lexer->token_varient.token.punctuator.mod_t = "%";
  lexer->token_varient.token.punctuator.leftshift_t = "<<";
  lexer->token_varient.token.punctuator.rightshift_t = ">>";
  lexer->token_varient.token.punctuator.lange_t = "<";
  lexer->token_varient.token.punctuator.range_t = ">";
  lexer->token_varient.token.punctuator.loreq_t = "<=";
  lexer->token_varient.token.punctuator.goreq_t = ">=";
  lexer->token_varient.token.punctuator.equal_t = "==";
  lexer->token_varient.token.punctuator.noteq_t = "!=";
  lexer->token_varient.token.punctuator.bxor_t = "^";
  lexer->token_varient.token.punctuator.bor_t = "|";
  lexer->token_varient.token.punctuator.land_t = "&&";
  lexer->token_varient.token.punctuator.lor_t = "||";
  lexer->token_varient.token.punctuator.ask_t = "?";
  lexer->token_varient.token.punctuator.colon_t = ":";
  lexer->token_varient.token.punctuator.semicolon_t = ";";
  lexer->token_varient.token.punctuator.variadic_t = "...";
  lexer->token_varient.token.punctuator.assign_t = "=";
  lexer->token_varient.token.punctuator.muleq_t = "*=";
  lexer->token_varient.token.punctuator.diveq_t = "/=";
  lexer->token_varient.token.punctuator.modeq_t = "%=";
  lexer->token_varient.token.punctuator.addeq_t = "+=";
  lexer->token_varient.token.punctuator.subeq_t = "-=";
  lexer->token_varient.token.punctuator.leftshifteq_t = "<<=";
  lexer->token_varient.token.punctuator.rightshifteq_t = ">>=";
  lexer->token_varient.token.punctuator.bandeq_t = "&=";
  lexer->token_varient.token.punctuator.bxoreq_t = "^=";
  lexer->token_varient.token.punctuator.boreq_t = "|=";
  lexer->token_varient.token.punctuator.comma_t = ",";
  lexer->token_varient.token.punctuator.htag_t = "#";
  lexer->token_varient.token.punctuator.hhtag_t = "##";

  return lexer;
}

Lexer *lexer_set_keywords(Lexer *lexer) {
  lexer->token_varient.token.keyword.auto_t = "auto";
  lexer->token_varient.token.keyword.break_t = "break";
  lexer->token_varient.token.keyword.case_t = "case";
  lexer->token_varient.token.keyword.char_t = "char";
  lexer->token_varient.token.keyword.const_t = "const";
  lexer->token_varient.token.keyword.continue_t = "continue";
  lexer->token_varient.token.keyword.default_t = "default";
  lexer->token_varient.token.keyword.do_t = "do";
  lexer->token_varient.token.keyword.double_t = "double";
  lexer->token_varient.token.keyword.else_t = "else";
  lexer->token_varient.token.keyword.enum_t = "enum";
  lexer->token_varient.token.keyword.extern_t = "extern";
  lexer->token_varient.token.keyword.float_t = "float";
  lexer->token_varient.token.keyword.for_t = "for";
  lexer->token_varient.token.keyword.goto_t = "goto";
  lexer->token_varient.token.keyword.if_t = "if";
  lexer->token_varient.token.keyword.int_t = "int";
  lexer->token_varient.token.keyword.long_t = "long";
  lexer->token_varient.token.keyword.register_t = "register";
  lexer->token_varient.token.keyword.return_t = "return";
  lexer->token_varient.token.keyword.short_t = "short";
  lexer->token_varient.token.keyword.signed_t = "signed";
  lexer->token_varient.token.keyword.sizeof_t = "sizeof";
  lexer->token_varient.token.keyword.static_t = "static";
  lexer->token_varient.token.keyword.struct_t = "struct";
  lexer->token_varient.token.keyword.switch_t = "switch";
  lexer->token_varient.token.keyword.typedef_t = "typedef";
  lexer->token_varient.token.keyword.union_t = "union";
  lexer->token_varient.token.keyword.unsigned_t = "unsigned";
  lexer->token_varient.token.keyword.void_t = "void";
  lexer->token_varient.token.keyword.volatile_t = "volatile";
  lexer->token_varient.token.keyword.while_t = "while";

  return lexer;
}

int main(int argc, char *argv[]) {

  Lexer lexer = lexer_new("atestjkh", 8);

  // NOTE:White space Characters:
  // Blank space, newline, horizontal tab, carriage return and
  // form feed.

  printf("%s\n", lexer.token_varient.token.keyword.auto_t);
  printf("%s\n", lexer.token_varient.token.punctuator.bxor_t);
  char *h = "Succses";
  printf("%s\n", h);

  return 0;
}
