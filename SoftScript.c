#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <setjmp.h>
#include <string.h>
#include <stdbool.h>
#include "dictionary.h"
#include "hollow_lists.h"
#include "softscript.h"

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

unsigned int create_datatype(){
	unsigned int output;
	output = next_datatype;
	next_datatype++;
	return output;
}

void create_variable(char *string, datavalue *value){
	datavalue **new_pointer;
	datavalue **old_pointer;
	char *buffer;
	new_pointer = malloc(sizeof(datavalue **));
	*new_pointer = value;
	old_pointer = (datavalue **) dictionary_read(globals, string);
	if(old_pointer){
		buffer = malloc(sizeof(char)*256);
		snprintf(buffer, 256, "Warning: included extension overwriting variable %s", string);
		warning(buffer);
		discard_data(*old_pointer);
		free(old_pointer);
		free(buffer);
	}
	dictionary_write(globals, string, new_pointer);
}

void create_function(char *string, datavalue *(*function)(expression **, unsigned int)){
	function_value *f;
	datavalue *var;
	datavalue **new_pointer;
	datavalue **old_pointer;
	char *buffer;
	new_pointer = malloc(sizeof(datavalue **));
	var = increment_references(create_datavalue());
	var->type = FUNCTION_TYPE;
	f = malloc(sizeof(function_value));
	f->function = function;
	var->value = f;
	*new_pointer = var;
	old_pointer = (datavalue **) dictionary_read(globals, string);
	if(old_pointer){
		buffer = malloc(sizeof(char)*256);
		snprintf(buffer, 256, "Warning: included extension overwriting function %s", string);
		warning(buffer);
		discard_data(*old_pointer);
		free(old_pointer);
		free(buffer);
	}
	dictionary_write(globals, string, new_pointer);
}

void create_operation(unsigned int datatype, char *string, datavalue *(*function)(datavalue *, datavalue *, expression *), unsigned int type, bool optimize){
	operation *oper;
	operation **operation_list;
	char *buffer;
	oper = malloc(sizeof(operation));
	oper->function = function;
	oper->type = type;
	oper->optimize = optimize;
	operation_list = (operation **) dictionary_read(operators, string);
	if(operation_list == (operation **) 0){
		operation_list = calloc(next_datatype, sizeof(operation));
		dictionary_write(operators, string, (void *) operation_list);
	}
	if(operation_list[datatype]){
		buffer = malloc(sizeof(char)*256);
		snprintf(buffer, 256, "Warning: included extension overwriting operation %s for datatype %d", string, datatype);
		warning(buffer);
		free(operation_list[datatype]);
		free(buffer);
	}
	operation_list[datatype] = oper;
}

void create_free_func(unsigned int datatype, void (*function)(void *)){
	free_func *f;
	char *buffer;
	f = malloc(sizeof(free_func));
	f->function = function;
	if(free_datatypes[datatype]){
		buffer = malloc(sizeof(char)*256);
		snprintf(buffer, 256, "Warning: included extension overwriting free function for datatype %d", datatype);
		warning(buffer);
		free(free_datatypes[datatype]);
		free(buffer);
	}
	free_datatypes[datatype] = f;
}

void free_data(datavalue *value){
	free_func *f;
	char *buffer;
	if(value == (datavalue *) 0){
		return;
	}
	f = free_datatypes[value->type];
	if(f == (free_func *) 0){
		buffer = malloc(sizeof(char)*256);
		snprintf(buffer, 256, "Could not free data for datatype %d", value->type);
		error(buffer);
	} else {
		f->function(value->value);
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

void create_free_list(){
	free_datatypes = calloc(next_datatype, sizeof(free_func));
}

void error(char *error_string){
	if(current_expression){
		printf("---\nLine %d in %s\n%s\n---\n", current_expression->line_num, file_name, error_string);
	} else {
		printf("---\nLine %d in %s\n%s\n---\n", 0, file_name, error_string);
	}
	longjmp(env, 1);
}

void warning(char *warning_string){
	if(current_expression){
		printf("---\nLine %d in %s\n%s\n---\n", current_expression->line_num, file_name, warning_string);
	} else {
		printf("---\nLine %d in %s\n%s\n---\n", 0, file_name, warning_string);
	}
}

void free_variable(void *value){
	datavalue **var_pointer;
	if(!value){
		return;
	}
	var_pointer = (datavalue **) value;
	discard_data(*var_pointer);
	free(var_pointer);
}

void free_operation_list(void *value){
	operation **operation_list;
	unsigned int i;
	operation_list = (operation **) value;
	for(i = 0; i < next_datatype; i++){
		free(operation_list[i]);
	}
	free(operation_list);
}

void quit(){
	unsigned int i;
	if(line){
		free(line);
	}
	if(program){
		free(program);
	}
	if(built_program){
		free_expression(built_program);
	}
	if(parsed_program){
		free_tokens(parsed_program);
	}
	if(globals){
		dictionary_free(globals, free_variable);
	}
	if(operators){
		dictionary_free(operators, free_operation_list);
	}
	for(i = 0; i < next_datatype; i++){
		if(free_datatypes[i]){
			free(free_datatypes[i]);
		}
	}
	exit(0);
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
	program = (char *) 0;
	line = (char *) 0;
	srand(time(NULL));
	next_datatype = 6;
	current_line = 0;
	file_name = "initialization";
	current_expression = (expression *) 0;
	globals = malloc(sizeof(dictionary));
	operators = malloc(sizeof(dictionary));
	free_datatypes = malloc(sizeof(hollow_list));
	*globals = dictionary_create();
	*operators = dictionary_create();
	INCLUDE_DATATYPES();
	create_free_list();
	INCLUDE();
	if(argc < 2){
		file_name = "console";
		current_line = 0;
		printf("SoftScript ver 1.0\n\n");
		setjmp(env);
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
				printf("%d\n", output->int_value);
			} else if(output->type == FLOAT_TYPE){
				printf("%lf\n", output->float_value);
			} else if(output->type == STRING_TYPE){
				printf("\"%s\"\n", (char *) output->value);
			}
			
			free(line);
			free_expression(built_program);
			free_tokens(parsed_program);
		}
	} else {
		if(setjmp(env)){
			quit();
		}
		file_name = argv[1];
		current_line = 1;
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
		program = (char *) 0;
		parsed_program = (linked_list *) 0;
		built_program = (expression *) 0;
	}
	quit();
}
