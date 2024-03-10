#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

/* The function lexer_new() creates the new state of a lexer. */
/* @param content The content that the lexer has to tokenise. */
/* @param size The content length. */
/* @param The position of the content the lexer should begin tokenise. */
/* @return Lexer The function returns a new initilized lexer. */
BASICLEXDEF Lexer *lexer_new(char *file_path, char *content, size_t size,
                             size_t position) {
  Lexer *lexer = malloc(sizeof(Lexer));
  if (lexer == NULL) {
#ifdef LEX_LOGERROR
    fprintf(stderr,
            "ERROR: No more memory for the content to lex can be allocated.\n");
#endif // LEX_LOGERROR
    return NULL;
  }

  lexer->content_length = size;
  lexer->content = content;
  lexer->position = position;
  lexer->next_start_position = lexer->position;
  lexer->file_name = file_path;
  lexer->line_count = 1;

  return lexer;
}

/* The function lexer_del() frees the allocated lexer. */
/* @param lexer The given Lexer that contains the current state. */
BASICLEXDEF void lexer_del(Lexer *lexer) {
  free(lexer->content);
  free(lexer);
}

/* The function lexer_chop_char() consumes the amount of chars given by the */
/* count. So it cuts of the content that the lexer tokenise form the left. */
/* @param lexer The given Lexer that contains the current state. */
/* @param count The amount of chars that will be choped from the left of the */
/* content. */
/* @return Token the Error token or the Invalid token. */
LEXDEF Token lexer_chop_char(Lexer *lexer, size_t count) {
  for (size_t i = 0; i < count; i++) {
    if (lexer->content[lexer->position] == '\n') {
      lexer->line_count++;
    }
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
LEXDEF Token lexer_invalid_token(Lexer *lexer) {
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
LEXDEF int lexer_check_boundery(Lexer *lexer) {
  if (lexer->position < lexer->content_length) {
    return 1;
  }
  return 0;
}

/* The function check_boundery_next() checks for the length of the lexer
 * content, if the current lexer position+1 is in the content length. */
/* @param lexer The given Lexer that contains the current state. */
/* @return boolean True, if the lexer position+1 is in the content length, */
/* otherwise false. */
LEXDEF int lexer_check_boundery_next(Lexer *lexer) {
  if (lexer->position + 1 < lexer->content_length) {
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
LEXDEF int lexer_next_char_is(Lexer *lexer, char c) {

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
LEXDEF int lexer_char_is(Lexer *lexer, char c) {
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
LEXDEF void lexer_trim_left(Lexer *lexer) {
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
LEXDEF int lexer_is_keyword(Lexer *lexer, size_t length) {
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
LEXDEF int lexer_is_punctuator(Lexer *lexer, size_t length, size_t max) {
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
/* otherwise false. On error -1.*/
LEXDEF int lexer_check_punctuator_lookahead(Lexer *lexer) {

  if (!lexer_check_boundery_next(lexer)) {
    return -1;
  }
  lexer_chop_char(lexer, 1);
  if (!lexer_check_boundery_next(lexer)) {
    lexer->position -= 1;
    return -1;
  }
  lexer->position -= 1;

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
 * if the function computes a number in the token or 0 if the token could
 * potentially be a punctuator. -1 if the token is the error token. */
LEXDEF int lexer_check_is_number(Lexer *lexer, Token *token) {
  int is_floating = 0;
  token->kind = NUMBER;
  size_t pos = lexer->position;

  if (lexer_char_is(lexer, '0') && lexer_check_boundery_next(lexer) &&
      (lexer_next_char_is(lexer, 'b') || lexer_next_char_is(lexer, 'B'))) {
    lexer_chop_char(lexer, 2);

    if (!lexer_check_boundery(lexer) ||
        !(lexer_char_is(lexer, '0') || lexer_char_is(lexer, '1'))) {
      goto errortoken;
    }

    while (lexer_check_boundery(lexer) &&
           (lexer_char_is(lexer, '0') || lexer_char_is(lexer, '1'))) {
      lexer_chop_char(lexer, 1);
    }

    if (!lexer_check_boundery(lexer)) {
      goto compute_return;
    }

  } else if (lexer_char_is(lexer, '0') && lexer_check_boundery_next(lexer) &&
             (lexer_next_char_is(lexer, 'x') ||
              lexer_next_char_is(lexer, 'X'))) {
    lexer_chop_char(lexer, 2);

    if (!lexer_check_boundery(lexer) ||
        !isxdigit(lexer->content[lexer->position])) {
      goto errortoken;
    }

    while (isxdigit(lexer->content[lexer->position]) &&
           lexer_check_boundery(lexer)) {
      lexer_chop_char(lexer, 1);
    }

    if (!lexer_check_boundery(lexer)) {
      goto compute_return;
    }

  } else {
    while (lexer_check_boundery(lexer) &&
           (isdigit(lexer->content[lexer->position]) ||
            lexer->content[lexer->position] == '.') &&
           !is_escape_seq(lexer, lexer->content[lexer->position])) {

      if (is_floating &&
          (lexer_char_is(lexer, ' ') || lexer_char_is(lexer, '.'))) {
        goto punctuator;
      }
      if (lexer_char_is(lexer, '.')) {
        is_floating += 1;
      }

      {
        Token token = lexer_chop_char(lexer, 1);
        if (token.kind == ERROR)
          goto errortoken;
      }

      char c = lexer->content[lexer->position];
      if (lexer->content[lexer->position - 1] == '.' &&
          (isalpha(c) || isspace(c) || lexer_char_is(lexer, '_') ||
           lexer_is_punctuator(lexer, 1, 0)) &&
          (!lexer_char_is(lexer, 'e') || !lexer_char_is(lexer, 'E'))) {
        goto punctuator;
      }

      if (lexer_char_is(lexer, '.')) {
        is_floating += 1;
        {
          Token token = lexer_chop_char(lexer, 1);
          if (token.kind == ERROR)
            goto errortoken;
        }

        // Just for ending floats with a '.' and after is a punctuator.
        size_t punct_pos = lexer->position;
        if (lexer->content[lexer->position - 1] == '.') {

          if (!lexer_check_boundery_next(lexer)) {
            continue;
          }

          while (lexer_check_boundery(lexer) &&
                 !lexer_is_escape_seq_or_space(lexer)) {
            Token token = lexer_chop_char(lexer, 1);
            if (token.kind == ERROR)
              break;
          }

          if (!lexer_check_boundery(lexer)) {
            if (punct_pos + 1 >= lexer->content_length) {
              goto errortoken;
            } else {
              size_t punct_length = lexer->position - punct_pos;
              assert(punct_length != 0);
              assert(punct_length == 0);
              // lexer->position = punct_pos;
              Token token = {0};
              if (lexer_punctuator_set_token(lexer, &token, punct_length))
                continue;
              else {
                goto errortoken;
              }
            }

          } else {
            lexer->position = punct_pos;
            continue;
          }
        }
        lexer->position = punct_pos;

        if (lexer->content[lexer->position - 2] == '.' &&
            lexer_char_is(lexer, '.')) {
          goto punctuator;

        } else if ((isdigit(lexer->content[lexer->position]) ||
                    (lexer_char_is(lexer, 'e') || lexer_char_is(lexer, 'E'))) &&
                   is_floating <= 1) {
          continue;
        } else if ((isdigit(lexer->content[lexer->position]) ||
                    (lexer_char_is(lexer, 'f') || lexer_char_is(lexer, 'F') ||
                     lexer_char_is(lexer, 'l') || lexer_char_is(lexer, 'L'))) &&
                   is_floating <= 1) {
          continue;
        }
        { goto errortoken; }
      }
    }

    if (!lexer_check_boundery(lexer)) {
      goto compute_return;
    }

    if (lexer_char_is(lexer, 'e') || lexer_char_is(lexer, 'E')) {
      {
        Token token = lexer_chop_char(lexer, 1);
        if (token.kind == ERROR)
          goto errortoken;
      }
      if (lexer_char_is(lexer, '-') || lexer_char_is(lexer, '+') ||
          isdigit(lexer->content[lexer->position])) {
        {
          Token token = lexer_chop_char(lexer, 1);
          if (token.kind == ERROR)
            goto errortoken;
        }
      }

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
  size_t maxiter = 4;
  if (is_floating) {
    switch (lexer->content[lexer->position]) {
    case 'L':
    case 'l':
    case 'F':
    case 'f': {
      lexer_chop_char(lexer, 1);
      goto compute_return;
    } break;
    case ' ':
      goto compute_return;
      break;
    default:
      if (lexer_check_boundery(lexer) &&
          (lexer_is_punctuator(lexer, 1, ARRAY_LENGTH(PUNCTUATORS) - 2) ||
           is_escape_seq(lexer, lexer->content[lexer->position]))) {
        goto compute_return;
        break;
      } else {
        goto errortoken;
      }
    }
    maxiter = 0;
  }

  // The 4th iteration handles the next char so, if the next char is not a
  // space the default catches the error.
  for (size_t i = 0; i < maxiter; i++) {
    switch (lexer->content[lexer->position]) {
    case 'L':
    case 'l':
    case 'U':
    case 'u': {
      lexer_chop_char(lexer, 1);
      break;
    }
    case 'F':
    case 'f': {
      lexer_chop_char(lexer, 1);
      i = maxiter;
      break;
    }
    case ' ': {
      i = maxiter;
      break;
    }
    default:
      if (lexer_check_boundery(lexer) &&
          (lexer_is_punctuator(lexer, 1, ARRAY_LENGTH(PUNCTUATORS) - 2) ||
           is_escape_seq(lexer, lexer->content[lexer->position]))) {
        i = maxiter;
        break;
      } else {
        goto errortoken;
      }
    }
  }
}

compute_return:
  token->size = lexer->position - pos;
  return 1;

punctuator: {
  // Is probably a punctuator.
  lexer->position = pos;
  return 0;
}

errortoken: {
  Token t = lexer_error(lexer);

  token->content = t.content;
  token->size = t.size;
  token->kind = t.kind;
  return -1;
}
}

/* The function lexer_check_is_preproc() computes the token for the
 * preprocessor. */
/* @param lexer The given Lexer that contains the current state. */
/* @param token The token that will be modified and contains the final token
 * that is passed up in the call stack. */
/* @return boolean 1, if the function check is passed, otherwise 0. */
LEXDEF int lexer_check_is_preproc(Lexer *lexer, Token *token) {

  size_t position = lexer->position;
  lexer_chop_char(lexer, 1);
  if (lexer->position >= lexer->content_length) {
    lexer->position = lexer->position - 1;
    return 0;
  }

  while (lexer_check_boundery(lexer) && !lexer_char_is(lexer, '\n')) {
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
LEXDEF int lexer_check_is_comment(Lexer *lexer, Token *token, int is_multi) {

  size_t position = lexer->position;
  {
    Token token = lexer_chop_char(lexer, 2);
    if (token.kind == ERROR) {
      return 0;
    }
  }

  while (lexer_check_boundery(lexer)) {
    if (is_multi) {
      if (lexer_char_is(lexer, '*') && lexer_next_char_is(lexer, '/') &&
          lexer_check_boundery_next(lexer)) {
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
LEXDEF int lexer_check_is_str(Lexer *lexer, Token *token) {

  if (!lexer_check_boundery_next(lexer)) {
    return 0;
  }

  token->kind = STRINGLITERAL;
  size_t position = lexer->position;
  lexer_chop_char(lexer, 1);
  while (lexer_check_boundery(lexer)) {
    if (lexer_char_is(lexer, '"')) {
      lexer_chop_char(lexer, 1);
      break;
    } else if (is_escape_seq(lexer, lexer->content[lexer->position])) {
      {
        Token token = lexer_chop_char(lexer, 1);
        if (token.kind == ERROR) {
          return 0;
        }
      }
    } else {
      lexer_chop_char(lexer, 1);
    }
  }
  token->size = lexer->position - position;
  return 1;
}

/* The function ilexer_is_escape_seq_or_space() returns, if the given char is
 * equal to an escape character or a space. */
/* @param lexer The given Lexer that contains the current state. */
/* @return boolean True, if the current lexer postion is an escape char or a
 * space, otherwise false. */
LEXDEF int lexer_is_escape_seq_or_space(Lexer *lexer) {
  return is_escape_seq(lexer, lexer->content[lexer->position]) ||
         lexer_char_is(lexer, ' ');
}

/* ========================================================================= */

/* The function is_escape_seq() returns, if the given char is equal to an
 * escape
 */
/* character. */
/* @param c The char to compare. */
/* @return boolean True, if the given character is an escape char, otherwise
 */
/* false. */
LEXDEF int is_escape_seq(Lexer *lexer, char c) {
  size_t length = sizeof(ESCAPE) / sizeof(ESCAPE[0]);
  for (size_t j = 0; j < length; j++) {
    if (c == ESCAPE[j]) {
      return 1;
    }
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
LEXDEF int is_sybol_alnum_and_(char c) { return isalnum(c) || c == '_'; }

/* The function is_sybol_alpha_and_() computes the, if the characters in the
 */
/* name of an identifier is part of the alpha group and the underscore. */
/* @param c The character to check for. */
/* @return boolean True, if the given character is part of the alpha group and
 */
/* the additional underscore. */
LEXDEF int is_sybol_alpha_and_(char c) { return isalpha(c) || c == '_'; }

/* =========================================================================
 */

/* The function lexer_next() computes the token by type, size and contents. */
/* Token Kind that can be detected are KEYWORD, NUMBER, IDENTIFIER, CONSTANT,
 */
/* STRINGLITERAL, PUNCTUATOR, INVALID */
/* @param lexer The given Lexer that contains the current state. */
/* @return Token The next found token in the given content */
BASICLEXDEF Token lexer_next(Lexer *lexer) {
  lexer->next_start_position = lexer->position;
  Token token = {0};
  token.kind = INVALID;

  lexer_trim_left(lexer);
  if (lexer->position > lexer->content_length) {
    return lexer_eof_token(lexer);
  }

  token.content = &lexer->content[lexer->position];
  // Check for preprocessing
  if (lexer_char_is(lexer, '#')) {
    token.kind = PREPROCESSING;

    if (!lexer_check_is_preproc(lexer, &token)) {
      return lexer_error(lexer);
    };
    return token;
  }

  // Check for Comments
  if (lexer_char_is(lexer, '/') && lexer_check_boundery_next(lexer) &&
      lexer_next_char_is(lexer, '/')) {

    if (!lexer_check_is_comment(lexer, &token, 0)) {
      return lexer_error(lexer);
    };
    token.kind = COMMENT;
    return token;
  }

  // Check for multi line Comments
  if (lexer_char_is(lexer, '/') && lexer_next_char_is(lexer, '*') &&
      lexer_check_boundery_next(lexer)) {

    if (!lexer_check_is_comment(lexer, &token, 1)) {
      return lexer_error(lexer);
    };
    token.kind = COMMENT;
    return token;
  }

  // Check for string literals
  if (lexer_char_is(lexer, '"')) {
    if (!lexer_check_is_str(lexer, &token)) {
      return lexer_error(lexer);
    };
    return token;
  }

  // Check for numbers
  if (isdigit(lexer->content[lexer->position]) || lexer_char_is(lexer, '.')) {
    if (lexer_check_is_number(lexer, &token)) {
      return token;
    }
    token.kind = INVALID;
  }

  // Check for keywords
  if (isalpha(lexer->content[lexer->position])) {

    size_t startpos = token.size = lexer->position;
    lexer_chop_char(lexer, 1);
    while (lexer_check_boundery(lexer) &&
           !lexer_is_escape_seq_or_space(lexer)) {

      lexer_chop_char(lexer, 1);
    }

    token.size = lexer->position - token.size;

    lexer->position = lexer->position - token.size;
    if (lexer_keyword_set_token(lexer, &token, token.size)) {

      lexer_chop_char(lexer, token.size);

      if (token.size < 2) {
        goto single_token;
      }
#ifndef EXLEX_IMPLEMENTATION
      token.kind = KEYWORD;
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

  // Check for punctuators
  if (lexer_check_boundery(lexer)) {
    token.size = lexer->position;
    if (lexer_is_punctuator(lexer, 1, 0)) {

#ifndef LEXLOOKAHEAD

      int lookahead = lexer_check_punctuator_lookahead(lexer);
      if (lookahead < 1) {
        token.size = 1;
      } else {
        token.size = lexer->position - token.size;
      }

      if (lookahead == -1) {
        lexer_punctuator_set_token(lexer, &token, token.size);
        lexer_chop_char(lexer, 1);
      } else {
        lexer->position = lexer->position - token.size;
        lexer_punctuator_set_token(lexer, &token, token.size);
        lexer_chop_char(lexer, token.size);
      }
#else
      token.size = 1;
      lexer_punctuator_set_token(lexer, &token, token.size);
      lexer_chop_char(lexer, 1);
#endif /* LEXLOOKAHEAD */
#ifndef EXLEX_IMPLEMENTATION
      token.kind = PUNCTUATOR;
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
LEXDEF Token lexer_eof_token(Lexer *lexer) {
  Token token;
  token.kind = EOF_TOKEN;
  token.size = 0;
  token.content = NULL;
  return token;
}

/* The function lexer_trace_token() try's to detect the first part of the */
/* broken token */
/* @param lexer The Lexer that will be modified. */
/* @param token The token that will be modified and contains the final token*/
LEXDEF void lexer_trace_token(Lexer *lexer, Token *token) {
  size_t current_lexer_posion = lexer->position;

  while (lexer->position >= 0 &&
         lexer->position >= lexer->next_start_position &&
         !lexer_is_escape_seq_or_space(lexer)) {
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
LEXDEF Token lexer_error(Lexer *lexer) {

  Token t;
  int checkend = 0;
  char *pstring, *pstring_tail = 0, *final_token;
  size_t current_lexer_posion = lexer->position;
  const char c = lexer->content[lexer->position];

  /* ------------------------------ EOF TOKEN ------------------------------
   */

  if (!lexer_check_boundery(lexer)) {
    return lexer_eof_token(lexer);
  }

  /* ------------------ IF POSSIBLE DETECT THE TOKEN. ----------------------
   */

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

  if (lexer_check_boundery_next(lexer)) {
    lexer->position = lexer->position + 1;
  } else {
    // That is very unlikely to happen. Only at the end of the content.
    checkend = 1;
    goto handle;
  }

  size_t k = 0;
  while (lexer_check_boundery(lexer)) {
    if (lexer_is_escape_seq_or_space(lexer))
      break;
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

#ifdef LEX_LOGERROR
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
  fprintf(stderr, "         LINE: [%llu]\n", lexer->line_count);
  fprintf(stderr, "--------------------------------------------------------\n");
  fprintf(stderr, "The current file name: %s\n", lexer->file_name);
  fprintf(stderr, "--------------------------------------------------------\n");
  fprintf(stderr, "\n");

#endif // LEX_LOGERROR
  /* Create the final Error Token to return. */
  Token token;
  token.kind = ERROR;
  token.size = strlen(final_token);
  token.content = &lexer->content[preserve_lexer_posion];

  /* Free the allocated memory for the individual parts of the token string.
   */

  free(pstring);
  free(pstring_tail);
  free(final_token);

  return token;
}

/**
 * @brief The function lexer_keyword_set_token() computes the token type for
 * the current lexer position for the keywords.
 *
 * @param lexer The given Lexer that contains the current state.
 * @param token The token the type is going to be set for.
 * @param length The length the expected keyword has.
 * @return boolean Returns true if a keyword was found, otherwise false.
 */
LEXDEF int lexer_keyword_set_token(Lexer *lexer, Token *token, size_t length) {

  switch (length) {
  case 2: {
    if (strncmp("do", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_DO;
    else if (strncmp("if", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_IF;
  } break;

  case 3: {
    if (strncmp("for", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_FOR;
    else if (strncmp("int", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_INT;
  } break;

  case 4: {
    if (strncmp("char", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_CHAR;
    else if (strncmp("void", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_VOID;
    else if (strncmp("else", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_ELSE;
    else if (strncmp("long", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_LONG;
    else if (strncmp("goto", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_GOTO;
    else if (strncmp("case", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_CASE;
    else if (strncmp("enum", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_ENUM;
    else if (strncmp("auto", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_AUTO;
  } break;

  case 5: {
    if (strncmp("while", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_WHILE;
    else if (strncmp("float", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_FLOAT;
    else if (strncmp("const", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_CONST;
    else if (strncmp("break", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_BREAK;
    else if (strncmp("short", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_SHORT;
    else if (strncmp("union", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_UNION;
  } break;

  case 6: {
    if (strncmp("return", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_RETURN;
    if (strncmp("size_t", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_SIZE_T;
    else if (strncmp("struct", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_STRUCT;
    else if (strncmp("switch", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_SWITCH;
    else if (strncmp("double", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_DOUBLE;
    else if (strncmp("sizeof", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_SIZEOF;
    else if (strncmp("static", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_STATIC;
    else if (strncmp("signed", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_SIGNED;
    else if (strncmp("extern", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_EXTERN;
  } break;

  case 7: {
    if (strncmp("default", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_DEFAULT;
    else if (strncmp("typedef", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_TYPEDEF;
  } break;

  case 8: {
    if (strncmp("continue", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_CONTINUE;
    else if (strncmp("register", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_REGISTER;
    else if (strncmp("unsigned", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_UNSIGNED;
    else if (strncmp("volatile", &lexer->content[lexer->position], length) == 0)
      token->kind = KEYWORD_VOLATILE;
  } break;

  default:
    return 0;
  }

  if (token->kind == INVALID) {
    return 0;
  }
  return 1;
}

/**
 * @brief The function lexer_punctuator_set_token() sets the corresponding
 * token type of the current lexer position for the punctuators.
 *
 * @param lexer The given Lexer that contains the current state.
 * @param token The token the type is going to be set for.
 * @param length The length the expected keyword has.
 * @return boolean Returns true if a punctuator was found, otherwise false.
 */
LEXDEF int lexer_punctuator_set_token(Lexer *lexer, Token *token,
                                      size_t length) {

  switch (length) {
  case 1: {
    switch (lexer->content[lexer->position]) {
    case PUNCT_SINGLEQUOTE:
      token->kind = PUNCT_SINGLEQUOTE;
      break;
    case '\\':
      token->kind = PUNCT_BACKSLASH;
      break;
    case '^':
      token->kind = PUNCT_BXOR;
      break;
    case '|':
      token->kind = PUNCT_BOR;
      break;
    case '?':
      token->kind = PUNCT_ASK;
      break;
    case ':':
      token->kind = PUNCT_COLON;
      break;
    case ';':
      token->kind = PUNCT_SEMICOLON;
      break;
    case '[':
      token->kind = PUNCT_LBRACKET;
      break;
    case ']':
      token->kind = PUNCT_RBRACKET;
      break;
    case '(':
      token->kind = PUNCT_LPAREN;
      break;
    case ')':
      token->kind = PUNCT_RPAREN;
      break;
    case '{':
      token->kind = PUNCT_LBRACE;
      break;
    case '}':
      token->kind = PUNCT_RBRACE;
      break;
    case '.':
      token->kind = PUNCT_POINT;
      break;
    case '&':
      token->kind = PUNCT_BAND;
      break;
    case '*':
      token->kind = PUNCT_MUL;
      break;
    case '+':
      token->kind = PUNCT_ADD;
      break;
    case '-':
      token->kind = PUNCT_SUB;
      break;
    case '~':
      token->kind = PUNCT_BNOT;
      break;
    case '!':
      token->kind = PUNCT_NOT;
      break;
    case '/':
      token->kind = PUNCT_DIV;
      break;
    case '%':
      token->kind = PUNCT_MOD;
      break;
    case '<':
      token->kind = PUNCT_LANGLE;
      break;
    case '>':
      token->kind = PUNCT_RANGLE;
      break;
    case ',':
      token->kind = PUNCT_COMMA;
      break;
    case '#':
      token->kind = PUNCT_HTAG;
      break;
    case '=':
      token->kind = PUNCT_ASSIGN;
      break;
    default:
      return 0;
    }
  } break;
  case 2: {
    if (strncmp("->", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_PDEREF;
    else if (strncmp("++", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_INCREMENT;
    else if (strncmp("--", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_DECREMENT;
    else if (strncmp("<<", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_LEFTSHIFT;
    else if (strncmp(">>", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_RIGHTSHIFT;
    else if (strncmp("<=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_LOREQ;
    else if (strncmp(">=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_GOREQ;
    else if (strncmp("==", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_EQUAL;
    else if (strncmp("!=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_NOTEQ;
    else if (strncmp("&&", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_LAND;
    else if (strncmp("||", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_LOR;
    else if (strncmp("*=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_MULEQ;
    else if (strncmp("/=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_DIVEQ;
    else if (strncmp("%=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_MODEQ;
    else if (strncmp("+=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_ADDEQ;
    else if (strncmp("-=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_SUBEQ;
    else if (strncmp("&=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_BANDEQ;
    else if (strncmp("^=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_BXOREQ;
    else if (strncmp("|=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_BOREQ;
    else if (strncmp("##", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_HHTAG;
    else
      return 0;
  } break;
  case 3: {
    if (strncmp("...", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_VARIADIC;
    else if (strncmp("<<=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_LEFTSHIFTEQ;
    else if (strncmp(">>=", &lexer->content[lexer->position], length) == 0)
      token->kind = PUNCT_RIGHTSHIFTEQ;
    else {
      return 0;
    }
  } break;
  default:
    return 0;
  }

  if (token->kind == INVALID) {
    return 0;
  }
  return 1;
}
