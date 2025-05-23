#include "lexer.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ============================================================================
// ========================= Utility Functions ================================
// ============================================================================
// ============================================================================

const char *lexer_kind_to_str(Kind kind) {
  switch (kind) {
  case INVALID:
    return "INVALID";
  case ERROR:
    return "ERROR";
  case NUMBER:
    return "NUMBER";
  case KEYWORD:
    return "KEYWORD";
  case STRINGLITERAL:
    return "STRINGLITERAL";
  case IDENTIFIER:
    return "IDENTIFIER";
  case PUNCTUATOR:
    return "PUNCTUATOR";
  case CONSTANT:
    return "CONSTANT";
  case PREPROCESSING:
    return "PREPROCESSING";
  case COMMENT:
    return "COMMENT";
  case EOF_TOKEN:
    return "EOF_TOKEN";
  case PUNCT_SINGLEQUOTE:
    return "PUNCT_SINGLEQUOTE";
  case PUNCT_BACKSLASH:
    return "PUNCT_BACKSLASH";
  case PUNCT_NOT:
    return "PUNCT_NOT";
  case PUNCT_HTAG:
    return "PUNCT_HTAG";
  case PUNCT_MOD:
    return "PUNCT_MOD";
  case PUNCT_BAND:
    return "PUNCT_BAND";
  case PUNCT_MUL:
    return "PUNCT_MUL";
  case PUNCT_ADD:
    return "PUNCT_ADD";
  case PUNCT_COMMA:
    return "PUNCT_COMMA";
  case PUNCT_SUB:
    return "PUNCT_SUB";
  case PUNCT_DIV:
    return "PUNCT_DIV";
  case PUNCT_LPAREN:
    return "PUNCT_LPAREN";
  case PUNCT_RPAREN:
    return "PUNCT_RPAREN";
  case PUNCT_POINT:
    return "PUNCT_POINT";
  case PUNCT_COLON:
    return "PUNCT_COLON";
  case PUNCT_SEMICOLON:
    return "PUNCT_SEMICOLON";
  case PUNCT_LANGLE:
    return "PUNCT_LANGLE";
  case PUNCT_ASSIGN:
    return "PUNCT_ASSIGN";
  case PUNCT_RANGLE:
    return "PUNCT_RANGLE";
  case PUNCT_ASK:
    return "PUNCT_ASK";
  case PUNCT_LBRACKET:
    return "PUNCT_LBRACKET";
  case PUNCT_RBRACKET:
    return "PUNCT_RBRACKET";
  case PUNCT_BXOR:
    return "PUNCT_BXOR";
  case PUNCT_LBRACE:
    return "PUNCT_LBRACE";
  case PUNCT_BOR:
    return "PUNCT_BOR";
  case PUNCT_RBRACE:
    return "PUNCT_RBRACE";
  case PUNCT_BNOT:
    return "PUNCT_BNOT";
  case PUNCT_EQUAL:
    return "PUNCT_EQUAL";
  case PUNCT_MULEQ:
    return "PUNCT_MULEQ";
  case PUNCT_DIVEQ:
    return "PUNCT_DIVEQ";
  case PUNCT_MODEQ:
    return "PUNCT_MODEQ";
  case PUNCT_ADDEQ:
    return "PUNCT_ADDEQ";
  case PUNCT_SUBEQ:
    return "PUNCT_SUBEQ";
  case PUNCT_LEFTSHIFTEQ:
    return "PUNCT_LEFTSHIFTEQ";
  case PUNCT_RIGHTSHIFTEQ:
    return "PUNCT_RIGHTSHIFTEQ";
  case PUNCT_BANDEQ:
    return "PUNCT_BANDEQ";
  case PUNCT_BOREQ:
    return "PUNCT_BOREQ";
  case PUNCT_BXOREQ:
    return "PUNCT_BXOREQ";
  case PUNCT_NOTEQ:
    return "PUNCT_NOTEQ";
  case PUNCT_LOREQ:
    return "PUNCT_LOREQ";
  case PUNCT_GOREQ:
    return "PUNCT_GOREQ";
  case PUNCT_LAND:
    return "PUNCT_LAND";
  case PUNCT_LOR:
    return "PUNCT_LOR";
  case PUNCT_LEFTSHIFT:
    return "PUNCT_LEFTSHIFT";
  case PUNCT_RIGHTSHIFT:
    return "PUNCT_RIGHTSHIFT";
  case PUNCT_PDEREF:
    return "PUNCT_PDEREF";
  case PUNCT_DECREMENT:
    return "PUNCT_DECREMENT";
  case PUNCT_INCREMENT:
    return "PUNCT_INCREMENT";
  case PUNCT_HHTAG:
    return "PUNCT_HHTAG";
  case PUNCT_VARIADIC:
    return "PUNCT_VARIADIC";
  case KEYWORD_AUTO:
    return "KEYWORD_AUTO";
  case KEYWORD_BREAK:
    return "KEYWORD_BREAK";
  case KEYWORD_CASE:
    return "KEYWORD_CASE";
  case KEYWORD_CHAR:
    return "KEYWORD_CHAR";
  case KEYWORD_CONST:
    return "KEYWORD_CONST";
  case KEYWORD_CONTINUE:
    return "KEYWORD_CONTINUE";
  case KEYWORD_DEFAULT:
    return "KEYWORD_DEFAULT";
  case KEYWORD_DO:
    return "KEYWORD_DO";
  case KEYWORD_DOUBLE:
    return "KEYWORD_DOUBLE";
  case KEYWORD_ELSE:
    return "KEYWORD_ELSE";
  case KEYWORD_ENUM:
    return "KEYWORD_ENUM";
  case KEYWORD_EXTERN:
    return "KEYWORD_EXTERN";
  case KEYWORD_FLOAT:
    return "KEYWORD_FLOAT";
  case KEYWORD_FOR:
    return "KEYWORD_FOR";
  case KEYWORD_GOTO:
    return "KEYWORD_GOTO";
  case KEYWORD_IF:
    return "KEYWORD_IF";
  case KEYWORD_INT:
    return "KEYWORD_INT";
  case KEYWORD_LONG:
    return "KEYWORD_LONG";
  case KEYWORD_REGISTER:
    return "KEYWORD_REGISTER";
  case KEYWORD_RETURN:
    return "KEYWORD_RETURN";
  case KEYWORD_SHORT:
    return "KEYWORD_SHORT";
  case KEYWORD_SIGNED:
    return "KEYWORD_SIGNED";
  case KEYWORD_SIZEOF:
    return "KEYWORD_SIZEOF";
  case KEYWORD_STATIC:
    return "KEYWORD_STATIC";
  case KEYWORD_STRUCT:
    return "KEYWORD_STRUCT";
  case KEYWORD_SWITCH:
    return "KEYWORD_SWITCH";
  case KEYWORD_TYPEDEF:
    return "KEYWORD_TYPEDEF";
  case KEYWORD_UNION:
    return "KEYWORD_UNION";
  case KEYWORD_UNSIGNED:
    return "KEYWORD_UNSIGNED";
  case KEYWORD_VOID:
    return "KEYWORD_VOID";
  case KEYWORD_VOLATILE:
    return "KEYWORD_VOLATILE";
  case KEYWORD_WHILE:
    return "KEYWORD_WHILE";
  case KEYWORD_SIZE_T:
    return "KEYWORD_SIZE_T";
  default:
    assert("UNREACHABLE");
  }

  return NULL;
}

struct {
  char *elements;
  size_t count;
  size_t capacity;
} tmp_buffer;

char *tmp_sprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int ret = vsnprintf(NULL, 0, fmt, args);
  if (ret == 0) {
    return NULL;
  }
  va_end(args);

  va_start(args, fmt);
  ret += 1;
  tmp_buffer.count = 0;
  lexer_dapc(&tmp_buffer, "\0", ret);
  vsnprintf(tmp_buffer.elements, ret, fmt, args);
  va_end(args);
  return tmp_buffer.elements;
}
