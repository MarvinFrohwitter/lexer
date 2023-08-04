typedef struct Keyword {
  char *auto_t;
  char *break_t;
  char *case_t;
  char *char_t;
  char *const_t;
  char *continue_t;
  char *default_t;
  char *do_t;
  char *double_t;
  char *else_t;
  char *enum_t;
  char *extern_t;
  char *float_t;
  char *for_t;
  char *goto_t;
  char *if_t;
  char *int_t;
  char *long_t;
  char *register_t;
  char *return_t;
  char *short_t;
  char *signed_t;
  char *sizeof_t;
  char *static_t;
  char *struct_t;
  char *switch_t;
  char *typedef_t;
  char *union_t;
  char *unsigned_t;
  char *void_t;
  char *volatile_t;
  char *while_t;
} Keyword;

typedef struct Identifier {
} Identifier;

typedef struct Constant {
} Constant;

typedef struct StringLiteral {
  char quote;
} StringLiteral;

typedef struct Punctuator {
  char *lbracket_t;
  char *rbracket_t;
  char *lparen_t;
  char *rparen_t;
  char *lbrace_t;
  char *rbrace_t;
  char *point_t;
  char *pderef_t;
  char *increment_t;
  char *decrement_t;
  char *band_t;
  char *mul_t;
  char *add_t;
  char *sub_t;
  char *bnot_t;
  char *not_t;
  char *div_t;
  char *mod_t;
  char *leftshift_t;
  char *rightshift_t;
  char *lange_t;
  char *range_t;
  char *loreq_t;
  char *goreq_t;
  char *equal_t;
  char *noteq_t;
  char *bxor_t;
  char *bor_t;
  char *land_t;
  char *lor_t;
  char *ask_t;
  char *colon_t;
  char *semicolon_t;
  char *variadic_t;
  char *assign_t;
  char *muleq_t;
  char *diveq_t;
  char *modeq_t;
  char *addeq_t;
  char *subeq_t;
  char *leftshifteq_t;
  char *rightshifteq_t;
  char *bandeq_t;
  char *bxoreq_t;
  char *boreq_t;
  char *comma_t;
  char *htag_t;
  char *hhtag_t;

  // char *langecolon_t = lbracket_t;    // '<: = ['
  // char *colonrange_t = rbracket_t;    // ':> = ]'
  // char *langelmod_t = lbrace_t;       // '<% = {'
  // char *kangelmod_t = rbrace_t;       // '%> = }'
  // char *modcolon_t = htag_t;          // '%: = #'
  // char *modcolonmodcolon_t = hhtag_t; // '%:%: = ##'

} Punctuator;

typedef struct HeaderName {
} HeaderName;
typedef struct PPNumber {
} PPNumber;
typedef struct CharacterConstant {
} CharacterConstant;
typedef struct NonWhiteSpaceChar {
} NonWhiteSpaceChar;

typedef struct PreprocessingToken {
  HeaderName header_name;
  Identifier identifier;
  PPNumber pp_number;
  CharacterConstant character_constant;
  StringLiteral string_literal;
  Punctuator punctuator;
  NonWhiteSpaceChar non_white_space_char;

} PreprocessingToken;

typedef struct TokenKind {
  Keyword keyword;
  Identifier identifier;
  Constant constant;
  StringLiteral string_literal;
  Punctuator punctuator;

} TokenKind;

typedef struct TokenVarient {
  TokenKind token_kind;
  PreprocessingToken preprocessing_token;
} TokenVarient;

typedef enum Kind {
  KEYWORD,
  IDENTIFIER,
  CONSTANT,
  STRINGLITERAL,
  PUNCTUATOR,
  INVALID
} Kind;

typedef struct Token {
  Kind kind;
  const char *content;
  size_t size;
} Token;

typedef struct Lexer {
  const char *content;
  size_t content_lenght;
  size_t position;
  TokenVarient token_varient;

} Lexer;

Lexer lexer_new(char *content, size_t size, size_t position);
Lexer *lexer_token_set_keywords(Lexer *lexer);
Lexer *lexer_token_set_punctuator(Lexer *lexer);
Lexer *lexer_token_set_string_literal(Lexer *lexer);
Token lexer_next(Lexer *lexer);
void lexer_trim_left(Lexer *lexer);
void lexer_chop_char(Lexer *lexer, size_t count);

int issybol_alpha(char c);
int issybol_alpha_and_(char c);
