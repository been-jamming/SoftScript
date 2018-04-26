#include "softscript.h"
#include "dictionary.h"
#include "hollow_lists.h"

void FREE_INTEGER(void *value);

datavalue *ADD_INTEGER(datavalue *a, datavalue *b, expression *expr);

void INCLUDE_STANDARD();