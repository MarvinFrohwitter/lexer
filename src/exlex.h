#ifndef EXLEX_H_
#define EXLEX_H_
#include <assert.h>
#include <stddef.h>
#include <string.h>

#ifndef EXLEXDEF
#define EXLEXDEF static inline
#endif // EXLEXDEF

#include "lexer.h"

EXLEXDEF void lexer_keyword_set_token(Lexer *l, Token *t, size_t len);
EXLEXDEF void lexer_punctuator_set_token(Lexer *l, Token *t, size_t len);

#endif // EXLEX_H_

#ifdef EXLEX_IMPLEMENTATION

EXLEXDEF void lexer_keyword_set_token(Lexer *l, Token *t, size_t len) {

  switch (len) {
  case 2: {
    if (strncmp("do", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_DO;
    else if (strncmp("if", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_IF;
  } break;

  case 3: {
    if (strncmp("for", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_FOR;
    else if (strncmp("int", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_INT;
  } break;

  case 4: {
    if (strncmp("char", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_CHAR;
    else if (strncmp("void", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_VOID;
    else if (strncmp("else", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_ELSE;
    else if (strncmp("long", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_LONG;
    else if (strncmp("goto", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_GOTO;
    else if (strncmp("case", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_CASE;
    else if (strncmp("enum", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_ENUM;
    else if (strncmp("auto", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_AUTO;
  } break;

  case 5: {
    if (strncmp("while", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_WHILE;
    else if (strncmp("float", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_FLOAT;
    else if (strncmp("const", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_CONST;
    else if (strncmp("break", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_BREAK;
    else if (strncmp("short", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_SHORT;
    else if (strncmp("union", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_UNION;
  } break;

  case 6: {
    if (strncmp("return", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_RETURN;
    else if (strncmp("struct", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_STRUCT;
    else if (strncmp("switch", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_SWITCH;
    else if (strncmp("double", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_DOUBLE;
    else if (strncmp("sizeof", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_SIZEOF;
    else if (strncmp("static", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_STATIC;
    else if (strncmp("signed", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_SIGNED;
    else if (strncmp("extern", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_EXTERN;
  } break;

  case 7: {
    if (strncmp("default", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_DEFAULT;
    else if (strncmp("typedef", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_TYPEDEF;
  } break;

  case 8: {
    if (strncmp("continue", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_CONTINUE;
    else if (strncmp("register", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_REGISTER;
    else if (strncmp("unsigned", &l->content[l->position - len], len) == 0)
      t->kind = KEYWORD_UNSIGNED;
    else if (strncmp("volatile", &l->content[l->position - len], len) == 0)
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
    else if (strncmp("=", &l->content[l->position + len - 1], len) == 0)
      t->kind = PUNCT_ASSIGN;
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
