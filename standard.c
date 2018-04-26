#include "softscript.h"
#include "dictionary.h"
#include "hollow_lists.h"
#include "standard.h"

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
char intstr[] = "int";
char floatstr[] = "float";
char inputstr[] = "input";

//Internal functions

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
	input[inputlen-1] = (char *) 0;
    return input;
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
	} else if(b->type = FLOAT_TYPE){
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
	if(expr->child2->child1){
		if(expr->child2->child1->type != VARIABLE){
			printf("Error: cannot assign to non-variable type %d\n");
			exit(1);
		} else {
			var_pointer = expr->child2->child1->variable;
			discard_data(*var_pointer);
			*var_pointer = b;
			(*var_pointer)->num_references++;
		}
	} else {
		printf("Error: cannot assign to non-variable type %d\n");
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
		}
	} else {
		if(c_true->type != CODE_TYPE){
			printf("Error: function if takes a code object for arguments 2 and 3");
		}
	}
	condition_value = evaluate_expression(condition);
	if(condition_value->type == INTEGER_TYPE){
		if(*((int *) condition_value->value)){
			is_true = true;
		} else {
			is_true = false;
		}
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
	string = read_input();
	output = increment_references(create_string(string));
	free(string);
	return output;
}

void INCLUDE_STANDARD(){
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
	
	create_operation(NONE_TYPE, equalsvalue, EQUALS);
	create_operation(NONE_TYPE, notequalsvalue, NOTEQUALS);
	create_operation(NONE_TYPE, equals, ASSIGN);
	
	create_free_func(NONE_TYPE, FREE_NONE);
	create_free_func(INTEGER_TYPE, FREE_INTEGER);
	create_free_func(FLOAT_TYPE, FREE_FLOAT);
	create_free_func(STRING_TYPE, FREE_STRING);
	
	create_variable(pi, increment_references(create_float(3.1415926535897932)));
	create_variable(estr, increment_references(create_float(2.718281828)));
	create_function(print, PRINT);
	create_function(whilestr, WHILE);
	create_function(ifstr, IF);
	create_function(intstr, INT);
	create_function(floatstr, FLT);
	create_function(inputstr, INPUT);
}