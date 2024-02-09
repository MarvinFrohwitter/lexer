#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define EXLEX_IMPLEMENTATION
#include "lexer.h"

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

/* The function lexer_new() creates the new state of a lexer. */
/* @param content The content that the lexer has to tokenise. */
/* @param size The content length. */
/* @param The position of the content the lexer should begin tokenise. */
/* @return Lexer The function returns a new initilized lexer. */
Lexer *lexer_new(char *content, size_t size, size_t position) {
  Lexer *lexer = malloc(sizeof(Lexer));
  if (lexer == NULL) {
    fprintf(stderr,
            "ERROR: No more memory for the content to lex can be allocated.\n");
    return NULL;
  }

  lexer->content_lenght = size;
  lexer->content = content;
  lexer->position = position;

  return lexer;
}

/* The function lexer_del() frees the allocated lexer. */
/* @param lexer The given Lexer that contains the current state. */
void lexer_del(Lexer *lexer) { free(lexer); }

/* The function lexer_chop_char() consumes the amount of chars given by the */
/* count. So it cuts of the content that the lexer tokenise form the left. */
/* @param lexer The given Lexer that contains the current state. */
/* @param count The amount of chars that will be choped from the left of the */
/* content. */
/* @return Token the Error token or the Invalid token. */
Token lexer_chop_char(Lexer *lexer, size_t count) {
  for (size_t i = 0; i < count; i++) {
    if (!lexer_check_boundery(lexer)) {
      return lexer_error(lexer);
    }
    lexer->position = lexer->position + 1;
  }

  return lexer_invalid_token(lexer);
}

/* The function lexer_invalid_token() sets the lexer values to the type of an */
/* invalid token. */
/* @param lexer The given Lexer that contains the current state. */
/* @return Token that has values of kind=INVALID, size=1 and content= current */
/* lexer position. */
Token lexer_invalid_token(Lexer *lexer) {
  Token token;
  token.kind = INVALID;
  token.size = 1;
  token.content = &lexer->content[lexer->position];
  return token;
}

/* The function check_boundery() checks for the length of the lexer content, if
 */
/* the current lexer position is in the content length. */
/* @param lexer The given Lexer that contains the current state. */
/* @return boolean True, if the lexer position is in the content length, */
/* otherwise false. */
int lexer_check_boundery(Lexer *lexer) {
  if (lexer->position < lexer->content_lenght) {
    return 1;
  }
  return 0;
}

/* The function lexer_next_char_is() returns, if the next character in the */
/* content is  equal to the given char c. */
/* @param lexer The given Lexer that contains the current state. */
/* @param c The character that will be compared with the lexer position. */
/* @return boolean True, if the given char c is identical to the next char in */
/* the content that lexer tokenise. */
int lexer_next_char_is(Lexer *lexer, char c) {

  if (c == lexer->content[lexer->position + 1]) {

    return 1;
  }
  return 0;
}

/* The function lexer_char_is() returns, if the character in the content is */
/* equal to the given char c. */
/* @param lexer The given Lexer that contains the current state. */
/* @param c The character that will be compared with the lexer position. */
/* @return boolean True, if the given char c is identical to the current char in
 */
/* the content that lexer tokenise. */
int lexer_char_is(Lexer *lexer, char c) {
  if (c == lexer->content[lexer->position]) {
    return 1;
  }
  return 0;
}

/* The function lexer_trim_left() consumes the characters from the left side of
 */
/* the given content. If the content matches isspace(). */
/* White space Characters: Blank space, newline, horizontal tab, carriage */
/* return and form feed. */
/* @param lexer The given Lexer that contains the current state. */
void lexer_trim_left(Lexer *lexer) {
  while (lexer_check_boundery(lexer) &&
         isspace(lexer->content[lexer->position])) {
    lexer_chop_char(lexer, 1);
  }
}

/* The function lexer_is_keyword() returns, if the lexer has found a keyword of
 */
/* the given length at the current lexer content position. */
/* @param lexer The given Lexer that contains the current state. */
/* @param length The character length the keyword should have. */
/* @return boolean True, if the content at the current position has a keyword,
 */
/* otherwise false. */
int lexer_is_keyword(Lexer *lexer, size_t length) {
  size_t array_count = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]) - 1;

  for (size_t i = 0; i < array_count; i++) {
    /* The position of the lexer is on the next char. */
    if (strncmp(KEYWORDS[i], &lexer->content[lexer->position - length],
                length) == 0) {
      return 1;
    }
  }
  return 0;
}

/* The function lexer_is_punctuator() returns, if the lexer has found a */
/* punctuator of the given length at the current lexer content position. */
/* @param lexer The given Lexer that contains the current state. */
/* @param length The character length the punctuator should have. */
/* @param max The maximum length a punctuator is searched by in the list. 0 if
 * the complete */
/* default list of punctuators should be searched thru. */
/* @return boolean True, if the content at the current position has a punctuator
 */
/* of given length, otherwise false. */
int lexer_is_punctuator(Lexer *lexer, size_t length, size_t max) {
  if (max == 0) {
    max = sizeof(PUNCTUATORS) / sizeof(PUNCTUATORS[0]) - 1;
  }

  /* The position of the lexer is on the next char. */
  for (size_t i = 0; i < max; i++) {
    if (strncmp(PUNCTUATORS[i], &lexer->content[lexer->position + length - 1],
                length) == 0) {
      return 1;
    }
  }
  return 0;
}

/* The function lexer_check_punctuator_lookahead() computes, if the next */
/* character can also be part of the punctuator. */
/* @param lexer The given Lexer that contains the current state. */
/* @return boolean True, if the next character is also part of the punctuator,
 */
/* otherwise false. */
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

/* The function lexer_check_is_number() computes if the given part to 'lex' is a
 * number. It then computes the corresponding token or calls the lexer error
 * token handler method. */
/* @param lexer The given Lexer that contains the current state. */
/* @param token The token that will be modified and contains the final token
 * that can also contain the error token that is passed back up in the call
 * stack. */
/* @return int The error code of the corresponding action result. It returns one
 * if the function computes a number in the token or 0 if the token is the error
 * token. */
int lexer_check_is_number(Lexer *lexer, Token *token) {
  int is_escape = 0;
  int ispostfix = 0;
  size_t length = sizeof(ESCAPE) / sizeof(ESCAPE[0]);
  token->kind = NUMBER;
  size_t pos = lexer->position;

  if (lexer_char_is(lexer, '0') &&
      (lexer_next_char_is(lexer, 'x') || lexer_next_char_is(lexer, 'X'))) {
    lexer_chop_char(lexer, 2);

    for (size_t j = 0; j < length; j++) {
      if (lexer_char_is(lexer, ESCAPE[j])) {
        is_escape = 1;
        break;
      }
    }

    if (is_escape || lexer_char_is(lexer, ' ') ||
        !lexer_check_boundery(lexer)) {
      goto errortoken;
    }

    while (isxdigit(lexer->content[lexer->position]) &&
           lexer_check_boundery(lexer)) {
      lexer_chop_char(lexer, 1);
    }

    if (!lexer_check_boundery(lexer)) {
      goto compute_return;
    }

    // if (!lexer_char_is(lexer, ' ') &&
    // !is_escape_seq(lexer->content[lexer->position]) &&
    //     !lexer_is_punctuator(lexer, 1, ARRAY_LENGTH(PUNCTUATORS) - 2)) {
    //   goto errortoken;
    // }

  } else {
    while (lexer_check_boundery(lexer) &&
           (isdigit(lexer->content[lexer->position]) ||
            lexer->content[lexer->position] == '.') &&
           !is_escape_seq(lexer->content[lexer->position])) {
      lexer_chop_char(lexer, 1);
    }

    if (!lexer_check_boundery(lexer)) {
      goto compute_return;
    }

    if (lexer_char_is(lexer, 'e') &&
        (lexer_next_char_is(lexer, '-') || lexer_next_char_is(lexer, '+')) &&
        isdigit(lexer->content[lexer->position + 2])) {

      lexer_chop_char(lexer, 2);
      while (isdigit(lexer->content[lexer->position]) &&
             lexer_check_boundery(lexer)) {

        lexer_chop_char(lexer, 1);
      }

      if (!lexer_check_boundery(lexer)) {
        goto compute_return;
      }
      goto postfixcheck;
    }
  }

postfixcheck: {
  // The 4th iteration handles the next char so, if the next char is not a
  // space the default catches the error.
  size_t maxiter = 4;
  for (size_t i = 0; i < maxiter; i++) {
    switch (lexer->content[lexer->position]) {
    case 'L':
    case 'l':
    case 'U':
    case 'u': {
      lexer_chop_char(lexer, 1);
      ispostfix = 1;
      break;
    }
    case ' ': {
      i = maxiter;
      break;
    }
    case '/':
      if (lexer_next_char_is(lexer, '/')) {
        i = maxiter;
      } else {
        goto errortoken;
      }

      break;

    default:
      if (lexer_check_boundery(lexer) &&
          (lexer_is_punctuator(lexer, 1, ARRAY_LENGTH(PUNCTUATORS) - 2) ||
           is_escape_seq(lexer->content[lexer->position]))) {
        i = maxiter;
        break;
      } else {
        fprintf(stderr, "postfixcheck faild with:%c at %zu\n",
                lexer->content[lexer->position], lexer->position);
        goto errortoken;
      }
    }
  }
}
compute_return:

  if (ispostfix) {
    token->size = lexer->position - 1 - pos;
  } else {
    token->size = lexer->position - pos;
  }
  return 1;

errortoken: {
  Token t = lexer_error(lexer);

  token->content = t.content;
  token->size = t.size;
  token->kind = t.kind;
  return 0;
}
}

/* The function lexer_check_is_preproc() computes the token for the
 * preprocessor. */
/* @param lexer The given Lexer that contains the current state. */
/* @param token The token that will be modified and contains the final token
 * that is passed up in the call stack. */
/* @return boolean 1, if the function check is passed, otherwise 0. */
int lexer_check_is_preproc(Lexer *lexer, Token *token) {

  size_t position = lexer->position;
  lexer_chop_char(lexer, 1);
  if (lexer->position >= lexer->content_lenght) {
    lexer->position = lexer->position - 1;
    return 0;
  }

  while (!lexer_char_is(lexer, '\n') && lexer_check_boundery(lexer)) {
    lexer_chop_char(lexer, 1);
  }

  token->size = lexer->position - position;

  return 1;
}

/* The function lexer_check_is_comment() computes the comment token */
/* @param lexer The given Lexer that contains the current state. */
/* @param token The token that will be modified and contains the final token
 * that is passed up in the call stack. */
/* @return boolean 1, if the function check is passed, otherwise 0. */
int lexer_check_is_comment(Lexer *lexer, Token *token, int is_multi) {

  size_t position = lexer->position;
  lexer_chop_char(lexer, 2);
  if (!lexer_check_boundery(lexer)) {
    lexer->position = lexer->position - 1;
    return 0;
  }

  while (lexer_check_boundery(lexer)) {
    if (is_multi) {
      if (lexer_char_is(lexer, '*') && lexer_next_char_is(lexer, '/')) {
        lexer_chop_char(lexer, 2);
        break;
      }
    } else {
      if (lexer_char_is(lexer, '\n'))
        break;
    }
    lexer_chop_char(lexer, 1);
  }

  token->size = lexer->position - position;
  return 1;
}

/* The function lexer_check_is_str() computes the possible sting token */
/* @param lexer The given Lexer that contains the current state. */
/* @param token The token that will be modified and contains the final token
 * that is passed up in the call stack. */
/* @return boolean 1, if the function check is passed, otherwise 0. */
int lexer_check_is_str(Lexer *lexer, Token *token) {

  if (lexer_char_is(lexer, '"') && lexer_check_boundery(lexer)) {
    if (!lexer_next_char_is(lexer, '"') && !lexer_check_boundery(lexer)) {
      return 0;
    }

    token->kind = STRINGLITERAL;
    size_t position = lexer->position;
    lexer_chop_char(lexer, 1);
    while (lexer_check_boundery(lexer)) {
      if (lexer_char_is(lexer, '"')) {
        lexer_chop_char(lexer, 1);
        break;
      } else if (is_escape_seq(lexer->content[lexer->position])) {
        lexer_chop_char(lexer, 1);
        if (!lexer_check_boundery(lexer)) {
          return 0;
        }
      } else {
        lexer_chop_char(lexer, 1);
      }
    }
    token->size = lexer->position - position;
  }
  return 1;
}

/* ========================================================================= */

/* The function is_escape_seq() returns, if the given char is equal to an escape
 */
/* character. */
/* @param c The char to compare. */
/* @return boolean True, if the given character is an escape char, otherwise */
/* false. */
int is_escape_seq(char c) {
  if (c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f' ||
      c == '\\') {
    return 1;
  }
  return 0;
}

/* The function is_sybol_alnum_and_() computes the, if the characters in the
 */
/* name of an identifier is part of the alnum group and the underscore. */
/* @param c The character to check for. */
/* @return boolean True, if the given character is part of the alnum group and
 */
/* the additional underscore. */
int is_sybol_alnum_and_(char c) { return isalnum(c) || c == '_'; }

/* The function is_sybol_alpha_and_() computes the, if the characters in the
 */
/* name of an identifier is part of the alpha group and the underscore. */
/* @param c The character to check for. */
/* @return boolean True, if the given character is part of the alpha group and
 */
/* the additional underscore. */
int is_sybol_alpha_and_(char c) { return isalpha(c) || c == '_'; }

/* =========================================================================
 */

/* The function lexer_next() computes the token by type, size and contents. */
/* Token Kind that can be detected are KEYWORD, NUMBER, IDENTIFIER, CONSTANT,
 */
/* STRINGLITERAL, PUNCTUATOR, INVALID */
/* @param lexer The given Lexer that contains the current state. */
/* @return Token The next found token in the given content */
Token lexer_next(Lexer *lexer) {
  Token token = {0};
  token.kind = INVALID;

  lexer_trim_left(lexer);
  if (lexer->position > lexer->content_lenght) {
    return lexer_eof_token(lexer);
  }

  token.content = &lexer->content[lexer->position];
  // Check for preprocessing
  if (lexer_char_is(lexer, '#') && lexer_check_boundery(lexer)) {
    token.kind = PREPROCESSING;

    if (!lexer_check_is_preproc(lexer, &token)) {
      return lexer_error(lexer);
    };
    return token;
  }

  // Check for Comments
  if (lexer_char_is(lexer, '/') && lexer_next_char_is(lexer, '/') &&
      lexer_check_boundery(lexer)) {

    if (!lexer_check_is_comment(lexer, &token, 0)) {
      return lexer_error(lexer);
    };
    token.kind = COMMENT;
    return token;
  }

  // Check for multi line Comments
  if (lexer_char_is(lexer, '/') && lexer_next_char_is(lexer, '*') &&
      lexer_check_boundery(lexer)) {

    if (!lexer_check_is_comment(lexer, &token, 1)) {
      return lexer_error(lexer);
    };
    token.kind = COMMENT;
    return token;
  }

  // TODO: support char strings like 'A' and '\0' maybe it is a number
  //       (design question)

  // Check for string literals
  if (lexer_char_is(lexer, '"') && lexer_check_boundery(lexer)) {
    if (!lexer_check_is_str(lexer, &token)) {
      return lexer_error(lexer);
    };
    return token;
  }

  // Check for numbers
  if (isdigit(lexer->content[lexer->position])) {

    if (!lexer_check_is_number(lexer, &token)) {
      fprintf(stderr,
              "ERROR: The lexer can not computet the token as part of a "
              "number!\n");
      fprintf(stderr,
              "--------------------------------------------------------\n");
    }

    return token;
  }

  // Check for keywords
  if (isalpha(lexer->content[lexer->position])) {

    size_t startpos = token.size = lexer->position;
    lexer_chop_char(lexer, 1);
    while (lexer_check_boundery(lexer) &&
           is_sybol_alpha_and_(lexer->content[lexer->position])) {
      lexer_chop_char(lexer, 1);
    }

    token.size = lexer->position - token.size;

    if (lexer_is_keyword(lexer, token.size)) {

      if (token.size < 2) {
        goto single_token;
      }
      token.kind = KEYWORD;
#ifdef EXLEX_IMPLEMENTATION
      lexer->position = lexer->position - token.size;
      lexer_keyword_set_token(lexer, &token, token.size);
      lexer_chop_char(lexer, token.size);
#endif /* EXLEX_IMPLEMENTATION */

      return token;
    } else {
    single_token:
      token.kind = INVALID;
      lexer->position = startpos;
    }
  }

  // Fall throw KEYWORDS to check for identifiers
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

  int valid_lookahead = 0;
  // Check for punctuators
  if (lexer_check_boundery(lexer)) {
    token.size = lexer->position;
    if (lexer_is_punctuator(lexer, 1, 0)) {
      token.kind = PUNCTUATOR;
      if (!lexer_check_punctuator_lookahead(lexer)) {
        token.size = 1;
        valid_lookahead = 1;
      }
      if (!valid_lookahead) {
        token.size = lexer->position - token.size;
      }

#ifdef EXLEX_IMPLEMENTATION
      lexer->position = lexer->position - token.size;
      lexer_punctuator_set_token(lexer, &token, token.size);
      lexer_chop_char(lexer, token.size);
#endif /* EXLEX_IMPLEMENTATION */

      return token;
    }
  }

  token.kind = INVALID;
  if (!lexer_check_boundery(lexer)) {
    return lexer_eof_token(lexer);
  } else {
    lexer_chop_char(lexer, 1);
  }
  token.size = 1;
  return token;
}

/* The  function creates the EOF_TOKEN */
/* @param lexer The Lexer that will be modified. */
/* @return token The token that will be modified and contains the EOF_TOKEN */
Token lexer_eof_token(Lexer *lexer) {
  Token token;
  token.kind = EOF_TOKEN;
  token.size = lexer->content_lenght;
  token.content = &lexer->content[lexer->content_lenght];
  return token;
}

/* The function lexer_trace_token() try's to detect the first part of the */
/* broken token */
/* @param lexer The Lexer that will be modified. */
/* @param token The token that will be modified and contains the final token*/
void lexer_trace_token(Lexer *lexer, Token *token) {
  size_t current_lexer_posion = lexer->position;

  while (lexer->position != 0 && lexer->content[lexer->position] != ' ' &&
         !is_escape_seq(lexer->content[lexer->position])) {
    lexer->position = lexer->position - 1;
  }

  if (lexer->position == 0) {
    token->size = current_lexer_posion - lexer->position + 1;
  } else {
    token->size = current_lexer_posion - lexer->position;
    lexer->position = lexer->position + 1;
  }
  token->content = &lexer->content[lexer->position];
  token->kind = ERROR;
}

/* The function lexer_error() handels the cases if a default token should be
 */
/* returned or an explicit error has to be handelt. As well as print */
/* the corresponding error message to standard error */
/* @param lexer The Lexer that will be modified. */
/* @return Token The ERROR token.  */
Token lexer_error(Lexer *lexer) {

  Token t;
  int checkend = 0;
  char *pstring, *pstring_tail = 0, *final_token;
  size_t current_lexer_posion = lexer->position;
  const char c = lexer->content[lexer->position];

  /* ----------------------------------------------------------------------- */
  /* ------------------------------ EOF TOKEN ------------------------------ */
  /* ----------------------------------------------------------------------- */

  if (!lexer_check_boundery(lexer)) {
    return lexer_eof_token(lexer);
  }

  /* ----------------------------------------------------------------------- */
  /* ------------------ IF POSSIBLE DETECT THE TOKEN. ---------------------- */
  /* ----------------------------------------------------------------------- */

  lexer_trace_token(lexer, &t);

  /* Allocate memory for the prefix string of the token. */
  pstring = malloc(sizeof(char) * t.size);
  assert(pstring != NULL && "Buy more RAM !!");
  pstring[0] = '\0';

  /* Add the chars of the first part from the lexer to the pstring string */
  /* manually. */
  for (size_t i = 0; i < t.size; i++) {
    pstring[i] = t.content[i];
  }
  pstring[t.size] = '\0';

  size_t preserve_lexer_posion = lexer->position;
  /* Compute the second part of the token after the fail point. */
  lexer->position = current_lexer_posion;

  if (lexer_check_boundery(lexer)) {
    lexer->position = lexer->position + 1;
  } else {
    // NOTE: That is very unlikely to happen. Only at the end of the content.

    checkend = 1;
    goto handle;
  }

  size_t k = 0;
  while (lexer_check_boundery(lexer)) {
    if (lexer_char_is(lexer, ' ') ||
        is_escape_seq(lexer->content[lexer->position])) {
      break;
    }
    lexer_chop_char(lexer, 1);
    k++;
  }

  /* Combine the to parts of the assumed token. */
  /* Allocate memory for the tail string of the token. */
  pstring_tail = malloc(sizeof(char) * k + 1);
  assert(pstring_tail != NULL && "Buy more RAM !!");
  pstring_tail[0] = '\0';

  /* Add the chars of the second part from the lexer to the tail string */
  /* manually. */
  for (size_t j = 0; j < k; j++) {
    pstring_tail[j] = lexer->content[current_lexer_posion + 1 + j];
  }
  pstring_tail[k] = '\0';

handle:
  /* Allocate memory for the final combined string of the token. */
  if (checkend) {
    final_token = calloc(1, (strlen(pstring) + 1 + 1) * sizeof(char));
  } else {
    final_token =
        calloc(1, (strlen(pstring) + strlen(pstring_tail) + 1) * sizeof(char));
  }
  assert(final_token != NULL && "Buy more RAM !!");
  final_token[0] = '\0';

  /* Add the to parts of the pstring and pstring_tail together to the */
  /* final_token string. */
  strcpy(final_token, pstring);

  if (!checkend) {
    strcat(final_token, pstring_tail);
  }

  /* Print the Error Message. */
  fprintf(stderr, "\n");
  fprintf(stderr, "================= ERROR: Broken Token! =================\n");
  fprintf(stderr, "ERROR: The first chars are:                     [%s]\n",
          pstring);
  fprintf(stderr, "ERROR: The assumed second part of the token is: [%s]\n",
          pstring_tail);
  fprintf(stderr, "ERROR: The assumed final broken token:          [%s]\n",
          final_token);
  fprintf(stderr, "Faild at CHAR: [%c]\n", c);
  fprintf(stderr, "         POS:  [%zu]\n", current_lexer_posion);
  fprintf(stderr, "--------------------------------------------------------\n");
  fprintf(stderr, "\n");

  /* Create the final Error Token to return. */
  Token token;
  token.kind = ERROR;
  token.size = strlen(final_token);
  token.content = &lexer->content[preserve_lexer_posion];

  /* Free the allocated memory for the individual parts of the token string. */

  free(pstring);
  free(pstring_tail);
  free(final_token);

  return token;
}

#ifdef EXLEX_IMPLEMENTATION

EXLEXDEF void lexer_keyword_set_token(Lexer *l, Token *t, size_t len) {

  switch (len) {
  case 2: {
    if (strncmp("do", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_DO;
    else if (strncmp("if", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_IF;
  } break;

  case 3: {
    if (strncmp("for", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_FOR;
    else if (strncmp("int", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_INT;
  } break;

  case 4: {
    if (strncmp("char", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_CHAR;
    else if (strncmp("void", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_VOID;
    else if (strncmp("else", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_ELSE;
    else if (strncmp("long", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_LONG;
    else if (strncmp("goto", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_GOTO;
    else if (strncmp("case", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_CASE;
    else if (strncmp("enum", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_ENUM;
    else if (strncmp("auto", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_AUTO;
  } break;

  case 5: {
    if (strncmp("while", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_WHILE;
    else if (strncmp("float", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_FLOAT;
    else if (strncmp("const", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_CONST;
    else if (strncmp("break", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_BREAK;
    else if (strncmp("short", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_SHORT;
    else if (strncmp("union", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_UNION;
  } break;

  case 6: {
    if (strncmp("return", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_RETURN;
    if (strncmp("size_t", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_SIZE_T;
    else if (strncmp("struct", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_STRUCT;
    else if (strncmp("switch", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_SWITCH;
    else if (strncmp("double", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_DOUBLE;
    else if (strncmp("sizeof", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_SIZEOF;
    else if (strncmp("static", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_STATIC;
    else if (strncmp("signed", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_SIGNED;
    else if (strncmp("extern", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_EXTERN;
  } break;

  case 7: {
    if (strncmp("default", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_DEFAULT;
    else if (strncmp("typedef", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_TYPEDEF;
  } break;

  case 8: {
    if (strncmp("continue", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_CONTINUE;
    else if (strncmp("register", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_REGISTER;
    else if (strncmp("unsigned", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_UNSIGNED;
    else if (strncmp("volatile", &l->content[l->position], len) == 0)
      t->kind = KEYWORD_VOLATILE;
  } break;

  default:
    assert(0 && "KEYWORD UNREACHABLE");
  }
}

EXLEXDEF void lexer_punctuator_set_token(Lexer *l, Token *t, size_t len) {

  switch (len) {
  case 1: {
    switch (l->content[l->position]) {
      // TODO: Think about singe quote
    case PUNCT_SINGLEQUOTE:
      t->kind = PUNCT_SINGLEQUOTE;
      break;
    case '^':
      t->kind = PUNCT_BXOR;
      break;
    case '|':
      t->kind = PUNCT_BOR;
      break;
    case '?':
      t->kind = PUNCT_ASK;
      break;
    case ':':
      t->kind = PUNCT_COLON;
      break;
    case ';':
      t->kind = PUNCT_SEMICOLON;
      break;
    case '[':
      t->kind = PUNCT_LBRACKET;
      break;
    case ']':
      t->kind = PUNCT_RBRACKET;
      break;
    case '(':
      t->kind = PUNCT_LPAREN;
      break;
    case ')':
      t->kind = PUNCT_RPAREN;
      break;
    case '{':
      t->kind = PUNCT_LBRACE;
      break;
    case '}':
      t->kind = PUNCT_RBRACE;
      break;
    case '.':
      t->kind = PUNCT_POINT;
      break;
    case '&':
      t->kind = PUNCT_BAND;
      break;
    case '*':
      t->kind = PUNCT_MUL;
      break;
    case '+':
      t->kind = PUNCT_ADD;
      break;
    case '-':
      t->kind = PUNCT_SUB;
      break;
    case '~':
      t->kind = PUNCT_BNOT;
      break;
    case '!':
      t->kind = PUNCT_NOT;
      break;
    case '/':
      t->kind = PUNCT_DIV;
      break;
    case '%':
      t->kind = PUNCT_MOD;
      break;
    case '<':
      t->kind = PUNCT_LANGE;
      break;
    case '>':
      t->kind = PUNCT_RANGE;
      break;
    case ',':
      t->kind = PUNCT_COMMA;
      break;
    case '#':
      t->kind = PUNCT_HTAG;
      break;
    case '=':
      t->kind = PUNCT_ASSIGN;
      break;
    default:
      assert(0 && "PUNCTUATOR ONE CHAR UNREACHABLE");
    }
  } break;
  case 2: {
    if (strncmp("->", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_PDEREF;
    else if (strncmp("++", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_INCREMENT;
    else if (strncmp("--", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_DECREMENT;
    else if (strncmp("<<", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_LEFTSHIFT;
    else if (strncmp(">>", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_RIGHTSHIFT;
    else if (strncmp("<=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_LOREQ;
    else if (strncmp(">=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_GOREQ;
    else if (strncmp("==", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_EQUAL;
    else if (strncmp("!=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_NOTEQ;
    else if (strncmp("&&", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_LAND;
    else if (strncmp("||", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_LOR;
    else if (strncmp("*=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_MULEQ;
    else if (strncmp("/=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_DIVEQ;
    else if (strncmp("%=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_MODEQ;
    else if (strncmp("+=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_ADDEQ;
    else if (strncmp("-=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_SUBEQ;
    else if (strncmp("&=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_BANDEQ;
    else if (strncmp("^=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_BXOREQ;
    else if (strncmp("|=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_BOREQ;
    else if (strncmp("##", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_HHTAG;
    else
      assert(0 && "PUNCTUATOR TOW CHAR UNREACHABLE");
  } break;
  case 3: {
    if (strncmp("...", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_VARIADIC;
    else if (strncmp("<<=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_LEFTSHIFTEQ;
    else if (strncmp(">>=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_RIGHTSHIFTEQ;
    else {
      assert(0 && "PUNCTUATOR THREE CHAR UNREACHABLE");
    }
  } break;
  default:
    assert(0 && "PUNCTUATOR UNREACHABLE");

    break;
  }
}

#endif // EXLEX_IMPLEMENTATION
