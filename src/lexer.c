#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GUTILS_H_IMPLEMENATION
#include "../../gutils/gutils.h"
#include "./lexer.h"

// The function lexer_new() creates the new state of a lexer.
//     @param content The content that the lexer has to tokenise.
//     @param size The content length.
//     @param The position of the content the lexer should begin tokenise.
//     @return Lexer The function returns a new initilized lexer.
Lexer lexer_new(char *content, size_t size, size_t position) {
  Lexer lexer;
  lexer_token_set_string_literal(&lexer);
  lexer_token_set_keywords(&lexer);
  lexer_token_set_punctuator(&lexer);
  lexer.content_lenght = size;
  lexer.content = content;
  lexer.position = position;

  return lexer;
}

// The function lexer_token_set_string_literal() sets the qoute to the internal
// lexer token.
// @param lexer The Lexer that will be modified.
// @return Lexer the that was passed in and was modified.
Lexer *lexer_token_set_string_literal(Lexer *lexer) {

  lexer->token_varient.token_kind.string_literal.quote = '"';
  return lexer;
}

// The variable PUNCTUATORS is a pointer to the array of single tokens of type
// PUNCTUATOR. It contains the tokens to match on.
const char *PUNCTUATORS[] = {"[", "]", "(", ")", "{", "}", ".", "&", "*",
                             "+", "-", "~", "|", "/", "%", "<", ">", "^",
                             "|", "?", ":", ";", "=", ",", NULL};

// The function ptr lexer_token_set_punctuator() is the function that sets the
// internal lexer tokens of type PUNCTUATOR.
// @param lexer The lexer that will be modified.
// @return Lexer the that was passed in and was modified.
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

// The variable KEYWORDS is a pointer to the array of single tokens of type
// KEYWORD. It contains the tokens to match on.
const char *KEYWORDS[] = {"auto",     "break",   "case",   "char",     "const",
                          "continue", "default", "do",     "double",   "else",
                          "enum",     "extern",  "float",  "for",      "goto",
                          "if",       "int",     "long",   "register", "return",
                          "short",    "signed",  "sizeof", "static",   "struct",
                          "switch",   "typedef", "union",  "unsigned", "void",
                          "volatile", "while",   NULL};

// The function ptr lexer_token_set_keywords() is the function that sets the
// internal lexer tokens of type KEYWORD.
// @param lexer The lexer that will be modified.
// @return Lexer the that was passed in and was modified.
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

// The function lexer_chop_char() consumes the amount of chars given by the
// count. So it cuts of the content that the lexer tokenise form the left.
// @param lexer The given Lexer that contains the current state.
// @param count The amount of chars that will be choped from the left of the
// content.
// @return Lexer the that was passed in and was modified.
Token lexer_chop_char(Lexer *lexer, size_t count) {

  for (size_t i = 0; i < count; i++) {
    if (!lexer_check_boundery(lexer)) {
      return lexer_error(lexer);
    }
    lexer->position = lexer->position + 1;
  }
  return lexer_invalid_token(lexer);
}

// The function lexer_invalid_token() sets the lexer values to the type of an
// invalid token.
// @param lexer The given Lexer that contains the current state.
// @return Token that has values of kind=INVALID, size=1 and content= current
// lexer position.
Token lexer_invalid_token(Lexer *lexer) {
  Token token;
  token.kind = INVALID;
  token.size = 1;
  token.content = &lexer->content[lexer->position];
  return token;
}

// The function check_boundery() checks for the length of the lexer content, if
// the current lexer position is in the content length.
// @param lexer The given Lexer that contains the current state.
// @return boolean True, if the lexer position is in the content length,
// otherwise false.
int lexer_check_boundery(Lexer *lexer) {
  if (lexer->position < lexer->content_lenght) {
    return 1;
  }
  return 0;
}

// The function lexer_next_char_is() returns, if the next character in the
// content is  equal to the given char c.
// @param lexer The given Lexer that contains the current state.
// @param c The character that will be compared with the lexer position.
// @return boolean True, if the given char c is identical to the next char in
// the content that lexer tokenise.
int lexer_next_char_is(Lexer *lexer, char c) {
  if (c == lexer->content[lexer->position + 1]) {
    return 1;
  }
  return 0;
}

// The function lexer_char_is() returns, if the character in the content is
// equal to the given char c.
// @param lexer The given Lexer that contains the current state.
// @param c The character that will be compared with the lexer position.
// @return boolean True, if the given char c is identical to the current char in
// the content that lexer tokenise.
int lexer_char_is(Lexer *lexer, char c) {
  if (c == lexer->content[lexer->position]) {
    return 1;
  }
  return 0;
}

// The function lexer_trim_left() consumes the characters from the left side of
// the given content. If the content matches isspace().
// White space Characters: Blank space, newline, horizontal tab, carriage
// return and form feed.
// @param lexer The given Lexer that contains the current state.
void lexer_trim_left(Lexer *lexer) {
  while (lexer_check_boundery(lexer) &&
         isspace(lexer->content[lexer->position])) {
    lexer_chop_char(lexer, 1);
  }
}

// The function lexer_is_keyword() returns, if the lexer has found a keyword of
// the given length at the current lexer content position.
// @param lexer The given Lexer that contains the current state.
// @param length The character length the keyword should have.
// @return boolean True, if the content at the current position has a keyword,
// otherwise false.
int lexer_is_keyword(Lexer *lexer, size_t length) {
  size_t array_count = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]) - 1;

  for (size_t i = 0; i < array_count; i++) {
    // The position of the lexer is on the next char.
    if (strncmp(KEYWORDS[i], &lexer->content[lexer->position - length],
                length) == 0) {
      return 1;
    }
  }
  return 0;
}

// The function lexer_is_punctuator() returns, if the lexer has found a
// punctuator of the given length at the current lexer content position.
// @param lexer The given Lexer that contains the current state.
// @param length The character length the punctuator should have.
// @param max The maximum length a punctuator is searched by. 0 if the complete
// default list of punctuators should be searched thru.
// @return boolean True, if the content at the current position has a punctuator
// of given length, otherwise false.
int lexer_is_punctuator(Lexer *lexer, size_t length, size_t max) {
  if (max == 0) {
    max = sizeof(PUNCTUATORS) / sizeof(PUNCTUATORS[0]) - 1;
  }

  // The position of the lexer is on the next char.
  for (size_t i = 0; i < max; i++) {
    if (strncmp(PUNCTUATORS[i], &lexer->content[lexer->position - length],
                length) == 0) {
      return 1;
    }
  }
  return 0;
}

// The function lexer_check_punctuator_lookahead() computes, if the next
// character can also be part of the punctuator.
// @param lexer The given Lexer that contains the current state.
// @return boolean True, if the next character is also part of the punctuator,
// otherwise false.
int lexer_check_punctuator_lookahead(Lexer *lexer) {

  if (lexer_char_is(lexer, '<') && lexer_next_char_is(lexer, '<') &&
      lexer->content[lexer->position + 2] == '=') {
    lexer_chop_char(lexer, 3);
  } else if (lexer_char_is(lexer, '>') && lexer_next_char_is(lexer, '>') &&
             lexer->content[lexer->position + 2] == '=') {
    lexer_chop_char(lexer, 3);
  } else if (lexer_char_is(lexer, '.') && lexer_next_char_is(lexer, '.') &&
             lexer->content[lexer->position + 2] == '.') {
    lexer_chop_char(lexer, 3);
  } else if (lexer_char_is(lexer, '<') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '>') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '<') && lexer_next_char_is(lexer, '<')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '>') && lexer_next_char_is(lexer, '>')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '=') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '!') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '&') && lexer_next_char_is(lexer, '&')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '|') && lexer_next_char_is(lexer, '|')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '-') && lexer_next_char_is(lexer, '>')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '+') && lexer_next_char_is(lexer, '+')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '-') && lexer_next_char_is(lexer, '-')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '*') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '/') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '%') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '+') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '-') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '&') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '^') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '|') && lexer_next_char_is(lexer, '=')) {
    lexer_chop_char(lexer, 2);
  } else if (lexer_char_is(lexer, '#') && lexer_next_char_is(lexer, '#')) {
    lexer_chop_char(lexer, 2);
  } else {
    lexer_chop_char(lexer, 1);
    return 0;
  }
  return 1;
}

// ===========================================================================
// The function is_escape_seq() returns, if the given char is equal to an escape
// character.
// @param c The char to compare.
// @return boolean True, if the given character is an escape char, otherwise
// false.
int is_escape_seq(char c) {
  if (c == '\n' || c == '\r' || c == '\t' || c == '\f' || c == '\\') {
    return 1;
  }
  return 0;
}

// The function is_sybol_alnum_and_() computes the, if the characters in the
// name of an identifier is part of the alnum group and the underscore.
// @param c The character to check for.
// @return boolean True, if the given character is part of the alnum group and
// the additional underscore.
int is_sybol_alnum_and_(char c) { return isalnum(c) || c == '_'; }

// The function is_sybol_alpha_and_() computes the, if the characters in the
// name of an identifier is part of the alpha group and the underscore.
// @param c The character to check for.
// @return boolean True, if the given character is part of the alpha group and
// the additional underscore.
int is_sybol_alpha_and_(char c) { return isalpha(c) || c == '_'; }

// ===========================================================================

Token lexer_next(Lexer *lexer) {
  Token token;
  lexer_trim_left(lexer);
  if (is_escape_seq(lexer->content[lexer->position])) {
    return lexer_invalid_token(lexer);
  }

  token.content = &lexer->content[lexer->position];
  if (lexer->position > lexer->content_lenght) {
    return lexer_invalid_token(lexer);
  }

  if (lexer_char_is(lexer, '"') && lexer_check_boundery(lexer)) {
    if (!lexer_next_char_is(lexer, '"') && !lexer_check_boundery(lexer)) {
      return lexer_error(lexer);
    }
    token.kind = STRINGLITERAL;
    token.size = lexer->position;
    lexer_chop_char(lexer, 1);
    while (lexer_check_boundery(lexer)) {
      if (lexer_char_is(lexer, '"')) {
        lexer_chop_char(lexer, 1);
        break;
      } else if (is_escape_seq(lexer->content[lexer->position])) {
        lexer_chop_char(lexer, 1);
        if (!lexer_check_boundery(lexer)) {
          return lexer_error(lexer);
        }
      } else {
        lexer_chop_char(lexer, 1);
      }
    }
    token.size = lexer->position - token.size;
    return token;
  }

  if (isdigit(lexer->content[lexer->position])) {
    token.kind = NUMBER;
    token.size = lexer->position;
    lexer_chop_char(lexer, 1);

    if (lexer->content[lexer->position - 1] == '0' &&
        lexer_char_is(lexer, 'x')) {
      lexer_chop_char(lexer, 1);
      for (size_t i = 1; i <= 6; i++) {
        if (isxdigit(lexer->content[lexer->position]) &&
            lexer_check_boundery(lexer)) {
          lexer_chop_char(lexer, 1);
        } else {
          return lexer_error(lexer);
        }
      }
      if (!lexer_char_is(lexer, ' ')) {
        return lexer_error(lexer);
      }
    } else {
      while (lexer_check_boundery(lexer) &&
             isdigit(lexer->content[lexer->position]) &&
             !is_escape_seq(lexer->content[lexer->position])) {
        lexer_chop_char(lexer, 1);
      }
    }
    token.size = lexer->position - token.size;
    return token;
  }

  if (isalpha(lexer->content[lexer->position])) {

    size_t startpos = token.size = lexer->position;
    lexer_chop_char(lexer, 1);
    while (lexer_check_boundery(lexer) &&
           isalpha(lexer->content[lexer->position])) {
      lexer_chop_char(lexer, 1);
    }

    token.size = lexer->position - token.size;

    if (lexer_is_keyword(lexer, token.size)) {

      token.kind = KEYWORD;
      return token;
    } else {
      lexer->position = startpos;
    }
  }

  if (is_sybol_alpha_and_(lexer->content[lexer->position])) {
    token.kind = IDENTIFIER;
    token.size = lexer->position;
    lexer_chop_char(lexer, 1);
    while (lexer_check_boundery(lexer) &&
           is_sybol_alnum_and_(lexer->content[lexer->position])) {
      lexer_chop_char(lexer, 1);
    }
    token.size = lexer->position - token.size;
    return token;
  }

  if (lexer_check_boundery(lexer)) {
    token.size = lexer->position;
    lexer_chop_char(lexer, 1);
    if (lexer_is_punctuator(lexer, 1, 0)) {
      token.kind = PUNCTUATOR;
      lexer->position = lexer->position - 1;

      if (!lexer_check_punctuator_lookahead(lexer)) {
        token.size = 1;
        return token;
      }

      token.size = lexer->position - token.size;
      return token;
    }
  }

  token.kind = INVALID;
  if (!lexer_check_boundery(lexer)) {
    return lexer_invalid_token(lexer);
  } else {
    lexer_chop_char(lexer, 1);
  }
  token.size = 1;
  return token;
}

Token lexer_error(Lexer *lexer) {
  fprintf(stderr, "ERROR: Broken Token! [%c] was given\n",
          lexer->content[lexer->position]);
  fprintf(stderr, "    CHAR:%c\n", lexer->content[lexer->position]);
  fprintf(stderr, "    POS:%zu\n", lexer->position);

  return lexer_invalid_token(lexer);
}

int main(void) {

  // size_t size = 32;
  // char return_buffer[size];
  // while (feof(stdin) == 0) {
    // char *content_to_parse = readline(stdin, size, return_buffer);
    // char *content_to_parse = fgets(return_buffer, size, stdin);
  // }
    char *content_to_parse =
    "do or int BUS hallo  0xBBAACC main(void){return 0\"klaer\";} 23 hallo"
    "void  9   HASE  while   \"jkkl\naer\" \"nijt\" .. ... <<= // -1 ";
    size_t len = strlen(content_to_parse);
    Lexer lexer = lexer_new(content_to_parse, len, 0);

    while (lexer.content_lenght - 1 >= lexer.position) {
      Token t = lexer_next(&lexer);
      char *temp;
      char *tofree_temp = temp = malloc(t.size * sizeof(char));
      strncpy(temp, t.content, t.size);
      temp[t.size] = 0;
      printf("%s form type %u\n", temp, t.kind);
      free(tofree_temp);
    }
  char *h = "Succses";
  printf("%s\n", h);

  return 0;
}
