
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#include "pyas/serialise.h"

int main ( int argc, char *argv[] ) {
  if ( argc < 4 ) {
    fprintf( stderr, "Usage :\n\t%s regexp text\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }

  if(pyasm(argv[1],argv[2],argv[3])) exit(EXIT_FAILURE);


  exit( EXIT_SUCCESS );
}
