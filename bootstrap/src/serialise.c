#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "pyas/serialise.h"


int pyasm(char* lexdef_file, char* pys, char* pyc){

  list_t lexem = lex(pys, lexdef_file);
  pyobj_t p = pyobj_parse(&lexem,lexdef_file);

  FILE* f = fopen(pyc,"wb");
  fclose(f);

  f = fopen(pyc,"ab");
  if(f==NULL) printf("ERREUR\n");

  int p1 = p->py.codeblock->version_pyvm;
  int p2 = 0x0a0d;
  long p3 = time(NULL);
  pyobj_t p4 = p->py.codeblock;
  pyobj_t p5 = p->py.codeblock->binary.content.consts;
  pyobj_t p6 = p->py.codeblock->binary.content.names;
  pyobj_t p7 = p->py.codeblock->binary.content.varnames;
  pyobj_t p8 = p->py.codeblock->binary.content.freevars;
  pyobj_t p9 = p->py.codeblock->binary.content.cellvars;
  pyobj_t p10 = p->py.codeblock->binary.trailer.filename;
  pyobj_t p11 = p->py.codeblock->binary.trailer.name;
  uint32_t p12 = p->py.codeblock->binary.trailer.firstlineno;
  pyobj_t p13 = p->py.codeblock->binary.trailer.lnotab;

  fwrite(&p1,2,1,f);
  fwrite(&p2,2,1,f);
  fwrite(&p3,4,1,f);

  pyobj_write(f,p);
  pyobj_write(f,p5);
  pyobj_write(f,p6);
  pyobj_write(f,p7);
  pyobj_write(f,p8);
  pyobj_write(f,p9);
  pyobj_write(f,p10);
  pyobj_write(f,p11);

  fwrite(&p12,4,1,f);

  pyobj_write(f,p13);

  // pyobj_write(f,p->py.codeblock->binary.content.bytecode);

  fclose(f);

  return 0;
}

int pyobj_write (FILE* fp, pyobj_t obj){
  if(obj->type=='c'){
    fwrite(&(obj->type), sizeof(char),1,fp);
    printf("%lf\n", fwrite(&(obj->py.codeblock->header._0.arg_count),sizeof(int),1,fp));
    fwrite(&(obj->py.codeblock->header._0.local_count),4,1,fp);
    fwrite(&(obj->py.codeblock->header._0.stack_size),4,1,fp);
    fwrite(&(obj->py.codeblock->header._0.flags),4,1,fp);
    pyobj_write(fp,obj->py.codeblock->binary.content.bytecode);
  }

  if((obj->type =='t')||(obj->type=='u')||(obj->type=='s')||(obj->type=='z')||(obj->type=='Z')){
    fwrite(&(obj->type), sizeof(char),1,fp);
    fwrite(&(obj->py.string.length),sizeof(int),1,fp);
    fwrite(obj->py.string.buffer,sizeof(char),(obj->py.string.length),fp);
    //printf("%d\n", obj->py.string.length);
  }

  if((obj->type=='N')||(obj->type=='0')||(obj->type=='T')||(obj->type=='F')){
    fwrite(&(obj->type), sizeof(char),1,fp);
  }

  if((obj->type=='[')||(obj->type=='(')||(obj->type==')')){
    fwrite(&(obj->type), sizeof(char),1,fp);
    fwrite(&(obj->py.list.size),sizeof(int),1,fp);
    int i;
    for(i=0;i<obj->py.list.size;i++){
      pyobj_write(fp,obj->py.list.value[i]);
    }
  }

  if((obj->type=='i')){
    fwrite(&(obj->type), sizeof(char),1,fp);
    fwrite(&(obj->py.number.integer),sizeof(int),1,fp);
  }

  if((obj->type=='g')){
    fwrite(&(obj->type), sizeof(char),1,fp);
    fwrite(&(obj->py.number.real),sizeof(double),1,fp);
  }

  if((obj->type=='y')){
    fwrite(&(obj->type), sizeof(char),1,fp);
    fwrite(&(obj->py.number.complex.real),sizeof(double),1,fp);
    fwrite(&(obj->py.number.complex.imag),sizeof(double),1,fp);
  }
}
