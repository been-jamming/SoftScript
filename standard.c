#include "softscript.h"
#include "dictionary.h"
#include "hollow_lists.h"
#include "standard.h"
#include <string.h>
#include <stdlib.h>

char plus[] = "+";
char minus[] = "-";
char multiply[] = "*";
char divide[] = "/";
char pi[] = "pi";
char estr[] = "e";
char print[] = "print";
char equals[] = "=";
char lessthan[] = "<";
char greaterthan[] = ">";
char equalsvalue[] = "==";
char notequalsvalue[] = "!=";
char whilestr[] = "while";
char ifstr[] = "if";
char forstr[] = "for";
char intstr[] = "int";
char floatstr[] = "float";
char inputstr[] = "input";
char exitstr[] = "exit";
char openbracketstr[] = "[";
char closebracketstr[] = "]";
char arraystr[] = "array";
char setarraystr[] = "set_array";
char typestr[] = "type";
char randstr[] = "rand";
char nonetypestr[] = "NONE";
char integertypestr[] = "INTEGER";
char floattypestr[] = "FLOAT";
char stringtypestr[] = "STRING";
char functiontypestr[] = "FUNCTION";
char codetypestr[] = "CODE";
char arraytypestr[] = "ARRAY";
char pointertypestr[] = "POINTER";

unsigned int ARRAY_TYPE;
unsigned int POINTER_TYPE;
unsigned int COMPLEX_TYPE;

operation **assign_operators;

//Internal structures

typedef struct array array;

struct array{
	datavalue **values;
	unsigned int length;
};

//Internal functions

datavalue *create_array(unsigned int length){
	array *a;
	datavalue *output;
	a = malloc(sizeof(array));
	a->values = malloc(sizeof(datavalue *)*length);
	a->length = length;
	output = create_datavalue();
	output->type = ARRAY_TYPE;
	output->value = (void *) a;
	return output;
}

datavalue *create_pointer(datavalue **value){
	datavalue *output;
	output = create_datavalue();
	output->type = POINTER_TYPE;
	output->value = (void *) value;
	return output;
}

//SoftScript free functions

void FREE_NONE(void *value){
	return;
}

void FREE_INTEGER(void *value){
	return;
}

void FREE_FLOAT(void *value){
	return;
}

void FREE_STRING(void *value){
	free(value);
}

void FREE_FUNCTION(void *value){
	free(value);
}

void FREE_CODE(void *value){
	code *c;
	unsigned int i;
	c = (code *) value;
	for(i = 0; i < c->num_expressions; i++){
		free_expression(c->expressions[i]);
	}
	free(c);
}

void FREE_ARRAY(void *value){
	array *a;
	unsigned int i;
	a = (array *) value;
	for(i = 0; i < a->length; i++){
		discard_data(a->values[i]);
	}
	free(a->values);
	free(a);
}

void FREE_POINTER(void *value){
	return;
}

//SoftScript operations

datavalue *ADD_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer(a->int_value + b->int_value));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(a->int_value + b->float_value));
	} else {
		return increment_references(create_nonetype());
	}
}

datavalue *SUBTRACT_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer(a->int_value - b->int_value));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(a->int_value - b->float_value));
	} else {
		return increment_references(create_integer(0));
	}
}

datavalue *MULTIPLY_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer(a->int_value*(b->int_value)));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(a->int_value*(b->float_value)));
	} else {
		return increment_references(create_integer(1));
	}
}

datavalue *DIVIDE_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer(a->int_value/b->int_value));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(a->int_value/b->float_value));
	} else {
		return increment_references(create_integer(1));
	}
}

datavalue *LESSTHAN_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (a->int_value < b->int_value)));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (a->float_value < b->float_value)));
	} else {
		return increment_references(create_integer(0));
	}
}

datavalue *GREATERTHAN_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (a->int_value > b->int_value)));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (a->int_value > b->float_value)));
	} else {
		return increment_references(create_integer(0));
	}
}

datavalue *LESSTHAN_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (a->float_value < b->int_value)));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (a->float_value < b->float_value)));
	} else {
		return increment_references(create_integer(0));
	}
}

datavalue *GREATERTHAN_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (a->float_value > b->int_value)));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (a->float_value > b->float_value)));
	} else {
		return increment_references(create_integer(0));
	}
}

datavalue *ADD_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_float(a->float_value + b->int_value));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(a->float_value + b->float_value));
	} else {
		return increment_references(create_float(0));
	}
}

datavalue *SUBTRACT_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_float(a->float_value - b->int_value));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(a->float_value - b->float_value));
	} else {
		return increment_references(create_float(0));
	}
}

datavalue *MULTIPLY_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_float(a->float_value*(b->int_value)));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(a->float_value*(b->float_value)));
	} else {
		return increment_references(create_integer(1));
	}
}

datavalue *DIVIDE_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_float(a->float_value/b->int_value));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(a->float_value/b->float_value));
	} else {
		return increment_references(create_integer(1));
	}
}

datavalue *ASSIGN(datavalue *a, datavalue *b, expression *expr){
	datavalue **var_pointer;
	if(a->type == POINTER_TYPE){
		var_pointer = (datavalue **) a->value;
		discard_data(*var_pointer);
		*var_pointer = b;
		increment_references(b);
	} else if(expr->child2->child1){
		if(expr->child2->child1->type != VARIABLE){
			error("Error: cannot assign to non-variable type");
		} else {
			var_pointer = expr->child2->child1->variable;
			discard_data(*var_pointer);
			*var_pointer = b;
			increment_references(b);
		}
	} else {
		error("Error: cannot assign to non-variable type");
	}
	return increment_references(b);
}

datavalue *EQUALS(datavalue *a, datavalue *b, expression *expr){
	if(a->type == INTEGER_TYPE && b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (a->int_value == b->int_value)));
	} else if(a->type == INTEGER_TYPE && b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (a->int_value == b->float_value)));
	} else if(a->type == FLOAT_TYPE && b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (a->float_value == b->int_value)));
	} else if(a->type == FLOAT_TYPE && b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (a->float_value == b->float_value)));
	} else if(a->type != b->type){
		return increment_references(create_integer(0));
	} else if(a->type == STRING_TYPE && b->type == STRING_TYPE){
		if(strcmp((char *) a->value, (char *) b->value)){
			return increment_references(create_integer(0));
		} else {
			return increment_references(create_integer(1));
		}
	} else {
		return increment_references(create_integer((int) (a->value == b->value)));
	}
}

datavalue *NOTEQUALS(datavalue *a, datavalue *b, expression *expr){
	if(a->type == INTEGER_TYPE && b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (a->int_value != b->int_value)));
	} else if(a->type == INTEGER_TYPE && b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (a->int_value != b->float_value)));
	} else if(a->type == FLOAT_TYPE && b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (a->float_value != b->int_value)));
	} else if(a->type == FLOAT_TYPE && b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (a->float_value != b->float_value)));
	} else if(a->type != b->type){
		return increment_references(create_integer(1));
	} else if(a->type == STRING_TYPE && b->type == STRING_TYPE){
		if(strcmp((char *) a->value, (char *) b->value)){
			return increment_references(create_integer(1));
		} else {
			return increment_references(create_integer(0));
		}
	} else {
		return increment_references(create_integer((int) (a->value != b->value)));
	}
}

datavalue *INDEX_ARRAY(datavalue *a, datavalue *b, expression *expr){
	int index;
	if(b->type != INTEGER_TYPE){
		error("Error: array indices must be integers");
	}
	index = b->int_value;
	if(index >= ((array *) a->value)->length || index < 0){
		error("Error: array index out of range");
	}
	if(expr->parent){
		if(expr->parent->parent){
			if(expr->parent->parent->operators == assign_operators){
				return increment_references(create_pointer(((array *) a->value)->values + index));
			}
		}
	}
	return increment_references(((array *) a->value)->values[index]);
}

datavalue *NO_OPERATION(datavalue *a, datavalue *b, expression *expr){
	return increment_references(a);
}

//SoftScript functions

datavalue *PRINT(expression **e, unsigned int num_args){
	unsigned int i;
	datavalue *d;
	for(i = 0; i < num_args; i++){
		d = evaluate_expression(e[i]);
		if(d->type == STRING_TYPE){
			printf("%s", (char *) d->value);
		} else if(d->type == INTEGER_TYPE){
			printf("%d", d->int_value);
		} else if(d->type == FLOAT_TYPE){
			printf("%lf", d->float_value);
		} else if(d->type == NONE_TYPE){
			printf("NONE");
		}
		printf(" ");
	}
	printf("\n");
	return increment_references(create_nonetype());
}

datavalue *IF(expression **e, unsigned int num_args){
	expression *condition;
	expression *true_code;
	expression *false_code;
	datavalue *condition_value;
	datavalue *c_true;
	datavalue *c_false;
	bool is_true;
	if(num_args != 2 && num_args != 3){
		error("Error: function if takes two or three arguments");
	}
	
	condition = e[0];
	true_code = e[1];
	if(num_args == 3){
		false_code = e[2];
	}
	
	c_true = evaluate_expression(true_code);
	if(num_args == 3){
		c_false = evaluate_expression(false_code);
		if(c_true->type != CODE_TYPE || c_false->type != CODE_TYPE){
			error("Error: function if takes a code object for arguments 2 and 3");
		}
	} else {
		if(c_true->type != CODE_TYPE){
			error("Error: function if takes a code object for arguments 2 and 3");
		}
	}
	condition_value = evaluate_expression(condition);
	if(condition_value->type == INTEGER_TYPE){
		if(condition_value->int_value){
			is_true = true;
		} else {
			is_true = false;
		}
	} else {
		is_true = true;
	}
	if(is_true){
		run_code((code *) c_true->value);
	} else if(num_args == 3){
		run_code((code *) c_false->value);
	}
	return increment_references(create_nonetype());
}

datavalue *WHILE(expression **e, unsigned int num_args){
	expression *condition;
	expression *loop;
	datavalue *condition_value;
	datavalue *c;
	code *cc;
	bool do_loop;
	if(num_args != 2){
		error("Error: function while takes exactly two arguments");
	}
	condition = e[0];
	loop = e[1];
	c = evaluate_expression(loop);
	if(c->type != CODE_TYPE){
		error("Error: function while takes a code object for argument number 2");
	}
	cc = (code *) c->value;
	condition_value = evaluate_expression(condition);
	if(condition_value->type == INTEGER_TYPE){
		if(condition_value->int_value){
			do_loop = true;
		} else {
			do_loop = false;
		}
	} else {
		do_loop = true;
	}
	while(do_loop){
		run_code(cc);
		condition_value = evaluate_expression(condition);
		if(condition_value->type == INTEGER_TYPE){
			if(condition_value->int_value == 0){
				do_loop = false;
			} else {
				do_loop = true;
			}
		} else {
			do_loop = true;
		}
	}
	return increment_references(create_nonetype());
}

datavalue *FOR(expression **e, unsigned int num_args){
	expression *initialize;
	expression *condition;
	expression *after;
	expression *execute;
	datavalue *condition_d;
	datavalue *execute_d;
	code *c;
	bool do_loop;
	if(num_args != 4){
		error("Error: function for takes exactly 4 arguments");
	}
	initialize = e[0];
	condition = e[1];
	after = e[2];
	execute = e[3];
	execute_d = evaluate_expression(execute);
	if(execute_d->type != CODE_TYPE){
		error("Error: function for takes a code object as the 4th argument");
	}
	c = (code *) execute_d->value;
	evaluate_expression(initialize);
	condition_d = evaluate_expression(condition);
	if(condition_d->type == INTEGER_TYPE){
		if(condition_d->int_value){
			do_loop = true;
		} else {
			do_loop = false;
		}
	} else {
		do_loop = true;
	}
	while(do_loop){
		run_code(c);
		evaluate_expression(after);
		condition_d = evaluate_expression(condition);
		if(condition_d->type == INTEGER_TYPE){
			if(condition_d->int_value){
				do_loop = true;
			} else {
				do_loop = false;
			}
		} else {
			do_loop = true;
		}
	}
	return increment_references(create_nonetype());
}

datavalue *INT(expression **e, unsigned int num_args){
	datavalue *v;
	char **c;
	char *cc;
	char *original_cc;
	unsigned int len;
	datavalue *output;
	if(num_args != 1){
		error("Error: function int takes exactly 1 argument");
	}
	v = evaluate_expression(e[0]);
	if(v->type == INTEGER_TYPE){
		return increment_references(v);
	} else if(v->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (v->float_value)));
	} else if(v->type == STRING_TYPE){
		len = strlen((char *) v->value);
		cc = calloc(len + 1, sizeof(char));
		c = &cc;
		original_cc = cc;
		strcpy(cc, (char *) v->value);
		output = increment_references(create_integer(parse_integer(c)));
		free(original_cc);
		return output;
	} else {
		return increment_references(create_nonetype());
	}
}

datavalue *FLT(expression **e, unsigned int num_args){
	datavalue *v;
	char **c;
	char *cc;
	char *original_cc;
	unsigned int len;
	datavalue *output;
	if(num_args != 1){
		error("Error: function float takes exactly 1 argument");
	}
	v = evaluate_expression(e[0]);
	if(v->type == INTEGER_TYPE){
		return increment_references(create_float((double) (v->int_value)));
	} else if(v->type == FLOAT_TYPE){
		return increment_references(v);
	} else if(v->type == STRING_TYPE){
		len = strlen((char *) v->value);
		cc = calloc(len + 1, sizeof(char));
		c = &cc;
		original_cc = cc;
		output = increment_references(create_float(parse_float(c)));
		free(original_cc);
		return output;
	} else {
		return increment_references(create_nonetype());
	}
}

datavalue *INPUT(expression **e, unsigned int num_args){
	datavalue *output;
	char *string;
	unsigned int len;
	if(num_args > 0){
		error("Error: function input takes no arguments");
	}
	string = malloc(sizeof(char)*256);
	fgets(string, 255, stdin);
	len = strlen(string) - 1;
	string[strlen(string)-1] = (char) 0;
	realloc(string, len*sizeof(char));
	output = increment_references(create_string(string));
	return output;
}

datavalue *EXIT(expression **e, unsigned int num_args){
	quit();
}

datavalue *ARRAY(expression **e, unsigned int num_args){
	datavalue *output;
	datavalue *d;
	unsigned int length;
	unsigned int i;
	if(num_args == 0){
		return increment_references(create_nonetype());
	}
	d = evaluate_expression(e[0]);
	if(d->type != INTEGER_TYPE){
		error("Error: function array takes only integer arguments");
	}
	length = d->int_value;
	output = create_array(length);
	for(i = 0; i < length; i++){
		((array *) output->value)->values[i] = ARRAY(e + 1, num_args - 1);
	}
	return increment_references(output);
}

datavalue *TYPE(expression **e, unsigned int num_args){
	if(num_args != 1){
		error("Error: function type takes exactly 1 argument");
	}
	return increment_references(create_integer(evaluate_expression(e[0])->type));
}

datavalue *RAND(expression **e, unsigned int num_args){
	if(num_args > 0){
		error("Error: function rand takes no arguments");
	}
	return increment_references(create_float(((double) rand())/RAND_MAX));
}

void STANDARD_DATATYPES(){
	ARRAY_TYPE = create_datatype();
	POINTER_TYPE = create_datatype();
}

void INCLUDE_STANDARD(){
	create_operation(INTEGER_TYPE, plus, ADD_INTEGER, BINARY, true);
	create_operation(INTEGER_TYPE, minus, SUBTRACT_INTEGER, BINARY, true);
	create_operation(INTEGER_TYPE, multiply, MULTIPLY_INTEGER, BINARY, true);
	create_operation(INTEGER_TYPE, divide, DIVIDE_INTEGER, BINARY, true);
	create_operation(FLOAT_TYPE, plus, ADD_FLOAT, BINARY, true);
	create_operation(FLOAT_TYPE, minus, SUBTRACT_FLOAT, BINARY, true);
	create_operation(FLOAT_TYPE, multiply, MULTIPLY_FLOAT, BINARY, true);
	create_operation(FLOAT_TYPE, divide, DIVIDE_FLOAT, BINARY, true);
	create_operation(INTEGER_TYPE, lessthan, LESSTHAN_INTEGER, BINARY, true);
	create_operation(INTEGER_TYPE, greaterthan, GREATERTHAN_INTEGER, BINARY, true);
	create_operation(FLOAT_TYPE, lessthan, LESSTHAN_FLOAT, BINARY, true);
	create_operation(FLOAT_TYPE, greaterthan, GREATERTHAN_FLOAT, BINARY, true);
	create_operation(ARRAY_TYPE, openbracketstr, INDEX_ARRAY, BINARY, false);
	
	create_operation(NONE_TYPE, equalsvalue, EQUALS, BINARY, true);
	create_operation(NONE_TYPE, notequalsvalue, NOTEQUALS, BINARY, true);
	create_operation(NONE_TYPE, equals, ASSIGN, BINARY, false);
	create_operation(NONE_TYPE, closebracketstr, NO_OPERATION, UNARY, true);
	
	create_free_func(NONE_TYPE, FREE_NONE);
	create_free_func(INTEGER_TYPE, FREE_INTEGER);
	create_free_func(FLOAT_TYPE, FREE_FLOAT);
	create_free_func(STRING_TYPE, FREE_STRING);
	create_free_func(FUNCTION_TYPE, FREE_FUNCTION);
	create_free_func(CODE_TYPE, FREE_CODE);
	create_free_func(ARRAY_TYPE, FREE_ARRAY);
	create_free_func(POINTER_TYPE, FREE_POINTER);
	
	create_variable(pi, increment_references(create_float(3.1415926535897932)));
	create_variable(estr, increment_references(create_float(2.718281828)));
	create_variable(nonetypestr, increment_references(create_integer(NONE_TYPE)));
	create_variable(integertypestr, increment_references(create_integer(INTEGER_TYPE)));
	create_variable(floattypestr, increment_references(create_integer(FLOAT_TYPE)));
	create_variable(stringtypestr, increment_references(create_integer(STRING_TYPE)));
	create_variable(functiontypestr, increment_references(create_integer(FUNCTION_TYPE)));
	create_variable(codetypestr, increment_references(create_integer(CODE_TYPE)));
	create_variable(arraytypestr, increment_references(create_integer(ARRAY_TYPE)));
	create_variable(pointertypestr, increment_references(create_integer(POINTER_TYPE)));
	
	create_function(print, PRINT);
	create_function(whilestr, WHILE);
	create_function(ifstr, IF);
	create_function(forstr, FOR);
	create_function(intstr, INT);
	create_function(floatstr, FLT);
	create_function(inputstr, INPUT);
	create_function(exitstr, EXIT);
	create_function(arraystr, ARRAY);
	create_function(typestr, TYPE);
	create_function(randstr, RAND);
	
	assign_operators = (operation **) dictionary_read(operators, equals);
}
