/*
SoftScript expressions

These are expressions which are compiled into an AST but not yet dereferenced.

by Ben Jones
4/9/2018
*/

#include <stdlib.h>
#include <stdio.h>
#include "softscript.h"
#include "hollow_lists.h"
#include "dictionary.h"

expression *create_expression(){
	expression *output;
	output = malloc(sizeof(expression));
	output->args = (expression **) 0;
	output->child1 = (expression *) 0;
	output->child2 = (expression *) 0;
	output->parent = (expression *) 0;
	output->num_args = 0;
	output->type = NONE;
	output->constant = (datavalue *) 0;
	output->variable = (datavalue **) 0;
	output->operators = (operation **) 0;
	output->root_node = false;
	output->top_node = true;
	output->begin = false;
	output->output = increment_references(create_nonetype());
	output->line_num = 0;
	return output;
}

void free_expression(expression *expr){
	unsigned int i;
	if(expr->child1){
		free_expression(expr->child1);
	}
	if(expr->child2){
		free_expression(expr->child2);
	}
	if(expr->constant){
		discard_data(expr->constant);
	}
	if(expr->output){
		discard_data(expr->output);
	}
	for(i = 0; i < expr->num_args; i++){
		free_expression(expr->args[i]);
	}
	free(expr);
}

expression *int_const_expression(token t){
	expression *output;
	output = create_expression();
	output->type = INT_CONST;
	output->constant = increment_references(create_integer(*((int *) t.value)));
	output->root_node = true;
	return output;
}

expression *float_const_expression(token t){
	expression *output;
	output = create_expression();
	output->type = FLOAT_CONST;
	output->constant = increment_references(create_float(*((double *) t.value)));
	output->root_node = true;
	return output;
}

expression *string_const_expression(token t){
	expression *output;
	output = create_expression();
	output->type = STRING_CONST;
	output->constant = increment_references(create_string((char *) t.value));
	output->root_node = true;
	return output;
}

expression *variable_expression(token t){
	expression *output;
	output = create_expression();
	output->type = VARIABLE;
	output->variable = dictionary_read(globals, t.value);
	if(output->variable == (datavalue **) 0){
		create_variable(t.value, create_nonetype());
		output->variable = dictionary_read(globals, t.value);
	}
	output->root_node = true;
	return output;
}

void add_expression(expression **output){
	expression *new_expression;
	new_expression = create_expression();
	new_expression->child2 = *output;
	new_expression->top_node = true;
	(*output)->top_node = false;
	(*output)->parent = new_expression;
	*output = new_expression;
}

void function_call_expression(linked_list **tokens, expression **output){
	unsigned int num_args;
	unsigned int i;
	unsigned int parentheses_count;
	unsigned int braces_count;
	linked_list *temp_tokens;
	expression **args;
	token current_token;
	temp_tokens = *tokens;
	current_token = *((token *) temp_tokens->value);
	parentheses_count = 0;
	braces_count = 0;
	if(current_token.type == CLOSE_PARENTHESES){
		num_args = 0;
	} else {
		num_args = 1;
		while(current_token.type != CLOSE_PARENTHESES || parentheses_count || braces_count){
			if(current_token.type == OPEN_PARENTHESES){
				parentheses_count++;
			} else if(current_token.type == CLOSE_PARENTHESES){
				parentheses_count--;
			} else if(current_token.type == OPEN_BRACES){
				braces_count++;
			} else if(current_token.type == CLOSE_BRACES){
				braces_count--;
			} else if(current_token.type == COMMA && !parentheses_count && !braces_count){
				num_args++;
			}
			temp_tokens = temp_tokens->next;
			current_token = *((token *) temp_tokens->value);
		}
	}
	args = malloc(sizeof(expression *)*num_args);
	for(i = 0; i < num_args; i++){
		args[i] = build_expression(tokens);
		args[i]->parent = *output;
	}
	if(num_args == 0){
		*tokens = (*tokens)->next;
	}
	(*output)->args = args;
	(*output)->num_args = num_args;
	add_expression(output);
}

expression *code_const_expression(linked_list **tokens){
	unsigned int num_expressions;
	unsigned int i;
	unsigned int braces_count;
	linked_list *temp_tokens;
	expression *output;
	datavalue *c;
	token current_token;
	temp_tokens = *tokens;
	current_token = *((token *) temp_tokens->value);
	num_expressions = 1;
	braces_count = 0;
	while(current_token.type != CLOSE_BRACES || braces_count){
		if(current_token.type == OPEN_BRACES){
			braces_count++;
		} else if(current_token.type == CLOSE_BRACES){
			braces_count--;
		} else if(current_token.type == SEMICOLON){
			num_expressions++;
		}
		temp_tokens = temp_tokens->next;
		current_token = *((token *) temp_tokens->value);
	}
	c = increment_references(create_code(num_expressions));
	output = create_expression();
	for(i = 0; i < num_expressions; i++){
		((code *) c->value)->expressions[i] = build_expression(tokens);
		((code *) c->value)->expressions[i]->parent = output;
	}
	output->constant = c;
	output->root_node = true;
	return output;
}

expression *build_expression(linked_list **tokens){
	expression *output;
	token current_token;
	current_token.type = NONE;
	output = create_expression();
	output->begin = true;
	output->line_num = current_line;
	current_expression = output;
	while(*tokens != (linked_list *) 0 && current_token.type != CLOSE_PARENTHESES && current_token.type != COMMA && current_token.type != CLOSE_BRACES && current_token.type != SEMICOLON){
		current_token = *((token *) (*tokens)->value);
		if(current_token.type == INTEGER){
			output->child1 = int_const_expression(current_token);
			add_expression(&output);
		} else if(current_token.type == FLOAT){
			output->child1 = float_const_expression(current_token);
			add_expression(&output);
		} else if(current_token.type == STRING){
			output->child1 = string_const_expression(current_token);
			add_expression(&output);
		} else if(current_token.type == VARIABLE){
			output->child1 = variable_expression(current_token);
			add_expression(&output);
		} else if(current_token.type == OPERATOR && output){
			if(output->operators){
				add_expression(&output);
			}
			output->operators = (operation **) dictionary_read(operators, (char *) current_token.value);
			if(output->operators == (operation **) 0){
				error("Error: Unknown operation");
			}
		} else if(current_token.type == OPEN_PARENTHESES && (output->operators || output->begin)){
			*tokens = (*tokens)->next;
			output->child1 = build_expression(tokens);
			add_expression(&output);
		} else if(current_token.type == OPEN_PARENTHESES){
			*tokens = (*tokens)->next;
			function_call_expression(tokens, &output);
		} else if(current_token.type == OPEN_BRACES){
			*tokens = (*tokens)->next;
			output->child1 = code_const_expression(tokens);
			add_expression(&output);
		} else if(current_token.type == NEW_LINE){
			current_line++;
		}
		if(*tokens != (linked_list *) 0 && current_token.type != OPEN_PARENTHESES && current_token.type != CLOSE_BRACES){
			*tokens = (*tokens)->next;
		}
	}
	return output;
}

datavalue *run_code(code *c){
	unsigned int i;
	datavalue *output;
	for(i = 0; i < c->num_expressions; i++){
		output = evaluate_expression(c->expressions[i]);
	}
	return output;
}

datavalue *evaluate_expression(expression *expr){
	datavalue *child1_value;
	datavalue *child2_value;
	function_value *f;
	operation *oper;
	datavalue *new_output;
	current_expression = expr;
	if(expr->begin && expr->child1){
		expr = expr->child1;
	}
	if(expr->root_node){
		if(expr->variable){
			return *(expr->variable);
		} else {
			return expr->constant;
		}
	} else if(expr->args){
		child2_value = evaluate_expression(expr->child2);
		if(child2_value->type == FUNCTION_TYPE){
			f = (function_value *) child2_value->value;
			new_output = f->function(expr->args, expr->num_args);
			discard_data(expr->output);
			expr->output = new_output;
			return expr->output;
		} else if(child2_value->type == CODE_TYPE){
			new_output = run_code((code *) child2_value->value);
			discard_data(expr->output);
			expr->output = new_output;
			return expr->output;
		} else {
			error("Error: Cannot call non-function type");
		}
	} else if(expr->operators){
		child2_value = evaluate_expression(expr->child2);
		oper = expr->operators[child2_value->type];
		if(oper == (operation *) 0){
			oper = expr->operators[NONE_TYPE];
		}
		if(oper != (operation *) 0){
			if(oper->type == BINARY){
				new_output = oper->function(child2_value, evaluate_expression(expr->child1), expr);
				discard_data(expr->output);
				expr->output = new_output;
				return expr->output;
			} else if(oper->type == UNARY){
				new_output = oper->function(child2_value, (datavalue *) 0, expr);
				discard_data(expr->output);
				expr->output = new_output;
				return expr->output;
			}
		} else {
			error("Error: Invalid operation on datatype");
		}
	} else if(expr->child2){
		return evaluate_expression(expr->child2);
	} else {
		new_output = increment_references(create_nonetype());
		discard_data(expr->output);
		expr->output = new_output;
		return expr->output;
	}
}
