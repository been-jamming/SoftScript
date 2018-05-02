#ifndef DICTIONARIES_INCLUDED
#define DICTIONARIES_INCLUDED

#include "hollow_lists.h"

typedef struct dictionary dictionary;

struct dictionary{
	void *value;
	hollow_list *child;
};

dictionary dictionary_create();

void _dictionary_free(void *value);

void dictionary_free(dictionary *dict, void (*free_func)(void *));

void dictionary_write(dictionary *dict, char *index, void *value);

void *dictionary_read(dictionary *dict, char *index);

#endif
