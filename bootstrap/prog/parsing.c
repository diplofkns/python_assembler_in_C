#include <stdlib.h>
#include <stdio.h>
#include "pyas/chargroup.h"
#include "pyas/list.h"
#include "pyas/regexp_char_group.h"

/*un programme qui lit (parse)une expression régulière passée
en argument sur la ligne de commande et affiche sa structure*/

/*verification fichier.pys*/

int main (int argc, char * argv []){

  if ( argc < 2 ) {
    fprintf( stderr, "Usage :\n\t%s regexp\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }
  unsigned char* exp = argv[1] ;
  /*char* exp;
  scanf("%s\n",&exp );*/
  /*dans le cas où elle est bien écrite*/
  list_t t = list_new();
  list_t p = list_new();
  t = char_group_parse(exp);
  p = t;
  char_group* c = NULL;
  //list_print(p,char_group_print) ;
  while( !list_empty(p)){
    c = list_first(p) ;
    int i;
    if(c->group[0]==1){
      printf("One not in \"");
      for(i=0;i<256;i++){
        if(c->group[i]==0){
          printf("%c",i);
        }
      }
    } else {
      printf("One in \"");
      for(i=0;i<256;i++){
        if(c->group[i]==1){
          printf("%c",i);
        }
      }
    }
    printf("\"");
    switch(c->op ){
      case(0):
        printf(", one time.\n");
        break;
      case(1):
        printf(", zero or more times.\n");
        break;
      case(2):
        printf(", one or more times.\n");
        break;
      case(3):
        printf(", zero or one time.\n" );
        break;
    }
    // printf("\n");
    // list_print(p,char_group_print);
    p = list_del_first(p,char_group_delete_cb);
  }

  list_delete(p, char_group_delete_cb);

  exit(EXIT_SUCCESS);
}
