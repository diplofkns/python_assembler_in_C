#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pyas/parse_py.h"
#include "pyas/parse.h"
#include "pyas/queue.h"

int pyobj_write (FILE* fp, pyobj_t obj);
int pyasm(char* lexdef_file, char* pys, char* pyc);
