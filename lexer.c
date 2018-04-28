/*
Soft Script Lexer

by Ben Jones
3/17/2018
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "softscript.h"

//The types of tokens

#define true 1
#define false 0

linked_list *create_linked_list(void *value){
	linked_list *output;
	output = malloc(sizeof(linked_list));
	*output = (linked_list) {.value = value, .next = (linked_list *) 0, .previous = (linked_list *) 0};
	return output;
}

linked_list *add_linked_list(linked_list **l, linked_list *element){
	(*l)->next = element;
	element->previous = *l;
	*l = element;
}

token *create_token(void *value, unsigned int type){
	token *output;
	output = malloc(sizeof(token));
	*output = (token) {.value = value, .type = type};
	return output;
}

bool is_a_digit(unsigned char c){
	if('0' <= c && c <= '9'){
		return true;
	} else {
		return false;
	}
}

bool is_a_letter(unsigned char c){
	if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')){
		return true;
	} else {
		return false;
	}
}

int parse_integer(char **c){
	int output;
	bool negate;
	
	output = 0;
	if(**c == '-'){
		negate = true;
		(*c)++;
	} else {
		negate = false;
	}
	while(is_a_digit(**c)){
		output = output*10 + (**c - '0');
		(*c)++;
	}
	if(negate){
		output *= -1;
	}
	return output;
}

double parse_float(char **c){
	double output;
	int int_part;
	double float_part;
	double digit;
	bool negate;
	
	digit = 1;
	float_part = 0;
	if(**c == '-'){
		negate = true;
		(*c)++;
	} else {
		negate = false;
	}
	int_part = parse_integer(c);
	if(**c == '.'){
		(*c)++;
	}
	while(is_a_digit(**c)){
		digit /= 10;
		float_part = float_part + (**c - '0')*digit;
		(*c)++;
	}
	output = int_part + float_part;
	if(negate){
		output *= -1;
	}
	return output;
}

char *parse_variable(char **c){
	char *output;
	unsigned int var_length;
	char *counter;
	unsigned int index;
	
	counter = *c;
	var_length = 0;
	index = 0;
	while(is_a_letter(*counter) || is_a_digit(*counter) || *counter == '_'){
		var_length++;
		counter++;
	}
	
	output = malloc(sizeof(char)*(var_length + 1));
	while(is_a_letter(**c) || is_a_digit(**c) || **c == '_'){
		output[index] = **c;
		index++;
		(*c)++;
	}
	output[var_length] = (char) 0;
	return output;
}

char *parse_string(char **c){
	char *output;
	char *counter;
	unsigned int string_length;
	if(**c != '"'){
		return (char *) 0;
	}
	(*c)++;
	counter = *c;
	string_length = 0;
	while(*counter != '"'){
		string_length++;
		counter++;
	}
	output = malloc(sizeof(char)*(string_length + 1));
	memcpy(output, *c, string_length*sizeof(char));
	output[string_length] = (char) 0;
	*c += string_length + 1;
	return output;
}

char *parse_operator(char **c){
	char *output;
	char *counter;
	unsigned int operator_length;
	counter = *c;
	operator_length = 0;
	while(*counter != ' ' && *counter != '	' && *counter != '{' && *counter != '(' && *counter != '"' && *counter != '\n' && !is_a_digit(*counter) && !is_a_letter(*counter) && *counter != '_' && *counter != (char) 0){
		counter++;
		operator_length++;
	}
	output = malloc(sizeof(char)*(operator_length + 1));
	memcpy(output, *c, operator_length*sizeof(char));
	output[operator_length] = (char) 0;
	*c += operator_length;
	return output;
}

unsigned int number_type(char *c){
	while(is_a_digit(*c)){
		c++;
	}
	if(*c == '.'){
		return FLOAT_TYPE;
	} else {
		return INTEGER_TYPE;
	}
}

linked_list *parse_program(char **c){
	linked_list *output;
	linked_list *original_output;
	int *int_const;
	double *float_const;
	char *string_const;
	bool last_token_operator;
	bool negate;
	last_token_operator = true;
	negate = false;
	output = create_linked_list(create_token((void *) 0, BEGIN));
	original_output = output;
	while(**c != (char) 0){
		if(is_a_digit(**c)){
			if(number_type(*c) == INTEGER_TYPE){
				int_const = malloc(sizeof(int));
				if(!negate){
					*int_const = parse_integer(c);
				} else {
					*int_const = -parse_integer(c);
				}
				add_linked_list(&output, create_linked_list((void *) create_token((void *) int_const, INTEGER)));
			} else {
				float_const = malloc(sizeof(double));
				if(!negate){
					*float_const = parse_float(c);
				} else {
					*float_const = -parse_float(c);
				}
				add_linked_list(&output, create_linked_list((void *) create_token((void *) float_const, FLOAT)));
			}
			last_token_operator = false;
			negate = false;
		} else if(is_a_letter(**c)){
			string_const = parse_variable(c);
			add_linked_list(&output, create_linked_list((void *) create_token((void *) string_const, VARIABLE)));
			last_token_operator = false;
		} else if(**c == '"'){
			string_const = parse_string(c);
			add_linked_list(&output, create_linked_list((void *) create_token((void *) string_const, STRING)));
			last_token_operator = false;
		} else if(**c == '('){
			add_linked_list(&output, create_linked_list((void *) create_token((void *) 0, OPEN_PARENTHESES)));
			(*c)++;
			last_token_operator = true;
		} else if(**c == ')'){
			add_linked_list(&output, create_linked_list((void *) create_token((void *) 0, CLOSE_PARENTHESES)));
			(*c)++;
			last_token_operator = true;
		} else if(**c == '{'){
			add_linked_list(&output, create_linked_list((void *) create_token((void *) 0, OPEN_BRACES)));
			(*c)++;
			last_token_operator = true;
		} else if(**c == '}'){
			add_linked_list(&output, create_linked_list((void *) create_token((void *) 0, CLOSE_BRACES)));
			(*c)++;
			last_token_operator = true;
		} else if(**c == ','){
			add_linked_list(&output, create_linked_list((void *) create_token((void *) 0, COMMA)));
			(*c)++;
			last_token_operator = true;
		} else if(**c == ';'){
			add_linked_list(&output, create_linked_list((void *) create_token((void *) 0, SEMICOLON)));
			(*c)++;
			last_token_operator = true;
		} else if(**c == ' ' || **c == '	' || **c == '\n' || **c == '\r'){
			(*c)++;
		} else {
			string_const = parse_operator(c);
			if(last_token_operator && string_const[0] == '-' && string_const[1] == (char) 0){
				free(string_const);
				negate = true;
			} else {
				add_linked_list(&output, create_linked_list((void *) create_token((void *) string_const, OPERATOR)));
				last_token_operator = true;
			}
		}
	}
	return original_output;
}

void free_tokens(linked_list *l){
	token current_token;
	linked_list *next;
	while(l){
		current_token = *((token *) l->value);
		if(current_token.value != (void *) 0){
			free(current_token.value);
		}
		next = l->next;
		free(l);
		l = next;
	}
}