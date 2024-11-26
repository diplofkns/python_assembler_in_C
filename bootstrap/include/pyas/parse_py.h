#include "pyas/pyobj.h"
#include "pyas/lexem.h"
#include "pyas/list.h"
#include "pyas/parse.h"
#include "pyas/queue.h"
#include "pyas/lex_def.h"
#include "pyas/label.h"

pyobj_t pyobj_parse(list_t* l, char* regexp);

void pyobj_eol(list_t* l);
void pyobj_prologue(list_t* l, pyobj_t p);
void pyobj_set_directives(list_t* l, pyobj_t p);
void pyobj_interned_strings(list_t* l, pyobj_t p);
pyobj_t pyobj_constant(list_t* l);
pyobj_t pyobj_tuple(list_t* l);
void pyobj_constants(list_t* l, pyobj_t p);
void pyobj_names(list_t* l, pyobj_t p);
void pyobj_varnames_freevars_cellvars(list_t* l, pyobj_t p);
void pyobj_set_version_pyvm(list_t* l, pyobj_t p);
void pyobj_set_flags(list_t* l, pyobj_t p);
void pyobj_set_filename(list_t* l, pyobj_t p);
void pyobj_set_name(list_t* l, pyobj_t p);
void pyobj_set_source_size(list_t* l, pyobj_t p);
void pyobj_set_stack_size(list_t* l, pyobj_t p);
void pyobj_set_arg_count(list_t* l, pyobj_t p);
void pyobj_set_kwonly_arg_count(list_t* l, pyobj_t p);
void pyobj_set_posonly_arg_count(list_t* l, pyobj_t p);
void pyobj_code(list_t* l, pyobj_t p, char* regexp);
void pyobj_assembly_line(list_t* l, pyobj_t p, queue_t* q);
int pyobj_insn(list_t* l, pyobj_t p, queue_t* q, int* compt);
int pyobj_source_lineno(list_t* l, pyobj_t p, int* compt, queue_t* ltab);
int pyobj_label(list_t* l, pyobj_t p,queue_t* q);
void bytecode(pyobj_t p, queue_t* lex, list_t lexdf);
