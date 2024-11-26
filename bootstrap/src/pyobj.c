#include <stdio.h>
#include <stdlib.h>

#include "pyas/pyobj.h"


pyobj_t pyobj_new(char type){
  pyobj_t p = calloc(1,sizeof(*p));
  p->refcount++;
  p->type = type;
  if(type=='c'){
    codeblock_new(p);
  } else if(type=='s'||type=='R'||type=='a'||type=='A'||type=='z'||type=='Z'||type=='u'||type=='t'){
    p->py.string.length = 0;
  } else if(type=='['||type=='('||type == ')'){
    p->py.list.size = 0;
  } else if(type=='i'){
    p->py.number.integer = 0;
  } else if(type=='g'){
    p->py.number.real = 0;
  } else if(type=='f'){
    p->py.string.length = 0;
  } else if(type=='x') {
    p->py.string.length = 0;
  } else if (type=='y') {
    p->py.number.complex.real = 0;
    p->py.number.complex.imag = 0;
  } else if (type=='0'||type=='N'||type=='F'||type=='T') {
    ;
  } else {
    printf("%c : ERREUR, type d'objet Python non géré dans ce projet\n", type);
    exit(EXIT_FAILURE);
  }
  return p;
}

void pyobj_delete(pyobj_t p){
  // printf("APPEL\n");
  int i ;
  if(p->type == '('||p->type == ')'||p->type == '['){
    for(i=0;i<p->py.list.size;i++) pyobj_delete(p->py.list.value[i]);
    if(p->py.list.size != 0) {
      free(p->py.list.value);
    }
  } else if(p->type=='s'||p->type=='R'||p->type=='a'||p->type=='A'||p->type=='z'||p->type=='Z'||p->type=='u'||p->type=='t'||p->type=='x'||p->type=='f') {
    if(p->py.string.length!=0) free(p->py.string.buffer);
  } else if(p->type=='c'){
    code_block_delete(p);
  }
  free(p);

  return 1;
}

void code_block_delete(pyobj_t p){
  pyobj_delete(p->py.codeblock->binary.content.interned);
  pyobj_delete(p->py.codeblock->binary.content.bytecode);
  pyobj_delete(p->py.codeblock->binary.content.consts);
  pyobj_delete(p->py.codeblock->binary.content.names);
  pyobj_delete(p->py.codeblock->binary.content.varnames);
  pyobj_delete(p->py.codeblock->binary.content.freevars);
  pyobj_delete(p->py.codeblock->binary.content.cellvars);
  pyobj_delete(p->py.codeblock->binary.trailer.filename);
  pyobj_delete(p->py.codeblock->binary.trailer.name);
  pyobj_delete(p->py.codeblock->binary.trailer.lnotab);
  free(p->py.codeblock);
  return 1;
}

void codeblock_new(pyobj_t p){
  p->py.codeblock = calloc(1,sizeof(*(p->py.codeblock)));
  p->py.codeblock->binary.content.interned = pyobj_new('[');
  p->py.codeblock->binary.content.bytecode = pyobj_new('s');
  p->py.codeblock->binary.content.consts = pyobj_new('(');
  p->py.codeblock->binary.content.names = pyobj_new('(');
  p->py.codeblock->binary.content.varnames = pyobj_new('(');
  p->py.codeblock->binary.content.freevars = pyobj_new('(');
  p->py.codeblock->binary.content.cellvars = pyobj_new('(');
  p->py.codeblock->binary.trailer.filename = pyobj_new('s');
  p->py.codeblock->binary.trailer.name = pyobj_new('t');
  p->py.codeblock->binary.trailer.lnotab = pyobj_new('s');
}

void pyobj_print(pyobj_t p){
  if(p->type == '('||p->type == ')'||p->type == '['){
    int i;
    for(i=0;i<p->py.list.size;i++){
      pyobj_print(p->py.list.value[i]);
    }
  } else if(p->type=='s'||p->type=='R'||p->type=='a'||p->type=='A'||p->type=='z'||p->type=='Z'||p->type=='u'||p->type=='t'||p->type=='x'||p->type=='f') {
    printf("%s\n", p->py.string.buffer);
  } else if(p->type == 'i') {
    printf("%d\n", p->py.number.integer);
  } else if(p->type == 'g') {
    printf("%lf\n", p->py.number.real);
  } else if (p->type == 'y') {
    printf("%lf + i%lf\n", p->py.number.complex.real, p->py.number.complex.imag);
  } else if (p->type=='0') {
    printf("NULL\n");
  } else if (p->type=='N') {
    printf("None\n");
  } else if (p->type=='F') {
    printf("False\n");
  } else if (p->type=='T') {
    printf("True");
  } else {
    codeblock_print(p);
  }
}

void codeblock_print(pyobj_t p){
  int i ;
  printf("Version_pyvm : %d\n", p->py.codeblock->version_pyvm);
  printf("Arg_count : %d\n", p->py.codeblock->header._0.arg_count);
  printf("stack_size : %d\n", p->py.codeblock->header._0.stack_size);
  printf("Flags : %d\n", p->py.codeblock->header._0.flags);
  printf("Filename : %s\n", p->py.codeblock->binary.trailer.filename->py.string.buffer);
  printf("Name : %s\n", p->py.codeblock->binary.trailer.name->py.string.buffer);
  printf("\nInterned : \n");
  pyobj_print(p->py.codeblock->binary.content.interned);
  printf("\nConsts :\n");
  pyobj_print(p->py.codeblock->binary.content.consts);
  printf("\nNames :\n");
  pyobj_print(p->py.codeblock->binary.content.names);
  printf("\nFirstlineno : %d\n", p->py.codeblock->binary.trailer.firstlineno);
  printf("\nLnotab : ");
  for(i=0;i<p->py.codeblock->binary.trailer.lnotab->py.string.length;i++) printf("%x ", p->py.codeblock->binary.trailer.lnotab->py.string.buffer[i]);
  printf("\n");
  printf("\nBytecode : ");
  for(i=0;i<p->py.codeblock->binary.content.bytecode->py.string.length;i++) printf("%x ", p->py.codeblock->binary.content.bytecode->py.string.buffer[i]);
  printf("\n");
}
