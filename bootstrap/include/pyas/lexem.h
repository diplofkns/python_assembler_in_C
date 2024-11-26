
/**
 * @file lexem.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  2 17:59:42 2021
 * @brief Lexems.
 *
 * Lexems.
 */

#ifndef _LEXEM_H_
#define _LEXEM_H_

#ifdef __cplusplus
extern "C" {
#endif

struct lexem {
  char *type;
  char *value;
  int   line;
  int   column;
};

  typedef struct lexem *lexem_t;

  lexem_t lexem_new( char *type, char *value, int line, int column );
  int lexem_print( void *_lex );
  int lexem_delete( void *_lex );

  /*lexem_t lexem_peek(list_t* lexems);
  lexem_t lexem_advance(list_t* lexems);
  int next_lexem_is(list_t* lexems, char* type);
  void print_parse_error( char *msg, list_t *lexems );*/

#ifdef __cplusplus
}
#endif

#endif /* _LEXEM_H_ */
