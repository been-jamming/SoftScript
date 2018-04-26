#ifndef LEXER_INCLUDED
#define LEXER_INCLUDED

#define NONE 0
#define SEMICOLON 1
#define INTEGER 2
#define FLOAT 3
#define STRING 4
#define COMMA 5
#define OPEN_PARENTHESES 6
#define CLOSE_PARENTHESES 7
#define VARIABLE 8
#define OPERATOR 9
#define BEGIN 10
#define OPEN_BRACES 11
#define CLOSE_BRACES 12

#include <stdbool.h>

typedef struct linked_list linked_list;

struct linked_list{
	void *value;
	linked_list *next;
	linked_list *previous;
};

typedef struct token token;

struct token{
	void *value;
	unsigned int type;
};

linked_list *create_linked_list(void *value);

linked_list *add_linked_list(linked_list **l, linked_list *element);

token *create_token(void *value, unsigned int type);

bool is_a_digit(unsigned char c);

bool is_a_letter(unsigned char c);

int parse_integer(char **c);

double parse_float(char **c);

char *parse_variable(char **c);

char *parse_string(char **c);

char *parse_operator(char **c);

unsigned int number_type(char *c);

linked_list *parse_program(char **c);

#endif