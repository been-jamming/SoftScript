/*
SoftScript expressions

These are expressions which are compiled into an AST but not yet dereferenced.

by Ben Jones
4/9/2018
*/

#ifndef EXPRESSION_INCLUDED
#define EXPRESSION_INCLUDED
#include "datavalue.h"
#include "lexer.h"
#include "hollow_lists.h"
#include "dictionary.h"

#define NONE 0
#define CODE 1
#define INT_CONST 2
#define FLOAT_CONST 3
#define STRING_CONST 4
#define FUNCTION_CALL 5
#define OPERATION 6
#define true 1
#define false 0

typedef struct expression expression;

struct expression{
	expression *args;
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

expression *create_expression();

expression *int_const_expression(token t);

expression *float_const_expression(token t);

expression *variable_expression(token t);

void add_expression(expression **output);

void func_add_expression(expression *new_expression, expression **output);

expression *build_expression(linked_list **tokens);

datavalue *evaluate_expression(expression *expr);
#endif