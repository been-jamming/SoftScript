#ifndef SOFTSCRIPT_INCLUDED
#define SOFTSCRIPT_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hollow_lists.h"
#include "dictionary.h"

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

#define NONE 0
#define CODE 1
#define INT_CONST 2
#define FLOAT_CONST 3
#define STRING_CONST 4
#define FUNCTION_CALL 5
#define OPERATION 6
#define true 1
#define false 0

#define NONE_TYPE 0
#define INTEGER_TYPE 1
#define FLOAT_TYPE 2
#define STRING_TYPE 3
#define FUNCTION_TYPE 4
#define CODE_TYPE 5

typedef struct {
	void *value;
	unsigned int type;
	int num_references;
} datavalue;

typedef struct {
	dictionary *variables;
	unsigned int id;
} varspace;

typedef struct {
	void (*func)(void *);
} free_func;

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

typedef struct expression expression;

struct expression{
	expression **args;
	expression *child1;
	expression *child2;
	unsigned int num_args;
	unsigned int type;
	bool root_node;
	bool top_node;
	datavalue *constant;
	datavalue **variable;
	hollow_list *operators;
	datavalue *output;
	bool begin;
};

typedef struct operation operation;

struct operation{
	datavalue *(*func)(datavalue *, datavalue *, expression *);
};

typedef struct function_value function_value;

struct function_value {
	datavalue *(*function)(expression **, unsigned int);
};

typedef struct code code;

struct code{
	expression **expressions;
	unsigned int num_expressions;
};

dictionary *globals;
dictionary *operators;
hollow_list *free_datatypes;

char *read_input();

void discard_data(datavalue *value);

void INCLUDE();

datavalue *run_code(code *c);

void create_variable(char *string, datavalue *value);

void create_operation(unsigned int datatype, char *string, datavalue *(*function)(datavalue *, datavalue *, expression *));

void free_data(datavalue *value);

void create_function(char *string, datavalue *(*function)(expression **, unsigned int));

void create_free_func(unsigned int datatype, void (*func)(void *));

expression *create_expression();

expression *int_const_expression(token t);

expression *float_const_expression(token t);

expression *variable_expression(token t);

void add_expression(expression **output);

void func_add_expression(expression *new_expression, expression **output);

expression *build_expression(linked_list **tokens);

datavalue *evaluate_expression(expression *expr);

unsigned int varspace_current_id;

datavalue *create_datavalue();

datavalue *increment_references(datavalue *value);

varspace *create_varspace();

datavalue *create_integer(int value);

datavalue *create_float(double value);

datavalue *create_string(char *value);

datavalue *create_code(unsigned int num_expressions);

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

datavalue *create_nonetype();

#endif