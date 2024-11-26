#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "pyas/read.h"
#include "pyas/chargroup.h" //char_group_parse

list_t read(char *regexp_file){

  FILE* f;
  f = fopen(regexp_file, "r");
  if(f==NULL) {
    perror("Erreur d'ouverture du fichier de définition des lexèmes") ;
    exit(EXIT_FAILURE);
  }

  char* source_orig = calloc(MAX_LENGTH2, sizeof(char));
  char* source = NULL;
  char* end = NULL;
  char *end_nb = NULL;
  queue_t lexdef_q = queue_new();
  char str[3];
  str[0] = 10; str[1] = 0; str[2] = '\0';
  int line = 1;
  while(!feof(f)){
    // printf("Ligne %d: ", line);
    fgets(source_orig, MAX_LENGTH2, f);
    //printf("%s\n", source_orig);

    source = source_orig;
    end = source ;
    /*récupération des commentaires*/
    if(re_match(char_group_parse("#^\n*"), source, &end)||re_match(char_group_parse(str), source, &end)){
      ;

    } else {

      /*récupération du type*/
      if(!re_match(char_group_parse("^[ \t]*"), source, &end)){
        printf("1ERREUR FICHIER DEF\n");
        exit(EXIT_FAILURE);
      }
      char* type = calloc(end-source+1,sizeof(char));
      int i;
      for(i=0;i<end-source;i++) *(type+i) = *(source+i) ;
      *(type+(end-source)) = '\0';
      //printf("type : %s\n",type);

      /*récupération de la regexp qui sera transformée en chargroup*/
      source = end ;
      if(!re_match(char_group_parse("[ \t]+"), source, &end)){
        printf("2ERREUR FICHIER DEF\n");
        exit(EXIT_FAILURE);
      }

      source = end;
      // char str2[4];
      // str2[0] = '^'; str2[1] = 13; str2[2] = '*' ; str2[3] = '\0' ; // ^(caractereascii=13)*\0"
      // if(!re_match(char_group_parse(str2), source, &end)){
      //   printf("3ERREUR FICHIER DEF\n");
      //   exit(EXIT_FAILURE);
      // }

      if(!re_match(char_group_parse("^[ \t\n]+"), source, &end)){
        printf("4ERREUR FICHIER DEF\n");
        exit(EXIT_FAILURE);
      }
      char* regexp = calloc(end-source+1, sizeof(char)) ;
      int j;
      for(j=0;j<end-source;j++) regexp[j] = source[j] ;
      regexp[end-source] = '\0' ;
      //printf("regexp : %s\n",regexp);
      // printf("%s\n",regexp);
      list_t list_chargroup = char_group_parse(regexp);
      free(regexp);

      /*récupération de l'opcode qui sera transformé en nombre*/
      source = end;
      re_match(char_group_parse("\n?"), source,&end);
      source = end;
      if(*end == '\0'){
        char *opcode = "NULL";
        int opcode_nb = atoi(opcode);
        lex_def_t lexdf = lex_def_new(type,list_chargroup,opcode_nb);
        free(type);
        lexdef_q = enqueue(lexdf, lexdef_q) ;
      }
      else {
        if(!re_match(char_group_parse("[ \t]+"), source, &end)){
          printf("5ERREUR FICHIER DEF\n");
          exit(EXIT_FAILURE);
        }

        source = end;
        // if(!re_match(char_group_parse(str2), source, &end)){
        //   printf("5ERREUR FICHIER DEF\n");
        //   exit(EXIT_FAILURE);
        // }

        if(!re_match(char_group_parse("^[ \t]+"), source, &end)){
          printf("6ERREUR FICHIER DEF\n");
          exit(EXIT_FAILURE);
        }
        char* opcode = calloc(end-source+1,sizeof(char));
        int k;
        for(k=0;k<end-source;k++) *(opcode+k) = *(source+k) ;
        *(opcode + (end-source)) = '\0';
        //printf("opcode : %s\n",opcode);
        int opcode_nb = strtol(opcode,end_nb,0); //pour transformer l'opcode en nombre
        //printf("opcode_nb : %d", opcode_nb);

        lex_def_t lexdf = lex_def_new(type,list_chargroup,opcode_nb);
        free(type);
        free(opcode);
        lexdef_q = enqueue(lexdf, lexdef_q) ;
      }
    }

    line++;
  }

  /*ajout de blank*/
  char *type = "blank";
  char *regexp = "[ \t]+";
  list_t list_chargroup = char_group_parse(regexp);
  char *opcode = "NULL";
  int opcode_nb = strtol(opcode,end_nb,0);
  lex_def_t lexdf = lex_def_new(type,list_chargroup,opcode_nb);
  lexdef_q = enqueue(lexdf, lexdef_q) ;

  free(source_orig);
  fclose(f);

  list_t lexdef = queue_to_list(lexdef_q);

  return lexdef ;
}
