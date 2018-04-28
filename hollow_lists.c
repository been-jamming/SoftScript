/*
Hollow lists

ie. representation of a balanced binary tree

By Benjamin Jones
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "hollow_lists.h"

//Creates a hollow list and allocates all of the needed memory
hollow_list hollow_list_create(unsigned int size){
	hollow_list output;
	output = (hollow_list) {.size = size, .value = (void *) 0, .written = calloc(size, sizeof(bool)), .children = calloc(size, sizeof(hollow_list))};
	return output;
}

//Frees all memory of associated with a hollow list and its children
void hollow_list_free(hollow_list *l, void (*free_values)(void *)){
	int i;
	for(i = 0; i < l->size; i++){
		hollow_list_free(l->children + i, free_values);
	}
	(*free_values)(l->value);
	free(l);
}

//Reads from the hollow list and returns a pointer to the item's data
void *hollow_list_read(hollow_list *l, unsigned int index){
	if(index == 0){
		return l->value;
	}
	unsigned int bit_checker;
	bit_checker = 1<<(l->size - 1);
	int i;
	for(i = 0; i < l->size; i++){
		if(bit_checker & index){
			if(l->written[i] == true){
				return hollow_list_read(l->children + i, bit_checker ^ index);
			} else {
				return (void *) 0;
			}
		}
		bit_checker >>= 1;
	}
}

//Writes to the hollow list, allocating memory only as it needs
void hollow_list_write(hollow_list *l, unsigned int index, void *value){
	if(index == 0){
		l->value = value;
	} else {
		unsigned int bit_checker;
		bit_checker = 1<<(l->size - 1);
		int i;
		for(i = 0; i < l->size; i++){
			if(bit_checker & index){
				if(!l->written[i]){
					l->children[i] = hollow_list_create(l->size - i - 1);
					l->written[i] = true;
				}
				hollow_list_write(l->children + i, bit_checker ^ index, value);
				break;
			}
			bit_checker >>= 1;
		}
	}
}