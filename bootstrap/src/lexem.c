
/**
 * @file lexem.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  2 17:59:27 2021
 * @brief Lexems.
 *
 * Lexems.
 */

#define _POSIX_C_SOURCE 200809L /* strdup(3) */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "pyas/lexem.h"
/*
#include "pyas/list.h"
#include "pyas/regexp_char_group.h"
#include "pyas/chargroup.h"
*/

/*struct lexem {
  char *type;
  char *value;
  int   line;
  int   column;
};*/

lexem_t lexem_new( char *type, char *value, int line, int column ) {
  lexem_t lex = calloc( 1, sizeof( *lex ) );

  assert( lex );

  if ( type  && *type  ) lex->type  = strdup( type );
  if ( value && *value ) lex->value = strdup( value );
  lex->line   = line;
  lex->column = column;

  return lex;
}

int     lexem_print( void *_lex ) {
  lexem_t lex = _lex;

  return printf( "[%d:%d:%s] %s",
		 lex->line,
		 lex->column,
		 lex->type,
		 lex->value );
}

int     lexem_delete( void *_lex ) {
  lexem_t lex = _lex;

  if ( lex ) {
    free( lex->type );
    free( lex->value );
  }

  free( lex );

  return 0;
}


/*
lexem_t lexem_peek(list_t* lexems){
  return list_first(*lexems);
}

lexem_t lexem_advance(list_t* lexems){
  return list_delete_first(*lexems,lexem_delete);
}

int next_lexem_is(list_t* lexems, char* type){
  return !strcmp(list_first(lexems).type, type) ;
}

void print_parse_error( char *msg, list_t *lexems ){
  printf("%s : ", msg) ;
  printf("Ligne %d, colonne %d\n", list_first(*lexems)->line, list_first(*lexems)->column);
}
*/
