// C Compiler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool had_error = false;

typedef enum {
  // single character tokens
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	LEFT_CURLY, RIGHT_CURLY, COMMA, DOT, MINUS, PLUS, 
	SEMICOLON, SLASH, STAR,

  // single or double character tokens
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // literals
  IDENTIFIER, STRING, NUMBER,

  // keywords
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  END // EOF
} token_type_T;

typedef struct {
  token_type_T type;
  char *lexeme; 
  // TODO add overloadable literal ?
  unsigned int line;
} token_T;

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

/* Error Handling */
void error(int line, char *msg) {
  printf("line %d | error: %s\n", line, msg);
  // TODO Halt Parser
  had_error = true;
}

/* Driver */
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