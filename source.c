// C Compiler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 100 // initial capacity of tokens array
bool had_error = false;
bool lexer_at_end = false;

/* ============================== TOKEN ========================================
 *
 * ===========================================================================*/
typedef enum {
  /* Single Character Tokens */
  SEMI,                      /* ; */
  COMMA,                     /* , */
  COLON,                     /* : */
  OCB,                       /* { */
  CCB,                       /* } */
  OP,                        /* ( */
  CP,                        /* ) */
  OSB,                       /* [ */
  CSB,                       /* ] */
  QUEST,                     /* ? */
  DOT,                       /* . */

  /* Single or Double Character Tokens */
  INC,                       /* ++ */
  DEC,                       /* -- */
  ARROW,                     /* -> */
  ADDEQ,                     /* += */
  SUBEQ,                     /* -= */
  MULEQ,                     /* *= */
  DIVEQ,                     /* /= */
  MODEQ,                     /* %= */
  AND,                       /* && */
  AMP,                       /* & */
  OR,                        /* || */
  LE,                        /* <= */
  GE,                        /* >= */
  LT,                        /* < */
  GT,                        /* > */
  EQ,                        /* == */
  NOTEQ,                     /* != */
  ASSIGN,                    /* = */
  NOT,                       /* ! */

  /* Keywords */
  INT,                       /* int */
  UNSIGN,                    /* unsigned*/
  CHAR,                      /* char */
  STAT,                      /* static */
  EXTERN,                    /* extern */
  CONST,                     /* const */
  IF,                        /* if */
  ELSE,                      /* else */
  WHILE,                     /* while */
  DO,                        /* do */
  FOR,                       /* for */
  SWITCH,                    /* switch */
  CASE,                      /* case */
  DEFAULT,                   /* default */
  RETURN,                    /* return */
  BREAK,                     /* break */
  CONTIN,                    /* continue */
  GOTO,                      /* goto */
  SIZEOF,                    /* sizeof */
  STRUCT,                    /* struct */
  UNION,                     /* union */
  VOID,                      /* void */
  NIL,                       /* NULL */

  /* Misc */
  END,                       /* EOF */
  IDENTIFIER, STRING, NUMBER, /* Literals */
  END // EOF
  
  // TODO support assembly level operations
  // TODO support bitwise operators
} token_type_T;

char *get_token_name(token_name_T type) {
  switch (type) {
  /* Single Character Tokens */
  case SEMI: return "SEMI";
  case COMMA: return "COMMA";
  case OCB: return "OCB";
  case CCB: return "CCB";
  case OP: return "OP";
  case CP: return "CP";
  case OSB: return "OSB";
  case CSB: return "CSB";
  case QUEST: return "QUEST"
  case DOT: return "DOT";

  /* Single or Double Character Tokens */
  case INC: return "INC";
  case DEC: return "DEC";
  case ARROW: return "ARROW";
  case ADDEQ: return "ADDEQ";
  case SUBEQ: return "SUBEQ";
  case MULEQ: return "MULEQ";
  case DIVEQ: return "DIVEQ";
  case MODEQ: return "MODEQ";
  case AND: return "AND";
  case AMP: return "AMP";
  case OR: return "OR";
  case LE: return "LE";
  case GE: return "GE";
  case LT: return "LT";
  case GT: return "GT";
  case EQ: return "EQ";
  case NOTEQ: return "NOTEQ";
  case ASSIGN: return "ASSIGN";
  case NOT: return "NOT";

  /* Keywords */
  case INT: return "INT";
  case UNSIGN: return "UNSIGN";
  case CHAR: return "CHAR";
  case STAT: return "STAT";
  case EXTERN: return "EXTERN";
  case CONST: return "CONST";
  case IF: return "IF";
  case ELSE: return "ELSE";
  case WHILE: return "WHILE";
  case DO: return "DO";
  case FOR: return "FOR";
  case SWITCH: return "SWITCH";
  case CASE: return "CASE";
  case DEFAULT: return "DEFAULT";
  case RETURN: return "RETURN";
  case BREAK: return "BREAK";
  case CONTIN: return "CONTIN";
  case GOTO: return "GOTO";
  case SIZEOF: return "SIZEOF";
  case STRUCT: return "STRUCT";
  case UNION: return "UNION";
  case VOID: return "VOID";
  case NIL: return "NIL";

  /* Misc */
  case NIL: return "NIL";
  case NIL: return "NIL";
  case NIL: return "NIL";
  
  // TODO support assembly level operations
  // TODO support bitwise operators
  }
}

typedef struct {
  token_type_T type;
  char *lexeme; 
  // TODO add overloadable literal ? might help during runtime
  unsigned int line;
} token_T;

// TODO
/*
char *get_token_as_string(token_T token) {
  pass
}
*/

/* ============================== LEXER ========================================
 *
 * ===========================================================================*/
typedef struct {
  char *src;
  size_t src_len;
  unsigned int current; // index of current char being considered
  unsigned int start; // first character in lexeme being scanned
  unsigned int line;

  token_T *tokens;
  unsigned int tokens_index;
  unsigned int tokens_capacity;
} lexer_T;

lexer_T * init_lexer(char *src, size_t len) {
  lexer_T * lexer = calloc(1, sizeof(lexer_T));
  lexer->src = src;
  lexer->src_len = len;
  lexer->current = 0;
  lexer->start = 0;
  lexer->line_number = 1;

  // TODO support dynamic resizing
  lexer->tokens_capacity = INITIAL_CAPACITY;
  lexer->tokens = malloc(sizeof(token_T) * lexer->tokens_capacity); 
  lexer->tokens_index = 0;

  return lexer;
}

bool resize(lexer_T *lexer) {
  lexer->tokens = realloc(void *lexer->tokens, (lexer->tokens_capacity * 2));

  return (lexer->tokens == NULL)
}

bool is_end(lexer_T *lexer) {
  return lexer->current >= lexer->src_len;
}

char advance(lexer_T *lexer) {
  if (lexer->current < lexer->src_size && !is_end(lexer)) {
    lexer->index++;
    lexer->current = lexer->src[lexer->index];
  }
}

void add_token(lexer_T *lexer, token_type_T t) {
  if (lexer->tokens_index + 1 > lexer->tokens_capacity) {
    if (!resize(lexer)) {
      // TODO create seperate handle for internal errors (errors that aren't from the lexer)
      perror("error: unable to resize token array");
      exit(1);
    }

  lexer->tokens[tokens_index]->type = t;

}

/* Overloaded functions handle literals */
void add_token(lexer_T *lexer, token_type_T t, char *literal) {
  
  
}

void add_token(lexer_T *lexer, token_type_T t, int literal) {
  
}


void consume_token(lexer_T *lexer) {
  char c = advance(lexer);

  switch (c):
    // single char 
    case ';': add_token(lexer, SEMI); break;
    case ',': add_token(lexer, COMMA); break;
    case ':': add_token(lexer, COLON); break;
    case '(': add_token(lexer, OP); break;
    case ')': add_token(lexer, CP); break;
    case '{': add_token(lexer, OCB); break;
    case '}': add_token(lexer, CCB); break;
    case '[': add_token(lexer, OSB); break;
    case ']': add_token(lexer, CSB); break;
    case '?': add_token(lexer, COMMA); break;
    case '.': add_token(lexer, DOT); break;

    // single or double char

    default: error(lexer->line_number, "unexpected character."); break;
}

// tokens are added to lexer
void scan_tokens(lexer_T *lexer) {
  while (!is_end()) {
    lexer->start = lexer->current;
    consume_token(lexer);
  }

  add_token(lexer, END, lexer->line_number);
}

/* ============================== PARSER =======================================
 *
 * ===========================================================================*/
  // pass

/* ============================== ERROR HANDLING ===============================
 *
 * ===========================================================================*/
void error(int line, char *msg) {
  printf("line %d | error: %s\n", line, msg);
  // TODO Halt Parser
  had_error = true;
}

/* ============================== DRIVER =======================================
 *
 * ===========================================================================*/
size_t get_file_size(FILE *f) {
  size_t file_size;
  if (fseek(f, 0, SEEK_END) != 0 ) exit(EXIT_FAILURE); 

  file_size = ftell(f);
  rewind(f);
  return file_size;
}

void read_file(FILE * f, char *buffer, size_t file_size) {
  if (file_size == 1) exit(EXIT_FAILURE);

  fread(buffer, 1, file_size, f);
  if (buffer == NULL) exit(EXIT_FAILURE);

  fclose(f); 
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("%s\n", "usage: source <filename>");
    exit(1);
  }

  FILE *f;
  if(!(f = fopen(argv[1], "r"))) {
    printf("%s\n", "error: file not found");
    exit(1);
  }

  size_t file_size = get_file_size(f);
  char *buffer = malloc(file_size * sizeof(char));

  read_file(f, buffer, file_size);

  printf("%s\n", buffer); 
}
