#include <stdio.h>
#include "pyas/regexp_char_group.h"
#include "pyas/chargroup.h" //char_group_parse

static int re_match_zero_or_more(list_t list_char_group, char *source , char ** end ) {

  char *t = source;
  char_group* c = list_first(list_char_group);

  if (c->group[(int)*t] == 1) {
    while ( '\0' != *t && (c->group[(int)*t] == 1)) {
      t++;
    }
  }

  do {
    if ( re_match( list_next(list_char_group), t, end ) ) {
      return 1;
    }
  }
  while( t-- > source );
  return 0;
}

static int re_match_one_or_more(list_t list_char_group, char *source , char ** end ) {
  char *t = source;

  char_group* c = list_first(list_char_group);

  if (c->group[(int)(int)*t] == 1) {
    /*si la position du 1 dans le tableau du char_group correspond au code ASCII du caractère pointé par
    la source correspond a celui de la regexp c bon, sinon il y en a moins que un donc ce n'est pas bon*/

    while ( '\0' != *t && ( c->group[(int)*t] == 1 ) ) { //on verifie si il n'y en a pas d'autres
    t++;
    }

    do {
      if ( re_match( list_next(list_char_group), t, end ) ) { //on verifie qu'il n'y a pas de prblm pour la suite de la regexp
        return 1;
      }
    }
    while( t-- > source + 1);

  }
  return 0;
}

static int re_match_zero_or_one(list_t list_char_group, char *source , char ** end ) {
  char *t = source;

  char_group* c = list_first(list_char_group) ;

  if (c->group[(int)*t] == 1) {
    t++;
  }

  do {
    if ( re_match( list_next(list_char_group), t, end  ) ) {
      return 1;
    }
  }
  while( t-- > source );
  return 0;

}

int re_match( list_t list_char_group, char *source, char **end ) {

  if ( list_empty(list_char_group) ) {
    //if ( end ) {
    *end = source;
    //}
    return 1;
  }

  char_group* c = list_first(list_char_group) ;

  if ( NULL == source ) {
    if ( c->op == 1 ) {
      return 1;
    }
    return 0;
  }

  if ( c->op == 1 ) {
    return re_match_zero_or_more( list_char_group, source, end) ;
  }
  if ( c->op == 2 ) {
    return re_match_one_or_more( list_char_group, source, end) ;
  }
  if ( c->op == 3 ) {
    return re_match_zero_or_one( list_char_group, source, end) ;
  }

  if ('\0' != *source && c->group[(int)*source] == 1) { //le group_char et le caractère pointé de la source correspondent
        return re_match( list_next(list_char_group), source+1, end );
  }
  return 0;

}
