/*
SoftScript datavalues

This contains the structures and functions for values stored by the interpreter.

by Ben Jones
4/9/2018
*/

#ifndef DATAVALUE_INCLUDED
#define DATAVALUE_INCLUDED

#include "hollow_lists.h"
#include "dictionary.h"
//#include "expression.h"

#define NONE_TYPE 0
#define INTEGER_TYPE 1
#define FLOAT_TYPE 2
#define STRING_TYPE 3
#define FUNCTION_TYPE 4

unsigned int varspace_current_id;

typedef struct {
	void *value;
	unsigned int type;
	unsigned int num_references;
} datavalue;

typedef struct {
	dictionary *variables;
	unsigned int id;
} varspace;

typedef struct function_value function_value;

struct function_value {
	datavalue *(*function)(expression *, unsigned int);
};

datavalue *create_datavalue();

varspace *create_varspace();

datavalue *create_integer(int value);

datavalue *create_float(double value);

datavalue *create_string(char *value);
#endif