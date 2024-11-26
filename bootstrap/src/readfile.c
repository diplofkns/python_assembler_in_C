#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pyas/chargroup.h"
#include "pyas/regexp_char_group.h"
#include "pyas/list.h"
#include "pyas/lexem.h"


list_t read_regexp_file(FILE* regexp_file){

  regexp_file =fopen("regexp_file.txt", "r");
  if (regexp_file== NULL){
    printf("problème à l'ouverture\n");
    exit(EXIT_FAILURE);
  }
  char* source;
  char** end;
  list_t def_lexem = list_new();

  while(regexp_file != NULL){
    source = fgets(source, 250, regexp_file);
    while(re_match("#\\^n*\n\n", source, end )==1 || re_match("\n*", source, end )==1){
      ; //commentaire ou saut de ligne
    }
    re_match( "[a-zA-Z]*[\t]", source, end );  //type

    char* type=NULL; //chaine de caractere de *source à **end ie le type dans le fichier
    type = calloc(**end - *source + 1, sizeof(*type));
    type = fgets(type,**end - *source+1, regexp_file);
    int i;
    for(i=0; i< *end - source +1;i++){
      type[i]=source[i];
    }

    re_match("[\t]*", source, end);
    re_match( "^[\n]", source, end ) ;       //value

    char* value = NULL; //chaine de caractere de *source à **end ie la value dans le fichier
    //value = calloc(**end - *source + 1, sizeof(*value));
    int j;
    value = calloc(**end - *source + 1, sizeof(*value));
    value = fgets(value,**end - *source+1, regexp_file);
    for(j=0; j< **end - *source +1;j++){
      type[j]=source[j]; }

    lexem_t lex = lexem_new(type, value, 0, 0);
    def_lexem = list_add_first(def_lexem, lex);
    free(type);
    free(value);
  }
  return def_lexem;
}
