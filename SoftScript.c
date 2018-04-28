#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dictionary.h"
#include "hollow_lists.h"
#include "softscript.h"

unsigned int create_datatype(){
	unsigned int output;
	output = next_datatype;
	next_datatype++;
	return output;
}

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
		printf("Could not free data %d\n", value->type);
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

char *read_input(){
   char* input = NULL;
   char tempbuf[256];
   size_t inputlen = 0, templen = 0;
   do {
       fgets(tempbuf, 256, stdin);
       templen = strlen(tempbuf);
       inputlen += templen;
       input = realloc(input, inputlen+1);
       strcat(input, tempbuf);
    } while (templen==255 && tempbuf[254]!='\n');
	input[inputlen-1] = (char) 0;
    return input;
}

int main(int argc, char *argv[]){
	char *program;
	char *program_pointer;
	char *line;
	char *line_pointer;
	linked_list *parsed_program;
	linked_list *parsed_program_copy;
	expression *built_program;
	datavalue *output;
	FILE *fp;
	int fsize;
	srand(time(NULL));
	next_datatype = 6;
	globals = malloc(sizeof(dictionary));
	operators = malloc(sizeof(dictionary));
	free_datatypes = malloc(sizeof(hollow_list));
	*globals = dictionary_create();
	*operators = dictionary_create();
	*free_datatypes = hollow_list_create(16);
	INCLUDE();
	if(argc < 2){
		printf("SoftScript ver 1.0\n\n");
		while(1){
			printf(">>>");
			line = malloc(sizeof(char)*256);
			fgets(line, 255, stdin);
			line_pointer = line;
			parsed_program = parse_program(&line_pointer);
			parsed_program_copy = parsed_program;
			built_program = build_expression(&parsed_program_copy);
			output = evaluate_expression(built_program);
			if(output->type == INTEGER_TYPE){
				printf("%d\n", *((int *) output->value));
			} else if(output->type == FLOAT_TYPE){
				printf("%lf\n", *((double *) output->value));
			} else if(output->type == STRING_TYPE){
				printf("\"%s\"\n", (char *) output->value);
			}
			free(line);
			free_expression(built_program);
			free_tokens(parsed_program);
		}
	} else {
		fp = fopen(argv[1], "rb");
		fseek(fp, 0L, SEEK_END);
		fsize = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		program = malloc(sizeof(char)*(fsize+1));
		if(program == NULL){
			return 1;
		}
		fread(program, sizeof(char), fsize, fp);
		fclose(fp);
		program[fsize] = (char) 0;
		program_pointer = program;
		parsed_program = parse_program(&program_pointer);
		parsed_program_copy = parsed_program;
		built_program = build_expression(&parsed_program_copy);
		
		evaluate_expression(built_program);
		free(program);
		free_expression(built_program);
		free_tokens(parsed_program);
	}
	return 0;
}
