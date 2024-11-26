
#include "pyas/list.h"
#include "pyas/callbacks.h"

#ifndef _LEX_DEF_H_
#define _LEX_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*Structure annexe pour la lecture du fichier de definition de lexeme*/

struct lex_def {
  char *type;
  list_t value; //list de chargroup
  //int opcode;
  int opcode;
};

  typedef struct lex_def *lex_def_t;

  lex_def_t lex_def_new( char *type, list_t value, int opcode);
  int lex_def_delete( void *_lex, action_t delete);
  int lex_def_print( void *_lex);

#ifdef __cplusplus
}
#endif

#endif /* _LEX_DEF_H_ */
