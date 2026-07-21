// A C-Programming lexer
// Copyright (c) 2023-2026 Marvin Frohwitter. All Rights Reserved.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef LEXER_H_
#define LEXER_H_
#include <stddef.h>

#ifndef BASICLEXDEF
#define BASICLEXDEF
#endif // BASICLEXDEF

#ifndef LEX_LOGERROR
// #define LEX_LOGERROR
#endif // LEX_LOGERROR

#ifndef LEXDEF
#define LEXDEF static inline
#endif // LEXDEF

typedef struct {
  unsigned char *elements;
  size_t count;
  size_t capacity;
} Lexer_Scratch_Buffer; // A representation of a file content.

typedef struct Lexer {
  unsigned char *content;
  unsigned long long int content_length;
  unsigned long long int position;
  unsigned long long int next_start_position;

  int isstrlit;
  unsigned long long int line_count;
  unsigned char *line_start;
  unsigned long long int column_count;
  const char *file_name;

  Lexer_Scratch_Buffer buffer;
} Lexer;

/* The variable that defines the escape characters. */
static const char ESCAPE[] = {'\n', '\r', '\t', '\v', '\f', '\\'};

/* The variable PUNCTUATORS is an array of single tokens of type PUNCTUATOR. */
/* It contains the tokens to match on. */
static const char PUNCTUATORS[] = {
    ',',  ']', ')', '=', ';', '{', '}', '&', '*', '+', '-', '~',  '|', '/',
    '\\', '%', '<', '>', '^', '|', '?', ':', '(', '[', '!', '\'', '.'};

#define KINDS_LIST                                                             \
  X_KIND(INVALID, -1)                                                          \
  X_KIND(NULL_TERMINATOR, 0)                                                   \
  X_KIND(ERROR, 1)                                                             \
  X_KIND(NUMBER, 2)                                                            \
  X_KIND(KEYWORD, 3)                                                           \
  X_KIND(STRINGLITERAL, 4)                                                     \
  X_KIND(IDENTIFIER, 5)                                                        \
  X_KIND(PUNCTUATOR, 6)                                                        \
  X_KIND(CONSTANT, 7)                                                          \
  X_KIND(PREPROCESSING, 8)                                                     \
  X_KIND(COMMENT, 9)                                                           \
  X_KIND(EOF_TOKEN, 10)                                                        \
  X_KIND(ASCII_LOW, 11)                                                        \
  X_KIND(ANSI_HIGH, 12)                                                        \
                                                                               \
  X_KIND(ASCII_DOLLAR, '$', /*      36*/)                                      \
  X_KIND(ASCII_AT, '@', /*          64*/)                                      \
  X_KIND(ASCII_BACKTICK, '`', /*    96*/)                                      \
                                                                               \
  X_KIND(PUNCT_SINGLEQUOTE, 39, /* "'"*/)                                      \
  X_KIND(PUNCT_DOUBLEQUOTE, 34, /* '"'*/)                                      \
  X_KIND(PUNCT_BACKSLASH, '\\', /*  92*/)                                      \
                                                                               \
  /* ALL PUNCTUATORS */                                                        \
                                                                               \
  X_KIND(PUNCT_NOT, '!', /*      33*/)                                         \
  X_KIND(PUNCT_HTAG, '#', /*     35*/)                                         \
  X_KIND(PUNCT_MOD, '%', /*      37*/)                                         \
  X_KIND(PUNCT_BAND, '&', /*     38*/)                                         \
  X_KIND(PUNCT_MUL, '*', /*      42*/)                                         \
  X_KIND(PUNCT_ADD, '+', /*      43*/)                                         \
  X_KIND(PUNCT_COMMA, ',', /*     44*/)                                        \
  X_KIND(PUNCT_SUB, '-', /*      45*/)                                         \
  X_KIND(PUNCT_DIV, '/', /*      47*/)                                         \
  X_KIND(PUNCT_LPAREN, '(', /*   50*/)                                         \
  X_KIND(PUNCT_RPAREN, ')', /*   51*/)                                         \
  X_KIND(PUNCT_POINT, '.', /*    56*/)                                         \
  X_KIND(PUNCT_COLON, ':', /*    58*/)                                         \
  X_KIND(PUNCT_SEMICOLON, ';', /* 59*/)                                        \
  X_KIND(PUNCT_LANGLE, '<', /*   60*/)                                         \
  X_KIND(PUNCT_ASSIGN, '=', /*   61*/)                                         \
  X_KIND(PUNCT_RANGLE, '>', /*   62*/)                                         \
  X_KIND(PUNCT_ASK, '?', /*      63*/)                                         \
  X_KIND(PUNCT_LBRACKET, '[', /* 91*/)                                         \
  X_KIND(PUNCT_RBRACKET, ']', /* 93*/)                                         \
  X_KIND(PUNCT_BXOR, '^', /*     94*/)                                         \
  X_KIND(PUNCT_LBRACE, '{', /*   123*/)                                        \
  X_KIND(PUNCT_BOR, '|', /*      124*/)                                        \
  X_KIND(PUNCT_RBRACE, '}', /*   125*/)                                        \
  X_KIND(PUNCT_BNOT, '~', /*     126*/)                                        \
  X_KIND(ASCII_DEL, 127, /*      127*/)                                        \
                                                                               \
  /* LANGECOLON = LBRACKET;    // '<: = ['        */                           \
  /* COLONRANGE = RBRACKET;    // ':> = ]'        */                           \
  /* LANGELMOD = LBRACE;       // '<% = {'        */                           \
  /* KANGELMOD = RBRACE;       // '%> = }'        */                           \
  /* MODCOLON = HTAG;          // '%: = #'        */                           \
  /* MODCOLONMODCOLON = HHTAG; // '%:%: = ##'     */                           \
                                                                               \
  X_KIND(PUNCT_EQUAL, 257, /*      '=='*/)                                     \
  X_KIND(PUNCT_MULEQ, 258, /*      '*='*/)                                     \
  X_KIND(PUNCT_DIVEQ, 259, /*      '/='*/)                                     \
  X_KIND(PUNCT_MODEQ, 260, /*      '%='*/)                                     \
  X_KIND(PUNCT_ADDEQ, 261, /*      '+='*/)                                     \
  X_KIND(PUNCT_SUBEQ, 262, /*      '-='*/)                                     \
  X_KIND(PUNCT_LEFTSHIFTEQ, 263, /*'<<='*/)                                    \
  X_KIND(PUNCT_RIGHTSHIFTEQ, 264, /*'>>='*/)                                   \
  X_KIND(PUNCT_BANDEQ, 265, /*     '&='*/)                                     \
  X_KIND(PUNCT_BOREQ, 266, /*      '|='*/)                                     \
  X_KIND(PUNCT_BXOREQ, 267, /*     '^='*/)                                     \
  X_KIND(PUNCT_NOTEQ, 268, /*      '!='*/)                                     \
  X_KIND(PUNCT_LOREQ, 269, /*      '<='*/)                                     \
  X_KIND(PUNCT_GOREQ, 270, /*      '>='*/)                                     \
  X_KIND(PUNCT_LAND, 271, /*       '&&'*/)                                     \
  X_KIND(PUNCT_LOR, 272, /*        '||'*/)                                     \
  X_KIND(PUNCT_LEFTSHIFT, 273, /*  '<<'*/)                                     \
  X_KIND(PUNCT_RIGHTSHIFT, 274, /* '>>'*/)                                     \
  X_KIND(PUNCT_PDEREF, 275, /*     '->'*/)                                     \
  X_KIND(PUNCT_DECREMENT, 276, /*  '--'*/)                                     \
  X_KIND(PUNCT_INCREMENT, 277, /*  '++'*/)                                     \
  X_KIND(PUNCT_HHTAG, 278, /*      '##'*/)                                     \
  X_KIND(PUNCT_VARIADIC, 279, /*   '...'*/)                                    \
                                                                               \
  /* ALL KEYWORDS */                                                           \
  X_KIND(KEYWORD_AUTO, 280)                                                    \
                                                                               \
  X_KIND(KEYWORD_BREAK, 281)                                                   \
  X_KIND(KEYWORD_CASE, 282)                                                    \
  X_KIND(KEYWORD_CHAR, 283)                                                    \
  X_KIND(KEYWORD_CONST, 284)                                                   \
  X_KIND(KEYWORD_CONTINUE, 285)                                                \
  X_KIND(KEYWORD_DEFAULT, 286)                                                 \
  X_KIND(KEYWORD_DO, 287)                                                      \
  X_KIND(KEYWORD_DOUBLE, 288)                                                  \
  X_KIND(KEYWORD_ELSE, 289)                                                    \
  X_KIND(KEYWORD_ENUM, 290)                                                    \
  X_KIND(KEYWORD_EXTERN, 291)                                                  \
  X_KIND(KEYWORD_FLOAT, 292)                                                   \
  X_KIND(KEYWORD_FOR, 293)                                                     \
  X_KIND(KEYWORD_GOTO, 294)                                                    \
  X_KIND(KEYWORD_IF, 295)                                                      \
  X_KIND(KEYWORD_INT, 296)                                                     \
  X_KIND(KEYWORD_LONG, 297)                                                    \
  X_KIND(KEYWORD_REGISTER, 298)                                                \
  X_KIND(KEYWORD_RETURN, 299)                                                  \
  X_KIND(KEYWORD_SHORT, 300)                                                   \
  X_KIND(KEYWORD_SIGNED, 301)                                                  \
  X_KIND(KEYWORD_SIZEOF, 302)                                                  \
  X_KIND(KEYWORD_STATIC, 303)                                                  \
  X_KIND(KEYWORD_STRUCT, 304)                                                  \
  X_KIND(KEYWORD_SWITCH, 305)                                                  \
  X_KIND(KEYWORD_TYPEDEF, 306)                                                 \
  X_KIND(KEYWORD_UNION, 307)                                                   \
  X_KIND(KEYWORD_UNSIGNED, 308)                                                \
  X_KIND(KEYWORD_VOID, 309)                                                    \
  X_KIND(KEYWORD_VOLATILE, 310)                                                \
  X_KIND(KEYWORD_WHILE, 311)                                                   \
  X_KIND(KEYWORD_SIZE_T, 312)

/* The Token Kind that can be detected by the lexer. */
typedef enum Kind {
#define X_KIND(name, value, ...) name = value,
  KINDS_LIST
#undef X_KIND
} Kind;

typedef struct Token {
  Kind kind;
  const char *content;
  size_t size;
} Token;

/** @brief The initial capacity of the dynamic arrays. */
#define LEXER_DAP_CAP 64

/**
 * @brief The macro pushes the new array element at the end of the dynamic
 * array.
 *
 * @param dynamic_array The given array by a pointer.
 * @param element The given new element by value of the same type the array
 * holds elements.
 */
#define lexer_dap(dynamic_array, element)                                      \
  do {                                                                         \
    if ((dynamic_array)->capacity <= (dynamic_array)->count) {                 \
      if ((dynamic_array)->capacity == 0)                                      \
        (dynamic_array)->capacity = LEXER_DAP_CAP;                             \
      else                                                                     \
        (dynamic_array)->capacity = (dynamic_array)->capacity * 2;             \
                                                                               \
      (dynamic_array)->elements = realloc((dynamic_array)->elements,           \
                                          sizeof(*(dynamic_array)->elements) * \
                                              (dynamic_array)->capacity);      \
                                                                               \
      if ((dynamic_array)->elements == NULL) {                                 \
        fprintf(                                                               \
            stderr,                                                            \
            "The allocation for the dynamic array has failed in: %s: %d\n",    \
            __FILE__, __LINE__);                                               \
        abort();                                                               \
      }                                                                        \
    }                                                                          \
                                                                               \
    (dynamic_array)->elements[(dynamic_array)->count] = (element);             \
    (dynamic_array)->count = (dynamic_array)->count + 1;                       \
  } while (0)

/**
 * @brief The macro pushes the given amount of new elements to the end of the
 * dynamic array.
 *
 * @param dynamic_array The given array by a pointer.
 * @param new_element The given new elements by pointer of the same type the
 * array holds elements.
 * @param new_elements_count The amount of elements to add to the array.
 */
#define lexer_dapc(dynamic_array, new_elements, new_elements_count)            \
  do {                                                                         \
    if (new_elements != NULL) {                                                \
      if ((dynamic_array)->capacity <                                          \
          (dynamic_array)->count + new_elements_count) {                       \
        if ((dynamic_array)->capacity == 0) {                                  \
          (dynamic_array)->capacity = LEXER_DAP_CAP;                           \
        }                                                                      \
        while ((dynamic_array)->capacity <                                     \
               (dynamic_array)->count + new_elements_count) {                  \
          (dynamic_array)->capacity = (dynamic_array)->capacity * 2;           \
        }                                                                      \
        (dynamic_array)->elements = realloc(                                   \
            (dynamic_array)->elements,                                         \
            sizeof(*(dynamic_array)->elements) * (dynamic_array)->capacity);   \
        if ((dynamic_array)->elements == NULL) {                               \
          fprintf(                                                             \
              stderr,                                                          \
              "The allocation for the dynamic array has failed in: %s: %d\n",  \
              __FILE__, __LINE__);                                             \
          abort();                                                             \
        }                                                                      \
      }                                                                        \
      memcpy((dynamic_array)->elements + (dynamic_array)->count,               \
             (new_elements),                                                   \
             sizeof(*(dynamic_array)->elements) * (new_elements_count));       \
      (dynamic_array)->count = (dynamic_array)->count + (new_elements_count);  \
    }                                                                          \
  } while (0)

/* ========================================================================= */
/* ============================ PUBLIC FUNCTIONS =========================== */
/* ========================================================================= */

BASICLEXDEF Lexer *lexer_new(const char *file_path, char *content, size_t size,
                             size_t position);
BASICLEXDEF void lexer_del(Lexer *lexer);
BASICLEXDEF Token lexer_next(Lexer *lexer);
const char *lexer_token_to_cstr(Lexer *lexer, Token *token);

/* ========================================================================= */
/* ============================ PRIVAT FUNCTIONS =========================== */
/* ========================================================================= */

LEXDEF Token lexer_eof_token(Lexer *lexer);
LEXDEF Token lexer_error(Lexer *lexer);
LEXDEF void lexer_trace_token(Lexer *lexer, Token *token);
LEXDEF Token lexer_invalid_token(Lexer *lexer);
LEXDEF Token lexer_chop_char(Lexer *lexer, size_t count);

LEXDEF int lexer_check_is_number(Lexer *lexer, Token *token);
LEXDEF int lexer_check_is_preproc(Lexer *lexer, Token *token);
LEXDEF int lexer_check_is_str(Lexer *lexer, Token *token);
LEXDEF int lexer_check_is_comment(Lexer *lexer, Token *token, int is_multi);

LEXDEF void lexer_trim_left(Lexer *lexer);
LEXDEF int lexer_next_char_is(Lexer *lexer, char c);
LEXDEF int lexer_char_is(Lexer *lexer, char c);
LEXDEF int lexer_is_escape_seq_or_space(Lexer *lexer);
LEXDEF int lexer_is_punctuator_exclude(Lexer *lexer, char exclude);
LEXDEF int lexer_is_punctuator(Lexer *lexer);
LEXDEF int lexer_check_punctuator_lookahead(Lexer *lexer);
LEXDEF int lexer_check_boundary(Lexer *lexer);
LEXDEF int lexer_check_boundary_next(Lexer *lexer);

LEXDEF int lexer_keyword_set_token(Lexer *lexer, Token *token, size_t length);
LEXDEF int lexer_punctuator_set_token(Lexer *lexer, Token *token,
                                      size_t length);
LEXDEF const char *lexer_kind_to_str(Kind kind);

/* ========================================================================= */
LEXDEF int is_escape_seq(char c);
LEXDEF int is_sybol_alnum_and_(char c);
LEXDEF int is_sybol_alpha_and_(char c);
/* ========================================================================= */

#endif // LEXER_H_
