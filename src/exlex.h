#include <stddef.h>

#ifndef EXLEX_H_
#define EXLEX_H_

typedef struct Keyword {
  char *auto_t;
  char *break_t;
  char *case_t;
  char *char_t;
  char *const_t;
  char *continue_t;
  char *default_t;
  char *do_t;
  char *double_t;
  char *else_t;
  char *enum_t;
  char *extern_t;
  char *float_t;
  char *for_t;
  char *goto_t;
  char *if_t;
  char *int_t;
  char *long_t;
  char *register_t;
  char *return_t;
  char *short_t;
  char *signed_t;
  char *sizeof_t;
  char *static_t;
  char *struct_t;
  char *switch_t;
  char *typedef_t;
  char *union_t;
  char *unsigned_t;
  char *void_t;
  char *volatile_t;
  char *while_t;
} Keyword;

typedef struct Identifier {
  char *char_identifier;
} Identifier;

typedef struct Constant {
  size_t int_constant;
} Constant;

typedef struct StringLiteral {
  char quote;
} StringLiteral;

typedef struct Punctuator {
  char *lbracket_t;
  char *rbracket_t;
  char *lparen_t;
  char *rparen_t;
  char *lbrace_t;
  char *rbrace_t;
  char *point_t;
  char *pderef_t;
  char *increment_t;
  char *decrement_t;
  char *band_t;
  char *mul_t;
  char *add_t;
  char *sub_t;
  char *bnot_t;
  char *not_t;
  char *div_t;
  char *mod_t;
  char *leftshift_t;
  char *rightshift_t;
  char *lange_t;
  char *range_t;
  char *loreq_t;
  char *goreq_t;
  char *equal_t;
  char *noteq_t;
  char *bxor_t;
  char *bor_t;
  char *land_t;
  char *lor_t;
  char *ask_t;
  char *colon_t;
  char *semicolon_t;
  char *variadic_t;
  char *assign_t;
  char *muleq_t;
  char *diveq_t;
  char *modeq_t;
  char *addeq_t;
  char *subeq_t;
  char *leftshifteq_t;
  char *rightshifteq_t;
  char *bandeq_t;
  char *bxoreq_t;
  char *boreq_t;
  char *comma_t;
  char *htag_t;
  char *hhtag_t;

  /* char *langecolon_t = lbracket_t;    // '<: = [' */
  /* char *colonrange_t = rbracket_t;    // ':> = ]' */
  /* char *langelmod_t = lbrace_t;       // '<% = {' */
  /* char *kangelmod_t = rbrace_t;       // '%> = }' */
  /* char *modcolon_t = htag_t;          // '%: = #' */
  /* char *modcolonmodcolon_t = hhtag_t; // '%:%: = ##' */

} Punctuator;

typedef struct HeaderName {
  char *header;
} HeaderName;
typedef struct PPNumber {
  size_t ppnumber;
} PPNumber;
typedef struct CharacterConstant {
  char char_constant;
} CharacterConstant;
typedef struct NonWhiteSpaceChar {
  char non_white_space;
} NonWhiteSpaceChar;

typedef struct PreprocessingToken {
  HeaderName header_name;
  Identifier identifier;
  PPNumber pp_number;
  CharacterConstant character_constant;
  StringLiteral string_literal;
  Punctuator punctuator;
  NonWhiteSpaceChar non_white_space_char;

} PreprocessingToken;

typedef struct TokenKind {
  Keyword keyword;
  Identifier identifier;
  Constant constant;
  StringLiteral string_literal;
  Punctuator punctuator;

} TokenKind;

typedef struct TokenVarient {
  TokenKind token_kind;
  PreprocessingToken preprocessing_token;
}TokenVarient;

#include "lexer.h"

Lexer *lexer_token_set_string_literal(Lexer *lexer);
Lexer *lexer_token_set_keywords(Lexer *lexer);
Lexer *lexer_token_set_punctuator(Lexer *lexer);

#endif // EXLEX_H_

#ifdef EXLEX_IMPLEMENTAION

/* The function lexer_token_set_string_literal() sets the qoute to the */
/* internal lexer token. */
/* @param lexer The Lexer that will be modified. */
/* @return Lexer the that was passed in and was modified. */
Lexer *lexer_token_set_string_literal(Lexer *lexer) {

  lexer->token_varient.token_kind.string_literal.quote = '"';
  return lexer;
}

/* The function ptr lexer_token_set_keywords() is the function that sets the */
/* internal lexer tokens of type KEYWORD. */
/* @param lexer The lexer that will be modified. */
/* @return Lexer the that was passed in and was modified. */
Lexer *lexer_token_set_keywords(Lexer *lexer) {
  lexer->token_varient.token_kind.keyword.auto_t = "auto";
  lexer->token_varient.token_kind.keyword.break_t = "break";
  lexer->token_varient.token_kind.keyword.case_t = "case";
  lexer->token_varient.token_kind.keyword.char_t = "char";
  lexer->token_varient.token_kind.keyword.const_t = "const";
  lexer->token_varient.token_kind.keyword.continue_t = "continue";
  lexer->token_varient.token_kind.keyword.default_t = "default";
  lexer->token_varient.token_kind.keyword.do_t = "do";
  lexer->token_varient.token_kind.keyword.double_t = "double";
  lexer->token_varient.token_kind.keyword.else_t = "else";
  lexer->token_varient.token_kind.keyword.enum_t = "enum";
  lexer->token_varient.token_kind.keyword.extern_t = "extern";
  lexer->token_varient.token_kind.keyword.float_t = "float";
  lexer->token_varient.token_kind.keyword.for_t = "for";
  lexer->token_varient.token_kind.keyword.goto_t = "goto";
  lexer->token_varient.token_kind.keyword.if_t = "if";
  lexer->token_varient.token_kind.keyword.int_t = "int";
  lexer->token_varient.token_kind.keyword.long_t = "long";
  lexer->token_varient.token_kind.keyword.register_t = "register";
  lexer->token_varient.token_kind.keyword.return_t = "return";
  lexer->token_varient.token_kind.keyword.short_t = "short";
  lexer->token_varient.token_kind.keyword.signed_t = "signed";
  lexer->token_varient.token_kind.keyword.sizeof_t = "sizeof";
  lexer->token_varient.token_kind.keyword.static_t = "static";
  lexer->token_varient.token_kind.keyword.struct_t = "struct";
  lexer->token_varient.token_kind.keyword.switch_t = "switch";
  lexer->token_varient.token_kind.keyword.typedef_t = "typedef";
  lexer->token_varient.token_kind.keyword.union_t = "union";
  lexer->token_varient.token_kind.keyword.unsigned_t = "unsigned";
  lexer->token_varient.token_kind.keyword.void_t = "void";
  lexer->token_varient.token_kind.keyword.volatile_t = "volatile";
  lexer->token_varient.token_kind.keyword.while_t = "while";

  return lexer;
}

/* The function ptr lexer_token_set_punctuator() is the function that sets the
 */
/* internal lexer tokens of type PUNCTUATOR. */
/* @param lexer The lexer that will be modified. */
/* @return Lexer the that was passed in and was modified. */
Lexer *lexer_token_set_punctuator(Lexer *lexer) {
  lexer->token_varient.token_kind.punctuator.lbracket_t = "[";
  lexer->token_varient.token_kind.punctuator.rbracket_t = "]";
  lexer->token_varient.token_kind.punctuator.lparen_t = "(";
  lexer->token_varient.token_kind.punctuator.rparen_t = ")";
  lexer->token_varient.token_kind.punctuator.lbrace_t = "{";
  lexer->token_varient.token_kind.punctuator.rbrace_t = "}";
  lexer->token_varient.token_kind.punctuator.point_t = ".";
  lexer->token_varient.token_kind.punctuator.pderef_t = "->";
  lexer->token_varient.token_kind.punctuator.increment_t = "++";
  lexer->token_varient.token_kind.punctuator.decrement_t = "--";
  lexer->token_varient.token_kind.punctuator.band_t = "&";
  lexer->token_varient.token_kind.punctuator.mul_t = "*";
  lexer->token_varient.token_kind.punctuator.add_t = "+";
  lexer->token_varient.token_kind.punctuator.sub_t = "-";
  lexer->token_varient.token_kind.punctuator.bnot_t = "~";
  lexer->token_varient.token_kind.punctuator.not_t = "|";
  lexer->token_varient.token_kind.punctuator.div_t = "/";
  lexer->token_varient.token_kind.punctuator.mod_t = "%";
  lexer->token_varient.token_kind.punctuator.leftshift_t = "<<";
  lexer->token_varient.token_kind.punctuator.rightshift_t = ">>";
  lexer->token_varient.token_kind.punctuator.lange_t = "<";
  lexer->token_varient.token_kind.punctuator.range_t = ">";
  lexer->token_varient.token_kind.punctuator.loreq_t = "<=";
  lexer->token_varient.token_kind.punctuator.goreq_t = ">=";
  lexer->token_varient.token_kind.punctuator.equal_t = "==";
  lexer->token_varient.token_kind.punctuator.noteq_t = "!=";
  lexer->token_varient.token_kind.punctuator.bxor_t = "^";
  lexer->token_varient.token_kind.punctuator.bor_t = "|";
  lexer->token_varient.token_kind.punctuator.land_t = "&&";
  lexer->token_varient.token_kind.punctuator.lor_t = "||";
  lexer->token_varient.token_kind.punctuator.ask_t = "?";
  lexer->token_varient.token_kind.punctuator.colon_t = ":";
  lexer->token_varient.token_kind.punctuator.semicolon_t = ";";
  lexer->token_varient.token_kind.punctuator.variadic_t = "...";
  lexer->token_varient.token_kind.punctuator.assign_t = "=";
  lexer->token_varient.token_kind.punctuator.muleq_t = "*=";
  lexer->token_varient.token_kind.punctuator.diveq_t = "/=";
  lexer->token_varient.token_kind.punctuator.modeq_t = "%=";
  lexer->token_varient.token_kind.punctuator.addeq_t = "+=";
  lexer->token_varient.token_kind.punctuator.subeq_t = "-=";
  lexer->token_varient.token_kind.punctuator.leftshifteq_t = "<<=";
  lexer->token_varient.token_kind.punctuator.rightshifteq_t = ">>=";
  lexer->token_varient.token_kind.punctuator.bandeq_t = "&=";
  lexer->token_varient.token_kind.punctuator.bxoreq_t = "^=";
  lexer->token_varient.token_kind.punctuator.boreq_t = "|=";
  lexer->token_varient.token_kind.punctuator.comma_t = ",";
  lexer->token_varient.token_kind.punctuator.htag_t = "#";
  lexer->token_varient.token_kind.punctuator.hhtag_t = "##";

  return lexer;
}

#endif // EXLEX_IMPLEMENTAION
