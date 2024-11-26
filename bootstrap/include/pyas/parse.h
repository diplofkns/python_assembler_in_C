#include "lexem.h"
#include "list.h"

int parse(list_t* lexems);


int eol(list_t* l);
int prologue(list_t* l);
  int set_directives(list_t* l);
    int set_version_pyvm(list_t* l);
    int set_flags(list_t* l);
    int set_filename(list_t* l);
    int set_name(list_t* l);
    int set_source_size(list_t* l);
    int set_stack_size(list_t* l);
    int set_arg_count(list_t* l);
    int set_kwonly_arg_count(list_t* l);
    int set_posonly_arg_count(list_t* l);
  int interned_strings(list_t* l);
  int constants(list_t* l);
    int constant(list_t* l);
    int list(list_t* l);
      int tuple(list_t* l);
  int names(list_t* l);
  int varnames(list_t* l);
  int freevars(list_t* l);
  int cellvars(list_t* l);
int code(list_t* l);
  int assembly_line(list_t* l);
    int label(list_t* l);
    int source_lineno(list_t* l);
    int insn(list_t* l);

lexem_t lexem_advance( list_t *lexems);
lexem_t lexem_peek(list_t *lexems);
int next_lexem_is( list_t *lexems, char*type);
