#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"
#include "hollow_lists.h"
#include "softscript.h"

void create_variable(char *string, datavalue *value){
	datavalue **new_pointer;
	new_pointer = malloc(sizeof(datavalue **));
	*new_pointer = value;
	dictionary_write(globals, string, new_pointer);
}

void create_function(char *string, datavalue *(*function)(expression **, unsigned int)){
	function_value *f;
	datavalue *var;
	datavalue **new_pointer;
	new_pointer = malloc(sizeof(datavalue **));
	var = create_datavalue();
	var->type = FUNCTION_TYPE;
	f = malloc(sizeof(function_value));
	f->function = function;
	var->value = f;
	*new_pointer = var;
	dictionary_write(globals, string, new_pointer);
}

void create_operation(unsigned int datatype, char *string, datavalue *(*function)(datavalue *, datavalue *, expression *)){
	hollow_list *operation_list;
	operation *oper;
	operation_list = (hollow_list *) dictionary_read(operators, string);
	if(operation_list == (hollow_list *) 0){
		operation_list = malloc(sizeof(hollow_list));
		*operation_list = hollow_list_create(16);
		dictionary_write(operators, string, (void *) operation_list);
	}
	oper = malloc(sizeof(operation));
	oper->func = function;
	hollow_list_write(operation_list, datatype, (void *) oper);
}

void create_free_func(unsigned int datatype, void (*func)(void *)){
	free_func *f;
	f = malloc(sizeof(free_func));
	f->func = func;
	hollow_list_write(free_datatypes, datatype, f);
}

void free_data(datavalue *value){
	free_func *f;
	if(value == (datavalue *) 0){
		return;
	}
	f = (free_func *) hollow_list_read(free_datatypes, value->type);
	if(f == (free_func *) 0){
		printf("Could not free data\n");
		exit(1);
	} else {
		f->func(value->value);
		free(value);
	}
}

void discard_data(datavalue *value){
	if(value == (datavalue *) 0){
		return;
	}
	value->num_references--;
	if(value->num_references <= 0){
		free_data(value);
	} else {
		//printf("%d\n", value->num_references);
	}
}

char program[256];

void main(){
	char *program_pointer;
	globals = malloc(sizeof(dictionary));
	operators = malloc(sizeof(dictionary));
	free_datatypes = malloc(sizeof(hollow_list));
	*globals = dictionary_create();
	*operators = dictionary_create();
	*free_datatypes = hollow_list_create(16);
	INCLUDE();
	gets(program);
	linked_list *parsed_program;
	expression *built_program;
	datavalue *output;
	program_pointer = program;
	printf("lexing...\n");
	parsed_program = parse_program(&program_pointer);
	printf("building...\n");
	built_program = build_expression(&parsed_program);
	printf("executing...\n");
	output = evaluate_expression(built_program);
	printf("done\n");
	printf("result: ");
	if(output->type == INTEGER_TYPE){
		printf("%d\n", *((int *) output->value));
	} else if(output->type == FLOAT_TYPE){
		printf("%lf\n", *((double *) output->value));
	} else if(output->type == STRING_TYPE){
		printf("%s\n", (char *) output->value);
	} else if(output->type == NONE_TYPE){
		printf("NONE\n");
	} else {
		printf("unknown %d\n", output->type);
	}
}
