#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pyas/label.h"

label_t label_new(char* value, int pos, int rel){
  label_t lab = calloc( 1, sizeof( *lab ) );

  assert( lab );

  if ( value  && *value  ) lab->value  = strdup( value );
  lab->position = pos;
  lab->rel = rel;

  return lab;
}

int label_print( void *_lab ){
  label_t lab = _lab ;

  return printf("[%s - %d - %d] ", lab->value, lab->position, lab->rel);
}

int label_delete( void *_lab ){
  label_t lab = _lab;

  if ( lab ) {
    free( lab->value );
  }

  free( lab );

  return 0;
}
