/*
Hollow lists library

by Ben Jones

3/29/17
*/

#ifndef HOLLOW_LISTS_INCLUDED
#define HOLLOW_LISTS_INCLUDED

#include <stdbool.h>

typedef struct hollow_list hollow_list;

struct hollow_list{
	unsigned int size;
	void *value;
	bool *written;
	hollow_list *children;
};

hollow_list hollow_list_create(unsigned int size);

void hollow_list_free(hollow_list *l, void (*free_values)(void *));

void *hollow_list_read(hollow_list *l, unsigned int index);

void hollow_list_write(hollow_list *l, unsigned int index, void *value);

#endif