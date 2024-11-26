#include <stdio.h>
#include <stdlib.h>
#include "pyas/regexp_char_group.h"
#include "pyas/list.h"
#include "pyas/chargroup.h"


int main (int argc, char * argv []){
  if ( argc < 3 ) {
    fprintf( stderr, "Usage :\n\t%s regexp text\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }

  char* end = argv[2] ;
  list_t l = char_group_parse(argv[1]);
  int is_match = re_match(l, argv[2], &end) ;
  list_delete(l,char_group_delete_cb);

  if (is_match) {
    printf( "The start of '%s' is %s, %s: '%s'.\n", argv[2], argv[ 1 ], *end ? "next" : "END", end );
  } else {
    printf( "The start of '%s' is *NOT* %s.\n", argv[2], argv[ 1 ] );
  }


  exit(EXIT_SUCCESS) ;
}
