#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pyas/parse.h"
#include "pyas/lexem.h"
#include "pyas/list.h"

int main ( int argc, char *argv[] ) {
  lexem_t l1 = lexem_new("blank"," ",0,0) ;
  lexem_t l2 = lexem_new("newline"," ",0,0) ;
  lexem_t l3 = lexem_new("comment","#atouwenyababadidibabasitiwuwenyaamawenyaama",0,0) ;

  list_t l = list_new();

  l = list_add_first(l, l1) ;
  l = list_add_first(l, l1) ;
  l = list_add_first(l, l3) ;
  l = list_add_first(l, l1) ;
  l = list_add_first(l, l2) ;
  l = list_add_first(l, l3) ;
  l = list_add_first(l, l1) ;

  // printf("Liste : \n");
  // list_print(l,lexem_print);
  // printf("\n");
  // printf("\n");
  //
  // lexem_print(lexem_peek(&l)) ;
  // printf("\n");
  // lexem_advance(&l) ;
  // lexem_print(lexem_peek(&l)) ;
  // printf("\n");
  // lexem_advance(&l) ;
  //
  // printf("%d\n", next_lexem_is(&l,"newline")) ;
  //
  // print_parse_error("AAAAAAAAAAAAAAAAAHHHHHHHH\n", &l);


  printf("%d\n", eol(&l));
  list_print(l,lexem_print);
  
  exit(EXIT_SUCCESS);
}
