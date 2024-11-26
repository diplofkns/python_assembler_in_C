#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "pyas/lex_def.h"
#include "pyas/chargroup.h"

/*Structure annexe pour la lecture du fichier de definition de lexeme*/

lex_def_t lex_def_new( char *type, list_t value, int opcode) {
  lex_def_t lexdf = calloc( 1, sizeof( *lexdf ) );

  assert( lexdf );

  if ( type  && *type  ) lexdf->type  = strdup( type );
  lexdf->opcode  = opcode;
  lexdf->value = value;

  return lexdf;
}

int     lex_def_delete( void *_lexdf, action_t delete) {
  lex_def_t lexdf = _lexdf;

  if ( lexdf ) {
    free( lexdf->type );
    list_delete(lexdf->value,char_group_delete_cb);
  }

  free( lexdf );

  return(EXIT_SUCCESS);
}

int     lex_def_print( void *_lex ) {
  lex_def_t lex = _lex;
  printf( "type : %s ||",lex->type);
  printf( " liste de chargroup:");
	list_print(lex->value,char_group_print_cb);
  printf( " || opcode : %d\n",lex->opcode);
  return EXIT_SUCCESS;
}
