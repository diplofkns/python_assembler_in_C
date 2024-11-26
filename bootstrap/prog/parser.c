
#include <stdio.h>
#include <stdlib.h>

#include "pyas/pyobj.h"
#include "pyas/parse.h"
// #include "pyas/parser.h"

/*Exécutable de test appelé parser qui prendra
en paramètre sur la ligne de commande un fichier de code source assembleur
Python et affichera l’objet de code Python ou la première erreur de syntaxe
(localisée) éventuellement détectée dans le code assembleur Python.
*/

int main ( int argc, char *argv[] ) {

  if ( argc < 2 ) {
    fprintf( stderr, "Usage :\n\t%s regexp text\n", argv[ 0 ] );
    exit(EXIT_FAILURE);
  }
  return NULL;
  // char* regexp_file = argv[1];
  // char* source_file = argv[2];
  //
  // list_t  list_lex_src = list_new();
  //
  // list_lex_src = lex(source_file, regexp_file);
  //
  // if (!parse(list_lex_src)) {
  //   printf("Erreur parsing du fichier de lexeme\n");
  //   exit(EXIT_FAILURE);
  // }
  //
  // pyobj_t pyobj_src = pyobj_new();
  // pyobj_src = parse_py(list_lex_src);
  //
  // pyobj_print(pyobj_src);

  exit(EXIT_SUCCESS);

}
