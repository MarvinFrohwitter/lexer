// Copyright (c) 2023 Marvin Frohwitter. All Rights Reserved.
#ifndef LEXER_H_
#define LEXER_H_
#include <stddef.h>

typedef struct Lexer {
  char *content;
  size_t content_lenght;
  size_t position;

} Lexer;

/* The variable that defines the escape characters. */
static const char ESCAPE[] = {'\n', '\r', '\t', '\f', '\\'};

/* The variable PUNCTUATORS is a pointer to the array of single tokens of type
 */
/* PUNCTUATOR. It contains the tokens to match on. */
static const char *PUNCTUATORS[] = {
    // NOTE: It is important that the '.' is at the end of the array! It is
    // checked by the number function for floating point calculations.
    ",",  "]", ")", "=", ";", "{", "}", "&", "*", "+", "-", "~", "|", "/",
    "\\", "%", "<", ">", "^", "|", "?", ":", "(", "[", "!", "'", ".", NULL};

/* The variable KEYWORDS is a pointer to the array of single tokens of type */
/* KEYWORD. It contains the tokens to match on. */
static const char *KEYWORDS[] = {
    "auto",     "break",    "size_t", "case",    "char",   "const",
    "continue", "default",  "do",     "double",  "else",   "enum",
    "extern",   "float",    "for",    "goto",    "if",     "int",
    "long",     "register", "return", "short",   "signed", "sizeof",
    "static",   "struct",   "switch", "typedef", "union",  "unsigned",
    "void",     "volatile", "while",  NULL};

/* The Token Kind that can be detected by the lexer. */
typedef enum Kind {
  INVALID = 0,
  ERROR = 1,
  NUMBER = 2,
  KEYWORD = 3,
  STRINGLITERAL = 4,
  IDENTIFIER = 5,
  PUNCTUATOR = 6,
  CONSTANT = 7,
  PREPROCESSING = 8,
  COMMENT = 9,
  EOF_TOKEN = 10,

#ifdef EXLEX_IMPLEMENTATION

  PUNCT_SINGLEQUOTE = 39, // "'"
  PUNCT_BACKSLASH = '\\', // 92

  // ALL PUNCTUATORS
  PUNCT_NOT = '!',       // 33
  PUNCT_HTAG = '#',      // 35
  PUNCT_MOD = '%',       // 37
  PUNCT_BAND = '&',      // 38
  PUNCT_MUL = '*',       // 42
  PUNCT_ADD = '+',       // 43
  PUNCT_COMMA = ',',     // 44
  PUNCT_SUB = '-',       // 45
  PUNCT_DIV = '/',       // 47
  PUNCT_LPAREN = '(',    // 50
  PUNCT_RPAREN = ')',    // 51
  PUNCT_POINT = '.',     // 56
  PUNCT_COLON = ':',     // 58
  PUNCT_SEMICOLON = ';', // 59
  PUNCT_LANGE = '<',     // 60
  PUNCT_ASSIGN = '=',    // 61
  PUNCT_RANGE = '>',     // 62
  PUNCT_ASK = '?',       // 63
  PUNCT_LBRACKET = '[',  // 91
  PUNCT_RBRACKET = ']',  // 93
  PUNCT_BXOR = '^',      // 94
  PUNCT_LBRACE = '{',    // 123
  PUNCT_BOR = '|',       // 124
  PUNCT_RBRACE = '}',    // 125
  PUNCT_BNOT = '~',      // 126

  PUNCT_EQUAL = 257,        //'=='
  PUNCT_MULEQ = 258,        //'*='
  PUNCT_DIVEQ = 259,        //'/='
  PUNCT_MODEQ = 260,        //'%='
  PUNCT_ADDEQ = 261,        //'+='
  PUNCT_SUBEQ = 262,        //'-='
  PUNCT_LEFTSHIFTEQ = 263,  //'<<='
  PUNCT_RIGHTSHIFTEQ = 264, //'>>='
  PUNCT_BANDEQ = 265,       //'&='
  PUNCT_BOREQ = 266,        //'|='
  PUNCT_BXOREQ = 267,       //'^='
  PUNCT_NOTEQ = 268,        //'!='
  PUNCT_LOREQ = 269,        //'<='
  PUNCT_GOREQ = 270,        //'>='
  PUNCT_LAND = 271,         //'&&'
  PUNCT_LOR = 272,          //'||'
  PUNCT_LEFTSHIFT = 273,    //'<<'
  PUNCT_RIGHTSHIFT = 274,   //'>>'
  PUNCT_PDEREF = 275,       //'->'
  PUNCT_DECREMENT = 276,    //'--'
  PUNCT_INCREMENT = 277,    //'++'
  PUNCT_HHTAG = 278,        //'##'
  PUNCT_VARIADIC = 279,     //'...'

  // LANGECOLON = LBRACKET;    // '<: = ['
  // COLONRANGE = RBRACKET;    // ':> = ]'
  // LANGELMOD = LBRACE;       // '<% = {'
  // KANGELMOD = RBRACE;       // '%> = }'
  // MODCOLON = HTAG;          // '%: = #'
  // MODCOLONMODCOLON = HHTAG; // '%:%: = ##'

  // ALL KEYWORDS

  KEYWORD_AUTO = 280,
  KEYWORD_BREAK = 281,
  KEYWORD_CASE = 282,
  KEYWORD_CHAR = 283,
  KEYWORD_CONST = 284,
  KEYWORD_CONTINUE = 285,
  KEYWORD_DEFAULT = 286,
  KEYWORD_DO = 287,
  KEYWORD_DOUBLE = 288,
  KEYWORD_ELSE = 289,
  KEYWORD_ENUM = 290,
  KEYWORD_EXTERN = 291,
  KEYWORD_FLOAT = 292,
  KEYWORD_FOR = 293,
  KEYWORD_GOTO = 294,
  KEYWORD_IF = 295,
  KEYWORD_INT = 296,
  KEYWORD_LONG = 297,
  KEYWORD_REGISTER = 298,
  KEYWORD_RETURN = 299,
  KEYWORD_SHORT = 300,
  KEYWORD_SIGNED = 301,
  KEYWORD_SIZEOF = 302,
  KEYWORD_STATIC = 303,
  KEYWORD_STRUCT = 304,
  KEYWORD_SWITCH = 305,
  KEYWORD_TYPEDEF = 306,
  KEYWORD_UNION = 307,
  KEYWORD_UNSIGNED = 308,
  KEYWORD_VOID = 309,
  KEYWORD_VOLATILE = 310,
  KEYWORD_WHILE = 311,
  KEYWORD_SIZE_T = 312,

#endif // EXLEX_IMPLEMENTATION

} Kind;

typedef struct Token {
  Kind kind;
  const char *content;
  size_t size;
} Token;

/* ========================================================================== */

Lexer *lexer_new(char *content, size_t size, size_t position);
void lexer_del(Lexer *lexer);
Token lexer_next(Lexer *lexer);
Token lexer_eof_token(Lexer *lexer);
Token lexer_error(Lexer *lexer);
void lexer_trace_token(Lexer *lexer, Token *token);
Token lexer_invalid_token(Lexer *lexer);
Token lexer_chop_char(Lexer *lexer, size_t count);

int lexer_check_is_number(Lexer *lexer, Token *token);
int lexer_check_is_preproc(Lexer *lexer, Token *token);
int lexer_check_is_str(Lexer *lexer, Token *token);
int lexer_check_is_comment(Lexer *lexer, Token *token, int is_multi);

void lexer_trim_left(Lexer *lexer);
int lexer_next_char_is(Lexer *lexer, char c);
int lexer_char_is(Lexer *lexer, char c);
int lexer_is_keyword(Lexer *lexer, size_t length);
int lexer_is_punctuator(Lexer *lexer, size_t length, size_t max);
int lexer_check_punctuator_lookahead(Lexer *lexer);
int lexer_check_boundery(Lexer *lexer);
int lexer_check_boundery_next(Lexer *lexer);

/* ========================================================================== */
int is_escape_seq(char c);
int is_sybol_alnum_and_(char c);
int is_sybol_alpha_and_(char c);

/* ========================================================================== */

#ifndef EXLEXDEF
#define EXLEXDEF static inline
#endif // EXLEXDEF

EXLEXDEF void lexer_keyword_set_token(Lexer *l, Token *t, size_t len);
EXLEXDEF void lexer_punctuator_set_token(Lexer *l, Token *t, size_t len);

#endif // LEXER_H_
