// C Compiler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 100 // initial capacity of tokens array
#define MAX_BUFF_SIZE 10000
bool had_error = false;
bool lexer_at_end = false;

/* ============================== ERROR HANDLING ===============================
 *
 * ===========================================================================*/
void error(int line, char *msg) {
  printf("line %d | error: %s\n", line, msg);
  // TODO Halt Parser, enter panic mode
  had_error = true;
}

/* ============================== HELPERS ======================================
 *
 * ===========================================================================*/
 bool is_digit(char c) { return c >= '0' && c <= '9'; }


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
  ADD,                       /* + */
  INC,                       /* ++ */
  ADDEQ,                     /* += */

  SUB,                       /* - */
  DEC,                       /* -- */
  ARROW,                     /* -> */
  SUBEQ,                     /* -= */

  MUL,                       /* * */
  MULEQ,                     /* *= */

  DIV,                       /* / */
  DIVEQ,                     /* /= */

  MOD,                       /* % */
  MODEQ,                     /* %= */

  NOT,                       /* ! */
  NOTEQ,                     /* != */

  AMP,                       /* & */
  AND,                       /* && */
  
  LT,                        /* < */
  LE,                        /* <= */

  GT,                        /* > */
  GE,                        /* >= */

  ASSIGN,                    /* = */
  EQ,                        /* == */
  
  BWOR,                      /* | */ // bitwise OR
  OR,                        /* || */

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
  
  // TODO support assembly level operations
  // TODO support bitwise operators
} token_type_T;

char *get_token_name(token_type_T type) {
  switch (type) {
  /* Single Character Tokens */
  case SEMI: return "SEMI";
  case COMMA: return "COMMA";
  case COLON: return "COLON";
  case OCB: return "OCB";
  case CCB: return "CCB";
  case OP: return "OP";
  case CP: return "CP";
  case OSB: return "OSB";
  case CSB: return "CSB";
  case QUEST: return "QUEST";
  case DOT: return "DOT";

  /* Single or Double Character Tokens */
  case ADD: return "ADD";
  case INC: return "INC";
  case ADDEQ: return "ADDEQ";

  case SUB: return "SUB";
  case DEC: return "DEC";
  case ARROW: return "ARROW";
  case SUBEQ: return "SUBEQ";
  
  case MUL: return "MUL";

  case DIV: return "DIV";

  case MOD: return "MOD";

  case MULEQ: return "MULEQ";
  case DIVEQ: return "DIVEQ";
  case MODEQ: return "MODEQ";

  case AMP: return "AMP";
  case AND: return "AND";

  case LE: return "LE";
  case GE: return "GE";
  case LT: return "LT";
  case GT: return "GT";

  case ASSIGN: return "ASSIGN";
  case EQ: return "EQ";
  
  case NOT: return "NOT";
  case NOTEQ: return "NOTEQ";
  
  case BWOR: return "BWOR";
  case OR: return "OR";

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
  case END: return "END";
  case IDENTIFIER: return "IDENTIFIER";
  case STRING: return "STRING";
  case NUMBER: return "NUMBER";
  
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
  lexer->line = 1;

  // TODO support dynamic resizing
  lexer->tokens_capacity = INITIAL_CAPACITY;
  lexer->tokens = malloc(sizeof(token_T) * lexer->tokens_capacity); 
  lexer->tokens_index = 0;

  return lexer;
}

bool resize(lexer_T *lexer) {
  lexer->tokens = realloc(lexer->tokens, (lexer->tokens_capacity * 2));

  return (lexer->tokens == NULL);
}

bool is_end(lexer_T *lexer) { return lexer->current >= lexer->src_len; }

char advance(lexer_T *lexer) {
  if (lexer->current < lexer->src_len && !is_end(lexer)) {
    lexer->current++; 
    return lexer->src[lexer->current];
  }

  return -1;
}

void add_token(lexer_T *lexer, token_type_T t) {
  if (lexer->tokens_index + 1 > lexer->tokens_capacity) {
    if (!resize(lexer)) {
      // TODO create seperate handle for internal errors (errors that aren't from the lexer)
      perror("error: unable to resize token array");
      exit(1);
    }
  }

  lexer->tokens[lexer->tokens_index].type = t;
  lexer->tokens[lexer->tokens_index].lexeme = get_token_name(t);
  lexer->tokens[lexer->tokens_index].line = lexer->line;

  lexer->tokens_index += 1;
}

/* Overloaded functions handle literals */
void add_token_str(lexer_T *lexer, token_type_T t, char *literal) {
  if (lexer->tokens_index + 1 > lexer->tokens_capacity) {
    if (!resize(lexer)) {
      // TODO create seperate handle for internal errors (errors that aren't from the lexer)
      perror("error: unable to resize token array");
      exit(1);
    }
  }

  lexer->tokens[lexer->tokens_index].type = t;
  lexer->tokens[lexer->tokens_index].lexeme = literal;
  lexer->tokens[lexer->tokens_index].line = lexer->line;

  lexer->tokens_index += 1;
}

void add_token_int(lexer_T *lexer, token_type_T t, int literal) {
  if (lexer->tokens_index + 1 > lexer->tokens_capacity) {
    if (!resize(lexer)) {
      // TODO create seperate handle for internal errors (errors that aren't from the lexer)
      perror("error: unable to resize token array");
      exit(1);
    }
  }
  
  char buf[10];
  sprintf(buf,"%d", literal);
  lexer->tokens[lexer->tokens_index].type = t;
  lexer->tokens[lexer->tokens_index].lexeme = buf;
  lexer->tokens[lexer->tokens_index].line = lexer->line;

  lexer->tokens_index += 1;
}

char peek(lexer_T *lexer, int offset) {
  if ((lexer->current + offset) >= lexer->src_len) return '\0';
  return lexer->src[lexer->current + offset];
}

bool match(lexer_T *lexer, char expected) {
  if (is_end(lexer)) return false;
  if (peek(lexer, 1) != expected) return false;

  advance(lexer);
  return true;
}

void handle_string(lexer_T *lexer) {
  unsigned int i = 0;
  char buf[MAX_BUFF_SIZE];

  while (peek(lexer,1) != '"' && !is_end(lexer)) {
    if (peek(lexer,1) == '\n') lexer->line += 1;
    buf[i] = lexer->src[lexer->current];
    i += 1;
    advance(lexer);
  }

  if (is_end(lexer)) {
    error(lexer->line, "unterminated string");
    return;
  }

  advance(lexer); // move to termination of string
  buf[i] = '\0';

  add_token_str(lexer, STRING, buf);
}

// FIXME Handle all numeric types. For now, just integers
void handle_numeric(lexer_T *lexer) {
  unsigned int i = 0;
  char buf[MAX_BUFF_SIZE];

  while (is_digit(peek(lexer,1)) && !is_end(lexer)) {
    buf[i] = lexer->src[lexer->current];
    i += 1;
    advance(lexer);
  }

  advance(lexer); // move to termination of string
  buf[i] = '\0';
  
  int res;
  sprintf(buf, "%d", res);

  add_token_int(lexer, NUMERIC, res);
}

void consume_token(lexer_T *lexer) {
  char c = advance(lexer);

  switch (c) {
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
    case '+': 
      if (match(lexer, '+')) {
        add_token(lexer, INC);
      }
      else if (match(lexer, '=')) {
        add_token(lexer, ADDEQ);
      }
      else add_token(lexer, ADD);
      break;

    case '-': 
      if (match(lexer, '-')) {
        add_token(lexer, DEC);
      }
      else if (match(lexer, '>')) {
        add_token(lexer, ARROW);
      }
      else if (match(lexer, '=')) {
        add_token(lexer, SUBEQ);
      }
      else add_token(lexer, SUB);
      break;

    case '*': 
      add_token(lexer, match(lexer, '=') ? MULEQ : MUL); break;

    case '/': 
      // single line comment 
      if (match(lexer, '/')) {
        while (peek(lexer, 1) != '\n' && !is_end(lexer)) advance(lexer);
      }
      // multi-line comment 
      if (match(lexer, '*')) {
        while (!(peek(lexer, 1) == '*' && peek(lexer,2) == '/') && !is_end(lexer)) {
          advance(lexer);
        }
        
        if (is_end(lexer)) error(lexer->line, "unterminated comment"); 
        else advance(lexer); // move to the termination of the multi line comment
      }
      else if (match(lexer, '=')) add_token(lexer, DIVEQ);
      else add_token(lexer, DIV);
      break;

    case '%': 
      add_token(lexer, match(lexer, '=') ? MOD : MODEQ); break;

    case '!': 
      add_token(lexer, match(lexer, '=') ? NOT : NOTEQ); break;

    case '&': 
      add_token(lexer, match(lexer, '&') ? AMP : AND); break;

    case '<': 
      add_token(lexer, match(lexer, '=') ? LT : LTEQ); break;
    
    case '>': 
      add_token(lexer, match(lexer, '=') ? GT : GTEQ); break;

    case '=': 
      add_token(lexer, match(lexer, '=') ? ASSIGN : EQ); break;

    case '|':
      add_token(lexer, match(lexer, '|') ? BWOR : OR); break;

    // String literals
    case '"': handle_string(lexer); break;
    // Numeric literals
    case is_digit(c): handle_numeric(lexer); break;

    // skip tabs, spaces, carriage returns and newlines
    case ' ':
    case '\t':
    case '\r':
      break;

    case '\n': lexer->line += 1; break;

    default: error(lexer->line, "unexpected character."); break;
  }
}

// tokens are added to lexer
void scan_tokens(lexer_T *lexer) {
  while (!is_end(lexer)) {
    consume_token(lexer);
  }

  add_token(lexer, END);
}

/* ============================== PARSER =======================================
 *
 * ===========================================================================*/
  // pass


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

  printf(buffer);

  printf("%s\n", buffer); 
}
