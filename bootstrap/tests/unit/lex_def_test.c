#include <stdio.h>
#include <stdlib.h>

#include "pyas/lex_def.h"
#include "pyas/read.h"

int main(){
  list_t lexdf = read("files/regexp_file.txt");

  printf("\n-----------------\n");

  lexdf = list_del_first(lexdf,lex_def_delete);

  printf("L1L\n");
}
