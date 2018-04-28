/*
Python dictionarys for c

ie. lists that can be indexed by strings

by Ben Jones

3/29/17
*/

#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"

dictionary dictionary_create(){
	dictionary output;
	output.child = malloc(sizeof(hollow_list));
	*output.child = hollow_list_create(8);
	output.value = (void *) 0;
	return output;
}

void dictionary_write(dictionary *dict, char *index, void *value){
	void *hollow_list_value;
	dictionary *new_dict;
	while(*index != (char) 0){
		hollow_list_value = hollow_list_read(dict->child, (int) *index);
		if(hollow_list_value == (void *) 0){
			new_dict = malloc(sizeof(dictionary));
			*new_dict = dictionary_create();
			hollow_list_write(dict->child, (int) *index, new_dict);
			dict = new_dict;
		} else {
			dict = (dictionary *) hollow_list_value;
		}
		index += 1;
	}
	dict->value = value;
}

void *dictionary_read(dictionary *dict, char *index){
	void *hollow_list_value;
	dictionary *new_dict;
	while(*index != (char) 0){
		hollow_list_value = hollow_list_read(dict->child, (int) *index);
		if(hollow_list_value == (void *) 0){
			return hollow_list_value;
		} else {
			dict = (dictionary *) hollow_list_value;
		}
		index += 1;
	}
	return dict->value;
}
