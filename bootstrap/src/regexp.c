/**
 * @file regexp.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  28 09:56:03 2022
 * @brief regexp
 *
 * regexp code, as in the project's pdf document
 *
 */
/*
#include <stdio.h>
#include <pyas/regexp.h>

static int re_match_zero_or_more( char c, char *regexp, char * source , char ** end ) {
  char *t = source;
  while ( '\0' != *t && ( *t == c || '.' == c ) ) {
    t++;
  }
  do {
    if ( re_match( regexp, t, end ) ) {
      return 1;
    }
  }
  while( t-- > source );

  return 0;
}

static int re_match_one_or_more( char c, char *regexp, char * source , char ** end ) {
  char *t = source;

  if ( *t != c ) {
    return 0;
  }

  t++;

  while ( '\0' != *t && ( *t == c || '.' == c ) ) {
    t++;
  }
  do {
    if ( re_match( regexp, t, end ) ) {
      return 1;
    }
  }
  while( t-- > source + 1);
  // si on est obligé d'enlever tous les c pour que l'expression soit bonne il y a pblm

  return 0;
}

static int re_match_zero_or_one( char c, char *regexp, char * source , char ** end ) {
  char *t = source;

  if ( *t == c ) {
    t++;
  }

  do {
    if ( re_match( regexp, t, end ) ) {
      return 1;
    }
  }
  while( t-- > source );

  return 0;

}

int re_match( char *regexp, char *source, char **end ) {
  if ( NULL == source ) {
    if ( regexp && regexp[ 0 ] && '*' == regexp[ 1 ] ) {
      return 1;
    }
    return 0;
  }

  if ( NULL == regexp || '\0' == regexp[ 0 ] ) {
    if ( end ) {
      *end = source;
    }
    return 1;
  }
  if ( '*' == regexp[ 1 ] ) {
    return re_match_zero_or_more( regexp[ 0 ], regexp+2, source, end) ;
  }
  if ( '+' == regexp[ 1 ] ) {
    return re_match_one_or_more( regexp[ 0 ], regexp+2, source, end) ;
  }
  if ( '?' == regexp[ 1 ] ) {
    return re_match_zero_or_one( regexp[ 0 ], regexp+2, source, end) ;
  }
  if ( '\0' != *source &&
  ( '.' == regexp[ 0 ] || *source ==regexp[ 0 ] ) ) {
    return re_match( regexp+1, source+1, end );
  }
  return 0;
}
*/
