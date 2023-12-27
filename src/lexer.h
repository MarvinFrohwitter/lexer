#include <stddef.h>

#ifndef LEXER_H_
#define LEXER_H_

typedef struct Lexer {
  const char *content;
  size_t content_lenght;
  size_t position;

#ifdef EXLEX_H_
  TokenVarient token_varient;
#endif // EXLEX_H_

} Lexer;

/* The variable that defines the escape characters. */
static const char ESCAPE[] = {'\n', '\r', '\t', '\f', '\\'};

/* The variable PUNCTUATORS is a pointer to the array of single tokens of type
 */
/* PUNCTUATOR. It contains the tokens to match on. */
static const char *PUNCTUATORS[] = {"[", "]", "(", ")", "{", "}", ".", "&", "*",
                                    "+", "-", "~", "|", "/", "%", "<", ">", "^",
                                    "|", "?", ":", ";", "=", ",", NULL};

/* The variable KEYWORDS is a pointer to the array of single tokens of type */
/* KEYWORD. It contains the tokens to match on. */
static const char *KEYWORDS[] = {
    "auto",     "break",  "case",    "char",   "const",    "continue",
    "default",  "do",     "double",  "else",   "enum",     "extern",
    "float",    "for",    "goto",    "if",     "int",      "long",
    "register", "return", "short",   "signed", "sizeof",   "static",
    "struct",   "switch", "typedef", "union",  "unsigned", "void",
    "volatile", "while",  NULL};

/* The Token Kind that can be detected by the lexer. */
typedef enum Kind {
  INVALID,
  ERROR,
  NUMBER,
  KEYWORD,
  STRINGLITERAL,
  IDENTIFIER,
  PUNCTUATOR,
  CONSTANT,
  PREPROCESSING
} Kind;

typedef struct Token {
  Kind kind;
  const char *content;
  size_t size;
} Token;

/* ========================================================================== */

Lexer lexer_new(char *content, size_t size, size_t position);
Token lexer_next(Lexer *lexer);
Token lexer_error(Lexer *lexer);
Token lexer_trace_token(Lexer *lexer);
Token lexer_invalid_token(Lexer *lexer);
Token lexer_chop_char(Lexer *lexer, size_t count);

int lexer_check_is_number(Lexer *lexer, Token *token);
int lexer_check_is_preproc(Lexer *lexer, Token *token);

void lexer_trim_left(Lexer *lexer);
int lexer_next_char_is(Lexer *lexer, char c);
int lexer_char_is(Lexer *lexer, char c);
int lexer_is_keyword(Lexer *lexer, size_t length);
int lexer_is_punctuator(Lexer *lexer, size_t length, size_t max);
int lexer_check_punctuator_lookahead(Lexer *lexer);
int lexer_check_boundery(Lexer *lexer);

/* ========================================================================== */
int is_escape_seq(char c);
int is_sybol_alnum_and_(char c);
int is_sybol_alpha_and_(char c);

/* ========================================================================== */

#endif // LEXER_H_
