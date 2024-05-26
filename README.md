# Description

This is a lexer that is created for the C language but is also usable as a general purpose lexer.

The main API functions are the following.

```C
Lexer *lexer_new(char *file_path, char *content, size_t size, size_t position);
void lexer_del(Lexer *lexer);
Token lexer_next(Lexer *lexer);
```

The logging can be enabled by providing the following.
```C
#define LEX_LOGERROR
```



