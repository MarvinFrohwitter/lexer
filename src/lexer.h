// A C-Programming lexer
// Copyright (c) 2023 Marvin Frohwitter. All Rights Reserved.

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
  char *elements;
  size_t count;
  size_t capacity;
} Lexer_Scratch_Buffer; // A representation of a file content.

typedef struct Lexer {
  char *content;
  unsigned long long int content_length;
  unsigned long long int position;
  unsigned long long int next_start_position;

  unsigned long long int line_count;
  char *file_name;

  Lexer_Scratch_Buffer buffer;
} Lexer;

/* The variable that defines the escape characters. */
static const char ESCAPE[] = {'\n', '\r', '\t', '\v', '\f', '\\'};

/* The variable PUNCTUATORS is an array of single tokens of type PUNCTUATOR. */
/* It contains the tokens to match on. */
static const char *PUNCTUATORS[] = {
    // NOTE: It is important that the '.' is at the end of the array! It is
    // checked by the number function for floating point calculations.
    // And by the lexer_is_punctuator() function.
    ",",  "]", ")", "=", ";", "{", "}", "&", "*", "+", "-", "~", "|", "/",
    "\\", "%", "<", ">", "^", "|", "?", ":", "(", "[", "!", "'", ".", NULL};

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
  PUNCT_LANGLE = '<',    // 60
  PUNCT_ASSIGN = '=',    // 61
  PUNCT_RANGLE = '>',    // 62
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
        }                                                                      \
      }                                                                        \
      memcpy((dynamic_array)->elements + (dynamic_array)->count, new_elements, \
             sizeof(*(dynamic_array)->elements) * new_elements_count);         \
      (dynamic_array)->count = (dynamic_array)->count + new_elements_count;    \
    }                                                                          \
  } while (0)

/* ==========================================================================
 */
/* ============================ PUBLIC FUNCTIONS ============================
 */
/* ==========================================================================
 */

BASICLEXDEF Lexer *lexer_new(char *file_path, char *content, size_t size,
                             size_t position);
BASICLEXDEF void lexer_del(Lexer *lexer);
BASICLEXDEF Token lexer_next(Lexer *lexer);
const char *lexer_token_to_cstr(Lexer *lexer, Token *token);

/* ==========================================================================
 */
/* ============================ PRIVAT FUNCTIONS ============================
 */
/* ==========================================================================
 */

LEXDEF Token lexer_eof_token(void);
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
LEXDEF int lexer_is_punctuator(Lexer *lexer, size_t max);
LEXDEF int lexer_check_punctuator_lookahead(Lexer *lexer);
LEXDEF int lexer_check_boundery(Lexer *lexer);
LEXDEF int lexer_check_boundery_next(Lexer *lexer);

LEXDEF int lexer_keyword_set_token(Lexer *lexer, Token *token, size_t length);
LEXDEF int lexer_punctuator_set_token(Lexer *lexer, Token *token,
                                      size_t length);

/* ==========================================================================
 */
LEXDEF int is_escape_seq(char c);
LEXDEF int is_sybol_alnum_and_(char c);
LEXDEF int is_sybol_alpha_and_(char c);
/* ==========================================================================
 */

#endif // LEXER_H_
