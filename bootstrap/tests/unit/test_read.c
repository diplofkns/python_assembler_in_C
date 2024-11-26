#include "pyas/read.h"
#include "pyas/regexp_char_group.h"
#include "pyas/list.h"
#include "pyas/lex_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <unitest/unitest.h>

int main(){

  list_t l = read("files/regexp_file.txt");
  printf("Liste de définition de lexème associé : \n");
  list_print(l,lex_def_print);
  printf(" \n ");
  list_delete(l,lex_def_delete);

}
