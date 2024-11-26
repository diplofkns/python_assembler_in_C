#include <stdlib.h>
#include <stdio.h>

#include "pyas/list.h"
#include "pyas/lexem_src.h"

/* Executable du livrable 1 : Final : Entrée : 2 fichiers sources et
def de lexemes et sortie : Liste de lexeme de source */
/*verification fichier.pys*/

int main ( int argc, char *argv[] ) {

  if ( argc < 3 ) {
    fprintf( stderr, "Usage :\n\t%s regexp text\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }

  char* regexp_file = argv[1];
  char* source_file = argv[2];

  list_t  list_lex_src = list_new();

  list_lex_src = lex(source_file, regexp_file);

  list_print(list_lex_src, lexem_print);

  exit(EXIT_SUCCESS);
}
