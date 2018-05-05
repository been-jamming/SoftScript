/*
SoftScript datavalues

This contains the structures and functions for values stored by the interpreter.

by Ben Jones
4/9/2018
*/

#include "softscript.h"
#include "dictionary.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

datavalue *create_datavalue(){
	datavalue *output;
	output = malloc(sizeof(datavalue));
	output->value = (void *) 0;
	output->type = NONE_TYPE;
	output->num_references = 0;
	return output;
}

datavalue* increment_references(datavalue *value){
	value->num_references++;
	return value;
}

datavalue *create_nonetype(){
	datavalue *output;
	output = create_datavalue();
	output->type = NONE_TYPE;
	return output;
}

datavalue *create_integer(int value){
	datavalue *output;
	output = create_datavalue();
	output->int_value = value;
	output->type = INTEGER_TYPE;
	return output;
}

datavalue *create_float(double value){
	datavalue *output;
	output = create_datavalue();
	output->float_value = value;
	output->type = FLOAT_TYPE;
	return output;
}

datavalue *create_string(char *value){
	datavalue *output;
	output = create_datavalue();
	output->value = calloc(sizeof(char), strlen(value) + 1);
	strcpy((char *) output->value, value);
	output->type = STRING_TYPE;
	return output;
}

datavalue *create_code(unsigned int num_expressions){
	datavalue *output;
	code *c;
	output = create_datavalue();
	c = malloc(sizeof(code));
	c->expressions = malloc(sizeof(expression *)*num_expressions);
	c->num_expressions = num_expressions;
	output->value = c;
	output->type = CODE_TYPE;
	return output;
}
