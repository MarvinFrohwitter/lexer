#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./lexer.h"

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

Lexer *lexer_token_set_string_literal(Lexer *lexer) {

  lexer->token_varient.token_kind.string_literal.quote = '"';
  return lexer;
}
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

const char *KEYWORDS[] = {"auto",     "break",   "case",   "char",     "const",
                          "continue", "default", "do",     "double",   "else",
                          "enum",     "extern",  "float",  "for",      "goto",
                          "if",       "int",     "long",   "register", "return",
                          "short",    "signed",  "sizeof", "static",   "struct",
                          "switch",   "typedef", "union",  "unsigned", "void",
                          "volatile", "while",   NULL};

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

void lexer_chop_char(Lexer *lexer, size_t count) {

  for (size_t i = 0; i < count; i++) {
    assert(lexer->position < lexer->content_lenght);

    lexer->position = lexer->position + 1;
  }
}

int lexer_next_char_is(Lexer *lexer, char c) {
  if (c == lexer->content[lexer->position + 1]) {
    return 1;
  }
  return 0;
}

int lexer_char_is(Lexer *lexer, char c) {
  if (c == lexer->content[lexer->position]) {
    return 1;
  }
  return 0;
}

int is_escape_seq(Lexer *lexer, char c) {
  if (c == '\n' || c == '\r' || c == '\t' || c == '\f' || c == '\\') {
    return 1;
  }
  return 0;
}

void lexer_trim_left(Lexer *lexer) {
  // NOTE:White space Characters: Blank space, newline, horizontal tab, carriage
  // return and form feed.
  while (lexer->position < lexer->content_lenght &&
         isspace(lexer->content[lexer->position]) &&
         !is_escape_seq(lexer, lexer->content[lexer->position])) {
    lexer_chop_char(lexer, 1);
  }
}
int is_keyword(char *word) {
  for (size_t i = 0; i <= sizeof(KEYWORDS); i++) {
    if (strcmp(KEYWORDS[i], word) == 0) {
      return 1;
    }
  }
  return 0;
}

int is_sybol_alnum(char c) { return isalnum(c) || c == '_'; }
int is_sybol_alpha_and_(char c) { return isalpha(c) || c == '_'; }

Token lexer_next(Lexer *lexer) {
  lexer_trim_left(lexer);

  Token token;

  token.content = &lexer->content[lexer->position];
  if (lexer->position > lexer->content_lenght) {
    token.kind = INVALID;
    token.size = 0;
    token.content = &lexer->content[0];
    return token;
  }

  if (lexer->content[lexer->position] == '"' &&
      lexer->position < lexer->content_lenght) {
    if (!lexer_next_char_is(lexer, '"') &&
        lexer->position == lexer->content_lenght) {
    error:
      fprintf(stderr, "ERROR: Broken Token! Just one %c was given\n",
              lexer->content[lexer->position]);
      fprintf(stderr, "    CHAR:%c\n", lexer->content[lexer->position]);
      fprintf(stderr, "    POS:%zu\n", lexer->position);
      token.kind = INVALID;
      token.size = 1;
      return token;
    }
    token.kind = STRINGLITERAL;
    token.size = lexer->position;
    lexer_chop_char(lexer, 1);
    while (lexer->position < lexer->content_lenght) {
      if (lexer->content[lexer->position] == '"') {
        lexer_chop_char(lexer, 1);
        break;
      } else if (is_escape_seq(lexer, lexer->content[lexer->position])) {
        lexer_chop_char(lexer, 1);
        if (lexer->position == lexer->content_lenght) {
          goto error;
        }
      } else if (is_sybol_alpha_and_(lexer->content[lexer->position])) {
        lexer_chop_char(lexer, 1);

      } else {

        // TODO: The escape char is escaped/interpreted before the lexer handels
        // the char.
        // NOTE: Parser task.
        fprintf(stderr,
                "ERROR: Unreachable! Unknown STRING char: %c occured. \n",
                lexer->content[lexer->position]);
        fprintf(stderr, "CHAR:%c\n", lexer->content[lexer->position]);
        fprintf(stderr, "POS:%zu\n", lexer->position);
        fprintf(stderr, "The escape char is escaped/interpreted before the "
                        "lexer handels the char.\n");
        lexer_chop_char(lexer, 1);
      }
    }
    token.size = lexer->position - token.size;
    return token;
  }

  // TODO: For hex do 2+6 itterations than chaeck e.g.'0xAABB88'
  if (isdigit(lexer->content[lexer->position])) {
    token.kind = NUMBER;
    token.size = lexer->position;
    lexer_chop_char(lexer, 1);

    if (lexer->content[lexer->position - 1] == '0' &&
        lexer->content[lexer->position] == 'x') {
      lexer_chop_char(lexer, 1);
      for (size_t i = 1; i <= 6; i++) {
        if (isxdigit(lexer->content[lexer->position]) &&
            lexer->position < lexer->content_lenght) {
          lexer_chop_char(lexer, 1);
        } else {
          goto error;
        }
      }
      if (!lexer_char_is(lexer, ' ')) {
        goto error;
      }
    } else {
      while (lexer->position < lexer->content_lenght &&
             isdigit(lexer->content[lexer->position]) &&
             !is_escape_seq(lexer, lexer->content[lexer->position])) {
        lexer_chop_char(lexer, 1);
        token.size = token.size + 1;
      }
    }
    token.size = lexer->position - token.size;
    return token;
  }

  if (isalpha(lexer->content[lexer->position])) {

    size_t startpos = token.size = lexer->position;
    lexer_chop_char(lexer, 1);
    while (lexer->position < lexer->content_lenght &&
           isalpha(lexer->content[lexer->position])) {
      lexer_chop_char(lexer, 1);
    }

    token.size = lexer->position - token.size;
    char *word;
    char *tofree_word = word = malloc(sizeof(char) * token.size + 1);
    strncpy(word, &lexer->content[startpos], token.size);
    strcat(word, "\0");
    if (is_keyword(word)) {

      token.kind = KEYWORD;
      free(tofree_word);
      return token;
    } else {
      free(tofree_word);
      lexer->position = startpos;
    }
  }

  if (is_sybol_alpha_and_(lexer->content[lexer->position])) {
    token.kind = IDENTIFIER;
    token.size = lexer->position;
    lexer_chop_char(lexer, 1);
    while (lexer->position < lexer->content_lenght &&
           is_sybol_alnum(lexer->content[lexer->position])) {
      lexer_chop_char(lexer, 1);
    }
    token.size = lexer->position - token.size;
    return token;
  }

  token.kind = INVALID;
  if (lexer->position >= lexer->content_lenght) {
    token.size = 0;
    token.content = &lexer->content[0];
    return token;
  } else {
    lexer_chop_char(lexer, 1);
  }
  token.size = 1;
  return token;
}
int main(int argc, char *argv[]) {

  char *content_to_parse = "int 0xBBAACC main(void){return 0\"klaer\";}";
  // char *content_to_parse = "   9        \"jkkl\naer\"  \"nijt\"       5";
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

  // printf("%s\n", lexer.token_varient.token_kind.keyword.auto_t);
  // printf("%s\n", lexer.token_varient.token_kind.punctuator.bxor_t);
  // char *h = "Succses";
  // printf("%s\n", h);

  return 0;
}
