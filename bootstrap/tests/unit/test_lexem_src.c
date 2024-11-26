
#include "pyas/lexem_src.h"
#include "pyas/regexp_char_group.h"
#include "pyas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unitest/unitest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main ( int argc, char *argv[] ) {
  list_t lex_src = lex("files/source_file.txt", "files/regexp_file.txt");
  list_print(lex_src,lexem_print);
  list_delete(lex_src,lexem_delete);
  // char* source = ".set version_pyvm 62211\n";
  // char* source_init = source;
  //
  // list_t lex_list = NULL ;
  // list_t def_lexem = read() ;
  // list_t def_lexem_init = def_lexem; //Sauvegarde de la liste de définition des lexèmes
  //
  // int line = 1 ;
  // int column = 0;
  // char* end = NULL;
  //
  // while(*source!='\n'&&*source!=0){
  //   lexem_t lex_rgxp = list_first(def_lexem) ;
  //   while(!re_match(lex_rgxp->value, source, &end)){
  //     if(list_empty(list_next(def_lexem))){
  //       printf("Erreur, impossible de déterminer le type du lexème.\n");
  //       exit(EXIT_FAILURE);
  //     }
  //     def_lexem = list_next(def_lexem) ;
  //     lex_rgxp = list_first(def_lexem);
  //   }
  //   column = source - source_init;
  //   int n = end-source;
  //
  //   char* value = calloc(n+1,sizeof(char));
  //   int i;
  //   for(i=0;i<n;i++) value[i] = source[i] ;
  //   value[n] = 0;
  //
  //   lexem_t lex_src = lexem_new(lex_rgxp->type, value, line, column) ;
  //
  //   lex_list = list_add_first(lex_list, lex_src) ;
  //   source = end;
  //   def_lexem = def_lexem_init;
  //   lex_rgxp = list_first(def_lexem) ;
  // }
  // if(*source == '\n'){
  //   lexem_t lexem = lexem_new("newline", "\n", line, source-source_init) ;
  //   lex_list = list_add_first(lex_list, lexem);
  //   def_lexem = def_lexem_init;
  //   line ++ ;
  // }
  //
  // list_print(lex_list,lexem_print);
}
