#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "pyas/lexem.h"
#include "pyas/lexem_src.h"
#include "pyas/list.h"


int main(){
  list_t lx = list_new();

  lx = lex("files/source_file.txt","files/regexp_file.txt");
  list_print(lx, lexem_print);
  parse(&lx);
  //printf("%d\n", next_lexem_is(&lx,"comment"));

  exit(EXIT_SUCCESS);
}
