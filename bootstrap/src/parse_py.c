#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pyas/parse_py.h"

void imprime(int* i){
  printf("%d\n", *i);
}

pyobj_t pyobj_parse(list_t* l,char* regexp){
  list_t save = *l;
  pyobj_t p = pyobj_new('c');
  pyobj_eol(l);
  pyobj_prologue(l,p);
  pyobj_code(l,p,regexp);
  if(!list_empty(*l)) printf("Erreur syntaxe fichier assembleur");
  *l = save;
  return p;
}

void pyobj_eol(list_t* l){
  int i = 1;
  list_t save = list_new();
  while(i){
    save = *l;
    if(next_lexem_is(l,"blank")){
      lexem_advance(l);
    }
    if(next_lexem_is(l,"comment")){
      lexem_advance(l);
    }
    if(!next_lexem_is(l,"newline")){
      i=0;
      *l = save;
    } else {
      lexem_advance(l);
    }

  }
  if(next_lexem_is(l,"blank")) {
    lexem_advance(l);
  }
  return 1;
}

void pyobj_prologue(list_t* l, pyobj_t p){
  pyobj_set_directives(l,p);
  pyobj_interned_strings(l,p);
  pyobj_constants(l,p);
  pyobj_names(l,p);
  pyobj_varnames_freevars_cellvars(l,p);

}

  void pyobj_set_directives(list_t* l, pyobj_t p){
    pyobj_set_version_pyvm(l,p);
    pyobj_set_flags(l,p);
    pyobj_set_filename(l,p);
    pyobj_set_name(l,p);
    pyobj_set_source_size(l,p);
    pyobj_set_stack_size(l,p);
    pyobj_set_arg_count(l,p);
    pyobj_set_kwonly_arg_count(l,p);
    pyobj_set_posonly_arg_count(l,p);

  }

  void pyobj_interned_strings(list_t* l, pyobj_t p){
    lexem_advance(l);
    pyobj_eol(l);

    //Comptage du nombre de strings
    list_t save = *l;
    int compt = 0;

    while(next_lexem_is(l,"string")){
      compt++;
      lexem_advance(l);
      pyobj_eol(l);
    }
    *l = save;
    p->py.codeblock->binary.content.interned->py.list.size = compt;
    p->py.codeblock->binary.content.interned->py.list.value = calloc(compt,sizeof(pyobj_t));
    int i;
    for(i=0;i<compt;i++){
      lexem_t str = lexem_peek(l);
      int taille = strlen(str->value) -2 ;
      (p->py.codeblock->binary.content.interned->py.list.value[i]) = pyobj_new('t');
      (p->py.codeblock->binary.content.interned->py.list.value[i])->py.string.length = taille;
      (p->py.codeblock->binary.content.interned->py.list.value[i])->py.string.buffer = calloc(taille,sizeof(char));
      int j;
      for(j=1;j<taille+1;j++) (p->py.codeblock->binary.content.interned->py.list.value[i])->py.string.buffer[j-1] = str->value[j];
      lexem_advance(l);
      eol(l);
    }
  }

  pyobj_t pyobj_constant(list_t* l){
    lexem_t lex = lexem_peek(l);
    if(next_lexem_is(l,"integer::dec")||next_lexem_is(l,"integer::hex")){
      pyobj_t p = pyobj_new('i');
      p->py.number.integer = atoi(lex->value);
      lexem_advance(l);
      return p;
    } else if (next_lexem_is(l,"float")) {
      pyobj_t p = pyobj_new('f');
      p->py.number.real = atoi(lex->value);
      lexem_advance(l);
      return p;
    } else if (next_lexem_is(l,"string")) {
      pyobj_t p = pyobj_new('t');
      p->py.string.length = strlen(lex->value)+1;
      p->py.string.buffer = calloc(p->py.string.length,sizeof(char));
      int i;
      for(i=0;i<p->py.string.length;i++) p->py.string.buffer[i] = lex->value[i] ;
      lexem_advance(l);
      return p;
    } else if (next_lexem_is(l,"pycst::None")) {
      return pyobj_new('N');
    } else if (next_lexem_is(l,"pycst::null")) {
      return pyobj_new('0');
    } else if (next_lexem_is(l,"pycst::True")) {
      return pyobj_new('T');
    } else if (next_lexem_is(l,"pycst::False")) {
      return pyobj_new('F');
    } else {
      return pyobj_tuple(l);
    }
  }

  pyobj_t pyobj_tuple(list_t* l){
    pyobj_t p = pyobj_new('(');
    list_t save = *l;
    while(!next_lexem_is(l,"paren::right")){
      lexem_advance(l);
      eol(l);
      p->py.list.size++;
    }
    *l = save;
    p->py.list.value = calloc(p->py.list.size, sizeof(pyobj_t));
    int i;
    for(i=0;i<p->py.list.size;i++){
      p->py.list.value[i] = pyobj_constant(l);
      eol(l);
    }
    return p;
  }

  void pyobj_constants(list_t* l, pyobj_t p){
    lexem_advance(l);
    eol(l);

    queue_t constantes = queue_new() ;
    int compt = 0;
    while(next_lexem_is(l,"integer::dec")||next_lexem_is(l,"integer::hex")||next_lexem_is(l,"float")||next_lexem_is(l,"string")||next_lexem_is(l,"pycst::None")||next_lexem_is(l,"pycst::False")||next_lexem_is(l,"pycst::True")||next_lexem_is(l,"paren::left")){
      constantes = enqueue(pyobj_constant(l),constantes) ;
      lexem_advance(l);
      eol(l);
      compt++;
    }
    // printf("%d\n", queue_count(constantes));

    p->py.codeblock->binary.content.consts->py.list.size = compt;
    p->py.codeblock->binary.content.consts->py.list.value = calloc(compt,sizeof(pyobj_t));
    int i=0;
    while(!queue_is_empty(constantes)){
      pyobj_t p1 = dequeue(&constantes,pyobj_delete);
      p->py.codeblock->binary.content.consts->py.list.value[i] = p1;
      i++;
    }
  }

  void pyobj_names(list_t* l, pyobj_t p){
    lexem_advance(l);
    eol(l);
    int compt = 0;
    list_t save = *l;
    while(next_lexem_is(l,"string")){
      compt++;
      lexem_advance(l);
      eol(l);
    }
    *l = save;

    p->py.codeblock->binary.content.names->py.list.size = compt;
    p->py.codeblock->binary.content.names->py.list.value = calloc(compt,sizeof(pyobj_t));
    int i ;
    for(i=0;i<compt;i++){
      lexem_t str = lexem_peek(l);
      int n = strlen(str->value)-2;
      pyobj_t p1 = pyobj_new('t');
      p1->py.string.length = n;
      p1->py.string.buffer = calloc(n,sizeof(char));
      int y;
      for(y=1;y<n+1;y++) p1->py.string.buffer[y-1] = str->value[y];
      p->py.codeblock->binary.content.names->py.list.value[i] = p1;
      lexem_advance(l);
      eol(l);
    }
  }

  void pyobj_varnames_freevars_cellvars(list_t* l, pyobj_t p){

    queue_t vars = queue_new();

    int vn, fv, cv = 0;

    if(next_lexem_is(l,"dir::varnames")){
      lexem_advance(l);
      pyobj_eol(l);
      while(next_lexem_is(l,"string")){
        enqueue(lexem_peek(l),vars);
        vn++;
        lexem_advance(l);
        pyobj_eol(l);
      }
      lexem_advance(l);
      pyobj_eol(l);
      while(next_lexem_is(l,"string")){
        enqueue(lexem_peek(l),vars);
        fv++;
        lexem_advance(l);
        pyobj_eol(l);
      }

      lexem_advance(l);
      pyobj_eol(l);
      while(next_lexem_is(l,"string")){
        enqueue(lexem_peek(l),vars);
        cv++;
        lexem_advance(l);
        pyobj_eol(l);
      }

      p->py.codeblock->binary.content.varnames->py.list.value = calloc(vn,sizeof(pyobj_t));
      p->py.codeblock->binary.content.freevars->py.list.value = calloc(fv,sizeof(pyobj_t));
      p->py.codeblock->binary.content.cellvars->py.list.value = calloc(cv,sizeof(pyobj_t));

      int i;

      for(i=0;i<vn;i++){
        p->py.codeblock->binary.content.varnames->py.list.value[i] = pyobj_new('s');
        lexem_t var = dequeue(&vars,lexem_delete);
        p->py.codeblock->binary.content.varnames->py.list.value[i]->py.string.buffer = strdup(var->value);
        p->py.codeblock->binary.content.varnames->py.list.value[i]->py.string.length = strlen(var->value);
      }
      for(i=0;i<fv;i++){
        p->py.codeblock->binary.content.freevars->py.list.value[i] = pyobj_new('s');
        lexem_t var = dequeue(&vars,lexem_delete);
        p->py.codeblock->binary.content.freevars->py.list.value[i]->py.string.buffer = strdup(var->value);
        p->py.codeblock->binary.content.freevars->py.list.value[i]->py.string.length = strlen(var->value);
      }
      for(i=0;i<cv;i++){
        p->py.codeblock->binary.content.cellvars->py.list.value[i] = pyobj_new('s');
        lexem_t var = dequeue(&vars,lexem_delete);
        p->py.codeblock->binary.content.cellvars->py.list.value[i]->py.string.buffer = strdup(var->value);
        p->py.codeblock->binary.content.cellvars->py.list.value[i]->py.string.length = strlen(var->value);
      }
    }
  }


void pyobj_set_version_pyvm(list_t* l, pyobj_t p){
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_t vers = lexem_peek(l) ;
  p->py.codeblock->version_pyvm = atoi(vers->value);
  lexem_advance(l);
  pyobj_eol(l);
}

void pyobj_set_flags(list_t* l, pyobj_t p){
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_t flag = lexem_peek(l);
  p->py.codeblock->header._0.flags = strtol(flag->value,NULL,16);
  lexem_advance(l);
  pyobj_eol(l);
}

void pyobj_set_filename(list_t* l, pyobj_t p){
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_t filen = lexem_peek(l);
  char* k ;
  int i;
  for(k=filen->value;*k!=0;k++) p->py.codeblock->binary.trailer.filename->py.string.length++ ;
  p->py.codeblock->binary.trailer.filename->py.string.buffer = calloc(p->py.codeblock->binary.trailer.filename->py.string.length-=2,sizeof(char));
  for(i=1;i<p->py.codeblock->binary.trailer.filename->py.string.length-1;i++) p->py.codeblock->binary.trailer.filename->py.string.buffer[i-1] = filen->value[i] ;
  lexem_advance(l);
  pyobj_eol(l);
}

void pyobj_set_name(list_t* l, pyobj_t p){
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_t filen = lexem_peek(l);
  char* k ;
  int i;
  for(k=filen->value;*k!=0;k++) p->py.codeblock->binary.trailer.name->py.string.length++ ;
  p->py.codeblock->binary.trailer.name->py.string.buffer = calloc(p->py.codeblock->binary.trailer.name->py.string.length-=2,sizeof(char));
  for(i=1;i<p->py.codeblock->binary.trailer.name->py.string.length-1;i++) p->py.codeblock->binary.trailer.name->py.string.buffer[i-1] = filen->value[i] ;
  lexem_advance(l);
  pyobj_eol(l);
}

void pyobj_set_source_size(list_t* l, pyobj_t p){
  list_t* t = calloc(1,sizeof(*t));
  *t = list_next(list_next(*l));
  if(next_lexem_is(t,"source_size")){
    lexem_advance(l);
    lexem_advance(l);
    lexem_advance(l);
    lexem_advance(l);
    lexem_t ss = lexem_peek(l);
    p->py.codeblock->binary.header._1.source_size = atoi(ss->value);
    p->py.codeblock->binary.header._2.source_size = atoi(ss->value);
    lexem_advance(l);
    pyobj_eol(l);
  }
  free(t);
}

void pyobj_set_stack_size(list_t* l, pyobj_t p){
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_t ss = lexem_peek(l);
  p->py.codeblock->header._0.stack_size = atoi(ss->value);
  p->py.codeblock->header._1.stack_size = atoi(ss->value);
  lexem_advance(l);
  pyobj_eol(l);
}

void pyobj_set_arg_count(list_t* l, pyobj_t p){
  if(next_lexem_is(l,"dir::set")){
    lexem_advance(l);
    lexem_advance(l);
    lexem_advance(l);
    lexem_advance(l);
    lexem_t ac = lexem_peek(l);
    p->py.codeblock->header._1.arg_count = atoi(ac->value);
    lexem_advance(l);
    pyobj_eol(l);
  }
}

void pyobj_set_kwonly_arg_count(list_t* l, pyobj_t p){
  list_t* t = calloc(1,sizeof(*t));
  *t = list_next(list_next(*l));
  if(next_lexem_is(t,"kwonly_arg_count")){
    lexem_advance(l);
    lexem_advance(l);
    lexem_advance(l);
    lexem_advance(l);
    lexem_t kac = lexem_peek(l);
    p->py.codeblock->header._1.kwonly_arg_count = atoi(kac->value);
    lexem_advance(l);
    pyobj_eol(l);
  }
  free(t);
}

void pyobj_set_posonly_arg_count(list_t* l, pyobj_t p){
  list_t* t = calloc(1,sizeof(*t));
  *t = list_next(list_next(*l));
  if(next_lexem_is(t,"posonly_arg_count")){
    lexem_advance(l);
    lexem_advance(l);
    lexem_advance(l);
    lexem_advance(l);
    lexem_t pac = lexem_peek(l);
    p->py.codeblock->header._1.kwonly_arg_count = atoi(pac->value);
    lexem_advance(l);
    pyobj_eol(l);
  }
  free(t);
}


void pyobj_code(list_t* l, pyobj_t p, char* regexp){
  queue_t lbytecode = queue_new();
  lexem_advance(l);
  pyobj_eol(l);
  lexem_advance(l);
  lexem_advance(l);
  lexem_t line = lexem_peek(l);
  p->py.codeblock->binary.trailer.firstlineno = atoi(line->value);
  lexem_advance(l);
  pyobj_eol(l);
  pyobj_assembly_line(l,p,&lbytecode);
  // queue_print(lbytecode,lexem_print);
  //Création d'une liste de lx_def réduite aux mnémoniques (se type insn::0 ou type insn::1)
  list_t lexdf = read(regexp);

  lex_def_t lx = list_first(lexdf);

  while(strcmp(lx->type,"insn::0")){
    lexdf = list_del_first(lexdf, lex_def_delete);
    lx = list_first(lexdf);
  }

  list_t parcours = lexdf;
  while(!strcmp(lx->type,"insn::0")||!strcmp(lx->type,"insn::1")){
    parcours = parcours->next;
    lx = list_first(parcours->next);
  }
  list_t parcours2 = parcours;
  parcours = parcours->next;
  list_delete(parcours,lex_def_delete);
  parcours2->next = NULL;

  bytecode(p, &lbytecode, lexdf);
  pyobj_eol(l);
}

void pyobj_assembly_line(list_t* l, pyobj_t p, queue_t *q){
  int compt = 0;
  queue_t ltab = queue_new();
  while(!list_empty(*l)&&(pyobj_insn(l,p,q,&compt)||pyobj_source_lineno(l,p,&compt,&ltab)||pyobj_label(l,p,q))){
    pyobj_eol(l);
  }
  // queue_print(ltab, lexem_print);
  if(queue_is_empty(ltab)){
    printf("Pas de lignes de code\n");
    exit(EXIT_FAILURE);
  }
  int taille = queue_count(ltab);
  p->py.codeblock->binary.trailer.lnotab->py.string.length = 2*taille;
  p->py.codeblock->binary.trailer.lnotab->py.string.buffer = calloc(2*taille, sizeof(char));
  char* cursor = p->py.codeblock->binary.trailer.lnotab->py.string.buffer;
  while(queue_count(ltab)!=0){
    lexem_t knot = dequeue(&ltab,lexem_delete);
    *cursor = (char)knot->line;
    cursor++;
    *cursor = (char)knot->column;
    cursor++;
    // sprintf(cursor,"%x",knot->line);
    // while(*cursor!=0) cursor++;
    // sprintf(cursor,"%x",knot->column);
    // while(*cursor!=0) cursor++;
  }
  cursor--;
  while(cursor!=p->py.codeblock->binary.trailer.lnotab->py.string.buffer+1){
    *cursor = *cursor - *(cursor-2);
    cursor-=2;
  }
  *cursor-=p->py.codeblock->binary.trailer.firstlineno;
  // int i;
  // for(i=0;i<2*taille;i++) printf("%d\n",p->py.codeblock->binary.trailer.lnotab->py.string.buffer[i]);

}

int pyobj_insn(list_t* l, pyobj_t p, queue_t* q, int* compt){
  if(next_lexem_is(l,"insn::0")){
    *compt += 1;
    p->py.codeblock->binary.content.bytecode->py.string.length += 1;
    *q = enqueue(lexem_peek(l),*q);
    lexem_advance(l);
    return 1;
  } else if(next_lexem_is(l,"insn::1")){
    *compt+=3;
    p->py.codeblock->binary.content.bytecode->py.string.length += 3;
    *q = enqueue(lexem_peek(l),*q);
    lexem_advance(l);
    lexem_advance(l);
    *q = enqueue(lexem_peek(l),*q);
    lexem_advance(l);
    return 1;
  } else {
    return 0;
  }
}

int pyobj_source_lineno(list_t* l, pyobj_t p, int *compt, queue_t* ltab){
  if(!next_lexem_is(l,"dir::line")) return 0;
  lexem_advance(l);
  lexem_advance(l);
  lexem_t line = lexem_peek(l);
  lexem_t tw_recy = lexem_new("md","r",*compt,atoi(line->value));
  *compt=0;
  *ltab = enqueue(tw_recy,*ltab);
  lexem_advance(l);
  return 1;
}

int pyobj_label(list_t* l, pyobj_t p, queue_t* q){
  if(!next_lexem_is(l,"symbol")) return 0;
  *q = enqueue(lexem_peek(l),*q);
  lexem_advance(l);
  lexem_advance(l);
  lexem_advance(l);
  return 1;
}

void bytecode(pyobj_t p, queue_t* lex, list_t lexdf){
  list_t save = lexdf;
  p->py.codeblock->binary.content.bytecode->py.string.buffer = calloc(p->py.codeblock->binary.content.bytecode->py.string.length,sizeof(char));
  char* cursor = p->py.codeblock->binary.content.bytecode->py.string.buffer;
  lex_def_t ld = list_first(lexdf);
  int compt = 0;
  queue_t labels = queue_new();
  list_t abs_labels = list_new();
  while(!queue_is_empty(*lex)){
    lexem_t l1 = dequeue(lex,lexem_delete);
    if(!strcmp(l1->type,"insn::0")) {
      //Recherche et écriture de l'opcode

      while(!re_match(ld->value,l1->value,l1->value)){
        if (list_empty(lexdf)||list_empty(lexdf->next)) {
          printf("Le type du lexème n'est pas reconnu\n");
          exit(EXIT_FAILURE);
        }
        lexdf = lexdf->next;
        ld = list_first(lexdf);
      }
      lexdf = save;
    // printf("%x\n", ld->opcode);
    *cursor = ld->opcode;
    cursor++;

    compt++;

  } else if(!strcmp(l1->type, "insn::1")){
      //Recherche et écriture de l'opcode
      while(!re_match(ld->value,l1->value,l1->value)){
        if (list_empty(lexdf)||list_empty(lexdf->next)) {
          printf("%s : Le type du lexème n'est pas reconnu\n", l1->value);
          exit(EXIT_FAILURE);
        }
        lexdf = lexdf->next;
        ld = list_first(lexdf);
      }
      lexdf = save;
      // printf("%x\n", ld->opcode);
      *cursor = ld->opcode;
      cursor++;

      lexem_t l2 = dequeue(lex,lexem_delete);
      int i;
      if(!strcmp(l2->type,"symbol")){
        i = 0;
        int position = compt+1;
        int rel = (ld->opcode == 0x70)||(ld->opcode == 0x6f)||(ld->opcode == 0x71)||(ld->opcode == 0x73)||(ld->opcode == 0x72)||(ld->opcode == 0x77) ;
        label_t lab = label_new(l2->value,position,rel);
        labels = enqueue(lab,labels);
      } else {
        i = atoi(l2->value);
      }
      *cursor = i%256;
      cursor++;
      *cursor = (i-*cursor)/256;
      cursor++;
      compt+=3;
    } else {
      label_t abslab = label_new(l1->value,compt,0);
      abs_labels = enqueue(abslab,abs_labels);
      if(!queue_is_empty(labels)) {
        queue_t pointeur = labels;
        do{
          label_t lab = queue_peek(labels);
          if(!strcmp(l1->value,lab->value)){
            // printf("Coucou\n");
            if(lab->rel){
              p->py.codeblock->binary.content.bytecode->py.string.buffer[lab->position] = compt%256;
              p->py.codeblock->binary.content.bytecode->py.string.buffer[lab->position+1] = compt/256;
            } else {
              p->py.codeblock->binary.content.bytecode->py.string.buffer[lab->position] = (compt - lab->position -2)%256;
              p->py.codeblock->binary.content.bytecode->py.string.buffer[lab->position+1] = (compt - lab->position -2)/256;
            }

            dequeue(&labels, label_delete);
          } else {
            labels = labels->next;
          }
        } while(pointeur!=labels->next);
        label_t lab = queue_peek(labels);
        if(!strcmp(l1->value,lab->value)){
          // printf("Coucou\n");
          if(lab->rel){
            p->py.codeblock->binary.content.bytecode->py.string.buffer[lab->position] = compt%256;
            p->py.codeblock->binary.content.bytecode->py.string.buffer[lab->position+1] = compt/256;
          } else {
            p->py.codeblock->binary.content.bytecode->py.string.buffer[lab->position] = (compt - lab->position -2)%256;
            p->py.codeblock->binary.content.bytecode->py.string.buffer[lab->position+1] = (compt - lab->position -2)/256;
          }

          dequeue(&labels, label_delete);
        } else {
          labels = labels->next;
        }
      }
    }
  }
  while(!queue_is_empty(labels)) {
    label_t lb1 = labels->content;
    list_t point = abs_labels;
    while(!list_empty(point)){
      label_t lb = list_first(point);
      if(!strcmp(lb1->value,lb->value)){
        // printf("Coucou\n");
        p->py.codeblock->binary.content.bytecode->py.string.buffer[lb1->position] = lb->position%256;
        p->py.codeblock->binary.content.bytecode->py.string.buffer[lb1->position+1] = lb->position/256;

        dequeue(&labels, label_delete);

        point = NULL;
      }
      if(!list_empty(point)) point = list_next(point);
    }
    point = abs_labels;
  }
  // queue_delete(labels,label_delete);
}
