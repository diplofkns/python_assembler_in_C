#include <stdio.h>
#include <stdlib.h>

#include "pyas/parse_py.h"

int main ( int argc, char *argv[] ) {
  pyobj_t p = pyobj_parse(NULL,"files/regexp_file_pyt3.txt") ;

  list_t l = list_new();
  l = list_add_first(l,lexem_new("","",0,0));

  exit(EXIT_SUCCESS);
}
