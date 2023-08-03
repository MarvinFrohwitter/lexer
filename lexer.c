#include <stdio.h>
#include "lexer.h"

typedef struct Keyword {
  char *if_t;
  char *else_t;
} Keyword;

typedef struct Identifier {
} Identifier;
typedef struct Constant {
} Constant;
typedef struct StringLiteral {
} StringLiteral;
typedef struct Punctuator {
} Punctuator;

typedef struct HeaderName {
} HeaderName;
typedef struct PPNumber {
} PPNumber;
typedef struct CharacterConstant {
} CharacterConstant;
typedef struct NonWhiteSpaceChar {
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

typedef struct Token {
  Keyword keyword;
  Identifier identifier;
  Constant constant;
  StringLiteral string_literal;
  Punctuator punctuator;

} Token;

typedef struct TokenVarient {
  Token token;
  PreprocessingToken preprocessing_token;
} TokenVarient;

typedef struct Lexer {
  TokenVarient token_varient;
} Lexer;

Lexer *create_Lexer() {
  Lexer *lexer = {0};
  return lexer;
}

int main(int argc, char *argv[]) {

  Lexer *lexer = create_Lexer();
  printf("Hello World!\n");

  return 0;
}
