#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "pyas/lexem_src.h"

/* Fonction lex:

*/

list_t lex (char *source_file, char *regexp_file) {

FILE* f = fopen("tests/banque_py/pys/test1.pys", "r");

char* source_orig = calloc(500, sizeof(char));
char* source = NULL;

queue_t lex_queue = queue_new() ;
list_t def_lexem = read(regexp_file) ;
list_t def_lexem_init = def_lexem; //Sauvegarde de la liste de définition des lexèmes


int line = 1 ;

while(!feof(f)){
  fgets(source_orig,500,f) ;
  source = source_orig;
    /*on verifie que toute la ligne a été lue*/
    /*if (strlen(source) == 2047)
    {
      printf("Data overload. Line exceed memory place") ;
      exit(EXIT_FAILURE) ;
    }*/
    char *end = NULL ;

    int column = 0;

    while(*source!='\n'&&*source!=0){
      lex_def_t lex_rgxp = list_first(def_lexem) ;
      while(!re_match(lex_rgxp->value, source, &end)){
        if(list_empty(list_next(def_lexem))){
          queue_print(lex_queue, lexem_print);
          printf("Erreur, impossible de déterminer le type du lexème : line %d %s\n", line, source);
          exit(EXIT_FAILURE);
        }
        def_lexem = list_next(def_lexem) ;
        lex_rgxp = list_first(def_lexem);
      }
      column = source - source_orig;
      int n = end-source;

      char* value = calloc(n+1,sizeof(char));
      int i;
      for(i=0;i<n;i++) value[i] = source[i] ;
      value[n] = 0;

      lexem_t lex_src = lexem_new(lex_rgxp->type, value, line, column) ;
      free(value);

      lex_queue = enqueue(lex_src,lex_queue) ;
      source = end;
      def_lexem = def_lexem_init;
      lex_rgxp = list_first(def_lexem) ;
    }
    //if(*source == '\n'){
    lexem_t lexem = lexem_new("newline", "\n", line, source-source_orig) ;
    lex_queue = enqueue(lexem,lex_queue);
    def_lexem = def_lexem_init;
    line ++ ;
    //}
  }
  list_delete(def_lexem,lexem_delete);
  free(source_orig);
  fclose(f);

  list_t list_lex = queue_to_list(lex_queue) ;

  return list_lex;
}
