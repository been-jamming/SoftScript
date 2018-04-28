#include "softscript.h"
#include "dictionary.h"
#include "hollow_lists.h"
#include "standard.h"
#include <string.h>

char plus[] = "+";
char minus[] = "-";
char multiply[] = "*";
char divide[] = "/";
char pi[] = "pi";
char estr[] = "e";
char test[] = "test";
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
char colonstr[] = ":";
char arraystr[] = "array";
char setarraystr[] = "set_array";
char typestr[] = "type";
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

hollow_list *assign_operators;

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
	free(value);
}

void FREE_FLOAT(void *value){
	free(value);
}

void FREE_STRING(void *value){
	free(value);
}

void FREE_FUNCTION(void *value){
	free(value);
	return;
}

void FREE_CODE(void *value){
	code *c;
	unsigned int i;
	c = (code *) value;
	/*for(i = 0; i < c->num_expressions; i++){
		free_expression(c->expressions[i]);
	}*/
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

//SoftScript operations

datavalue *ADD_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer(*((int *) a->value) + *((int *) b->value)));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(*((int *) a->value) + *((double *) b->value)));
	} else {
		return increment_references(create_nonetype());
	}
}

datavalue *SUBTRACT_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer(*((int *) a->value) - *((int *) b->value)));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(*((int *) a->value) - *((double *) b->value)));
	} else {
		return increment_references(create_integer(0));
	}
}

datavalue *MULTIPLY_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer(*((int *) a->value)*(*((int *) b->value))));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(*((int *) a->value)*(*((double *) b->value))));
	} else {
		return increment_references(create_integer(1));
	}
}

datavalue *DIVIDE_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer(*((int *) a->value)/(*((int *) b->value))));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(*((int *) a->value)/(*((double *) b->value))));
	} else {
		return increment_references(create_integer(1));
	}
}

datavalue *LESSTHAN_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (*((int *) a->value) < *((int *) b->value))));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (*((int *) a->value) < *((double *) b->value))));
	} else {
		return increment_references(create_integer(0));
	}
}

datavalue *GREATERTHAN_INTEGER(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (*((int *) a->value) > *((int *) b->value))));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (*((int *) a->value) > *((double *) b->value))));
	} else {
		return increment_references(create_integer(0));
	}
}

datavalue *LESSTHAN_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (*((double *) a->value) < *((int *) b->value))));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (*((double *) a->value) < *((double *) b->value))));
	} else {
		return increment_references(create_integer(0));
	}
}

datavalue *GREATERTHAN_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (*((double *) a->value) > *((int *) b->value))));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (*((double *) a->value) > *((double *) b->value))));
	} else {
		return increment_references(create_integer(0));
	}
}

datavalue *ADD_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_float(*((double *) a->value) + *((int *) b->value)));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(*((double *) a->value) + *((double *) b->value)));
	} else {
		return increment_references(create_float(0));
	}
}

datavalue *SUBTRACT_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_float(*((double *) a->value) - *((int *) b->value)));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(*((double *) a->value) - *((double *) b->value)));
	} else {
		return increment_references(create_float(0));
	}
}

datavalue *MULTIPLY_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_float(*((double *) a->value)*(*((int *) b->value))));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float(*((double *) a->value)*(*((double *) b->value))));
	} else {
		return increment_references(create_integer(1));
	}
}

datavalue *DIVIDE_FLOAT(datavalue *a, datavalue *b, expression *expr){
	if(b->type == INTEGER_TYPE){
		return increment_references(create_float((*((double *) a->value))/(*((int *) b->value))));
	} else if(b->type == FLOAT_TYPE){
		return increment_references(create_float((*((double *) a->value))/(*((double *) b->value))));
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
			printf("Error: cannot assign to non-variable type %d\n", expr->child2->child1->type);
			exit(1);
		} else {
			var_pointer = expr->child2->child1->variable;
			discard_data(*var_pointer);
			*var_pointer = b;
			increment_references(b);
		}
	} else {
		printf("Error: cannot assign to non-variable type\n");
		exit(1);
	}
	return increment_references(b);
}

datavalue *EQUALS(datavalue *a, datavalue *b, expression *expr){
	if(a->type == INTEGER_TYPE && b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (*((int *) a->value) == *((int *) b->value))));
	} else if(a->type == INTEGER_TYPE && b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (*((int *) a->value) == *((double *) b->value))));
	} else if(a->type == FLOAT_TYPE && b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (*((double *) a->value) == *((int *) b->value))));
	} else if(a->type == FLOAT_TYPE && b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (*((double *) a->value) == *((double *) b->value))));
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
		return increment_references(create_integer((int) (*((int *) a->value) != *((int *) b->value))));
	} else if(a->type == INTEGER_TYPE && b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (*((int *) a->value) != *((double *) b->value))));
	} else if(a->type == FLOAT_TYPE && b->type == INTEGER_TYPE){
		return increment_references(create_integer((int) (*((double *) a->value) != *((int *) b->value))));
	} else if(a->type == FLOAT_TYPE && b->type == FLOAT_TYPE){
		return increment_references(create_integer((int) (*((double *) a->value) != *((double *) b->value))));
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
	if(b->type != INTEGER_TYPE){
		printf("Error: array indices must be integers\n");
		exit(1);
	}
	if(expr->parent){
		if(expr->parent->operators == assign_operators){
			return increment_references(create_pointer(((array *) a->value)->values + *((int *) b->value)));
		}
	}
	return increment_references(((array *) a->value)->values[*((int *) b->value)]);
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
			printf("%d", *((int *) d->value));
		} else if(d->type == FLOAT_TYPE){
			printf("%lf", *((double *) d->value));
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
		printf("Error: function if takes two or three arguments\n");
		exit(1);
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
			printf("Error: function if takes a code object for arguments 2 and 3");
			exit(1);
		}
	} else {
		if(c_true->type != CODE_TYPE){
			printf("Error: function if takes a code object for arguments 2 and 3");
			exit(1);
		}
	}
	condition_value = evaluate_expression(condition);
	if(condition_value->type == INTEGER_TYPE){
		if(*((int *) condition_value->value)){
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
		printf("Error: function while takes exactly two arguments\n");
		exit(1);
	}
	condition = e[0];
	loop = e[1];
	c = evaluate_expression(loop);
	if(c->type != CODE_TYPE){
		printf("Error: function while takes a code object for argument number 2\n");
		exit(1);
	}
	cc = (code *) c->value;
	condition_value = evaluate_expression(condition);
	if(condition_value->type == INTEGER_TYPE){
		if(*((int *) condition_value->value)){
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
			if(*((int *) condition_value->value) == 0){
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
		printf("Error: function for takes exactly 4 arguments\n");
		exit(1);
	}
	initialize = e[0];
	condition = e[1];
	after = e[2];
	execute = e[3];
	execute_d = evaluate_expression(execute);
	if(execute_d->type != CODE_TYPE){
		printf("Error: function for takes a code object as the 4th argument\n");
		exit(1);
	}
	c = (code *) execute_d->value;
	evaluate_expression(initialize);
	condition_d = evaluate_expression(condition);
	if(condition_d->type == INTEGER_TYPE){
		if(*((int *) condition_d->value)){
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
			if(*((int *) condition_d->value)){
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
	if(num_args != 1){
		printf("Error: function int takes exactly 1 argument\n");
		exit(1);
	}
	v = evaluate_expression(e[0]);
	if(v->type == INTEGER_TYPE){
		return increment_references(v);
	} else if(v->type == FLOAT_TYPE){
		return increment_references(create_integer((int) *((double *) v->value)));
	} else if(v->type == STRING_TYPE){
		c = (char **) &(v->value);
		return increment_references(create_integer(parse_integer(c)));
	} else {
		return increment_references(create_nonetype());
	}
}

datavalue *FLT(expression **e, unsigned int num_args){
	datavalue *v;
	char **c;
	if(num_args != 1){
		printf("Error: function float takes exactly 1 argument\n");
		exit(1);
	}
	v = evaluate_expression(e[0]);
	if(v->type == INTEGER_TYPE){
		return increment_references(create_float((double) *((int *) v->value)));
	} else if(v->type == FLOAT_TYPE){
		return increment_references(v);
	} else if(v->type == STRING_TYPE){
		c = (char **) &(v->value);
		return increment_references(create_float(parse_float(c)));
	} else {
		return increment_references(create_nonetype());
	}
}

datavalue *INPUT(expression **e, unsigned int num_args){
	datavalue *output;
	char *string;
	if(num_args > 0){
		printf("Error: function input takes no arguments\n");
		exit(1);
	}
	string = malloc(sizeof(char)*256);
	fgets(string, 255, stdin);
	string[strlen(string)-1] = (char) 0;
	output = increment_references(create_string(string));
	return output;
}

datavalue *EXIT(expression **e, unsigned int num_args){
	exit(0);
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
		printf("Error: function array takes only integer arguments\n");
		exit(1);
	}
	length = *((int *) d->value);
	output = create_array(length);
	for(i = 0; i < length; i++){
		((array *) output->value)->values[i] = ARRAY(e + 1, num_args - 1);
	}
	return output;
}

datavalue *SET_ARRAY(expression **e, unsigned int num_args){
	datavalue *output;
	datavalue **array_data;
	datavalue *array_pointer;
	datavalue *set_value;
	array *a;
	unsigned int arg_index;
	unsigned int index;
	if(num_args < 2){
		printf("Error: function set_array takes at least 2 arguments\n");
		exit(1);
	}
	array_pointer = evaluate_expression(e[0]);
	if(array_pointer->type != ARRAY_TYPE){
		printf("Error: function set_array takes an array as the 1st argument\n");
		exit(1);
	}
	a = (array *) array_pointer->value;
	index = *((int *) evaluate_expression(e[1])->value);
	array_data = a->values + index;
	for(arg_index = 2; arg_index < num_args - 1; arg_index++){
		if((*array_data)->type != ARRAY_TYPE){
			printf("Error: invalid array dimensions\n");
			exit(1);
		}
		index = *((int *) evaluate_expression(e[arg_index])->value);
		array_data = ((array *) (*array_data)->value)->values + index;
	}
	set_value = evaluate_expression(e[arg_index]);
	discard_data(*array_data);
	*array_data = set_value;
	increment_references(*array_data);
	return increment_references(create_nonetype());
}

datavalue *TYPE(expression **e, unsigned int num_args){
	if(num_args != 1){
		printf("Error: function type takes exactly 1 argument\n");
		exit(1);
	}
	return increment_references(create_integer(evaluate_expression(e[0])->type));
}

void INCLUDE_STANDARD(){
	ARRAY_TYPE = create_datatype();
	POINTER_TYPE = create_datatype();
	
	create_operation(INTEGER_TYPE, plus, ADD_INTEGER);
	create_operation(INTEGER_TYPE, minus, SUBTRACT_INTEGER);
	create_operation(INTEGER_TYPE, multiply, MULTIPLY_INTEGER);
	create_operation(INTEGER_TYPE, divide, DIVIDE_INTEGER);
	create_operation(FLOAT_TYPE, plus, ADD_FLOAT);
	create_operation(FLOAT_TYPE, minus, SUBTRACT_FLOAT);
	create_operation(FLOAT_TYPE, multiply, MULTIPLY_FLOAT);
	create_operation(FLOAT_TYPE, divide, DIVIDE_FLOAT);
	create_operation(INTEGER_TYPE, lessthan, LESSTHAN_INTEGER);
	create_operation(INTEGER_TYPE, greaterthan, GREATERTHAN_INTEGER);
	create_operation(FLOAT_TYPE, lessthan, LESSTHAN_FLOAT);
	create_operation(FLOAT_TYPE, greaterthan, GREATERTHAN_FLOAT);
	create_operation(ARRAY_TYPE, colonstr, INDEX_ARRAY);
	
	create_operation(NONE_TYPE, equalsvalue, EQUALS);
	create_operation(NONE_TYPE, notequalsvalue, NOTEQUALS);
	create_operation(NONE_TYPE, equals, ASSIGN);
	
	create_free_func(NONE_TYPE, FREE_NONE);
	create_free_func(INTEGER_TYPE, FREE_INTEGER);
	create_free_func(FLOAT_TYPE, FREE_FLOAT);
	create_free_func(STRING_TYPE, FREE_STRING);
	create_free_func(FUNCTION_TYPE, FREE_FUNCTION);
	create_free_func(CODE_TYPE, FREE_CODE);
	create_free_func(ARRAY_TYPE, FREE_ARRAY);
	
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
	create_function(setarraystr, SET_ARRAY);
	create_function(typestr, TYPE);
	
	assign_operators = (hollow_list *) dictionary_read(operators, equals);
}