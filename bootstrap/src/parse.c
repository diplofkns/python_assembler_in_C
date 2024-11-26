#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pyas/lexem.h"
#include "pyas/list.h"
#include "pyas/parse.h"

lexem_t lexem_peek(list_t *lexems){
  return list_first(*lexems);
}

lexem_t lexem_advance( list_t *lexems){
  list_t l = list_new();
  l = *lexems;
  *lexems = list_next(*lexems);
  return list_first(l);
}

int next_lexem_is( list_t *lexems, char*type){
  if(list_empty(*lexems)) return 0;
  lexem_t lx;
  lx = list_first(*lexems);
  return !strcmp(lx->type, type);
}

void print_parse_error( char* msg, list_t *lexems){
  lexem_t lex;
  lex = list_first(*lexems);
  printf(msg);
  printf(" ligne %d colonne %d\n", lex->line,lex->column );
}


int parse(list_t* lexems){
  if(list_empty(*lexems)) return 1;

  //Non-terminal eol

  if(!eol(lexems)){
    printf("Erreur eol\n");
    exit(EXIT_FAILURE);
  }

  //Non-terminal prologue

  if(!prologue(lexems)){
    printf("Erreur prologue\n");
    exit(EXIT_FAILURE);
  }

  //Non-terminal code

  if(!code(lexems)){
    printf("Error code\n");
    exit(EXIT_FAILURE);
  }

  return 1;
}

int eol(list_t* l){
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

int prologue(list_t* l){
  if(set_directives(l)){
  } else {
    printf("set_directives manquant\n");
    return 0;
  }

  if(interned_strings(l)){
  } else {

    return 0;
  }

  if(constants(l)){
  } else {
    return 0;
  }

  if(names(l)){
  } else {
    return 0;
  }

  list_t save = *l;
  if(!(varnames(l)&&freevars(l)&&cellvars(l))){
    *l=save;
  }
  return 1;
}

  int set_directives(list_t* l){
    list_t save;
    if(set_version_pyvm(l)){
    } else {
      return 0;
    }

    if(set_flags(l)){
    } else {
      return 0;
    }

    if(set_filename(l)){
    } else {
      return 0;
    }

    if(set_name(l)){
    } else {
      return 0;
    }

    save = *l;
    if(!set_source_size(l)){
      *l=save;
    } else{
    }

    if(set_stack_size(l)){
    } else {
      return 0;
    }

    save = *l;
    if(!set_arg_count(l)){
      *l=save;
    } else {
    }

    save = *l;
    if(set_kwonly_arg_count(l)){
      *l=save;
    }else {
    }

    save = *l;
    if(set_posonly_arg_count(l)){
      *l=save;
    }else {
    }

    return 1;
  }

    int set_version_pyvm(list_t* l){
      if(!next_lexem_is(l,"dir::set")){
        print_parse_error("Erreur, .set manquant", l) ;
        return 0;
      }
      lexem_advance(l);
      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur d'espace", l) ;
        return 0;
      }
      lexem_advance(l);
      if(!next_lexem_is(l,"version_pyvm")){
        print_parse_error("Version_pyvm non déclarée", l) ;
        return 0;
      }
      lexem_advance(l);
      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur d'espace", l) ;
        return 0;
      }
      lexem_advance(l);
      if(!next_lexem_is(l,"integer::dec")){
        print_parse_error("Numéro de version_pyvm non déclarée", l) ;
        return 0;
      }
      lexem_advance(l);

      if(eol(l)){
      } else {
        return 0;
      }

      return 1;
    }

    int set_flags(list_t* l){
      if(!next_lexem_is(l,"dir::set")){
        print_parse_error(".set manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"flags")){
        print_parse_error("Flags non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);


      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);


      if(!next_lexem_is(l,"integer::hex")){
        print_parse_error("Numéro de flags non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);

      if(eol(l)){
      } else {
        return 0;
      }

      return 1;

    }

    int set_filename(list_t* l){

      if(!next_lexem_is(l,"dir::set")){
        print_parse_error(".set manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);


      if(!next_lexem_is(l,"filename")){
        print_parse_error("Filename non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);


      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);


      if(!next_lexem_is(l,"string")){
        print_parse_error("Nom du fichier non précisé", l) ;
        return 0;
      }
      lexem_advance(l);

      if(eol(l)){
      } else {
        return 0;
      }

      return 1;

    }

    int set_name(list_t* l){
      if(!next_lexem_is(l,"dir::set")){
        print_parse_error(".set manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);


      if(!next_lexem_is(l,"name")){
        print_parse_error("Name non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);


      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);


      if(!next_lexem_is(l,"string")){
        print_parse_error("Name non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);

      if(eol(l)){
      } else {
        return 0;
      }

      return 1;

    }

    int set_source_size(list_t* l){
      if(!next_lexem_is(l,"dir::set")){
        print_parse_error(".set manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
        print_parse_error("esapce manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"source_size")){
        print_parse_error(".source_size non déclaré manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
        print_parse_error("esapce manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"integer::dec")){
        print_parse_error("integer::dec non déclaré", l);
        return 0;
      }
      lexem_advance(l);

      if(eol(l)){
      } else {
        return 0;
      }

      return 1;

    }

    int set_stack_size(list_t* l){

      if(!next_lexem_is(l,"dir::set")){
        print_parse_error(".set manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"stack_size")){
        print_parse_error("stack_size non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"integer::dec")){
        print_parse_error("integer::dec non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);

      if(eol(l)){
      } else {
        return 0;
      }

      return 1;
    }

    int set_arg_count(list_t* l){

      if(!next_lexem_is(l,"dir::set")){
        //print_parse_error(".set manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"arg_count")){
        print_parse_error("arg_count non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
        print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"integer::dec")){
        print_parse_error("integer::dec non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);

      if(eol(l)){
      } else {
        return 0;
      }

      return 1;
    }


    int set_kwonly_arg_count(list_t* l){

      if(!next_lexem_is(l,"dir::set")){
      //  print_parse_error(".set manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
      //  print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"kwonly_arg_count")){
      //  print_parse_error("kwonly_arg_count non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
      //  print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"integer::dec")){
      //  print_parse_error("integer::dec non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);

      if(eol(l)){
      } else {
        return 0;
      }

      return 1;

    }

    int set_posonly_arg_count(list_t* l){

      if(!next_lexem_is(l,"dir::set")){
      //  print_parse_error(".set manquant", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
      //  print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"posonly_arg_count")){
      //  print_parse_error("posonly_arg_count non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"blank")){
      //  print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
        return 0;
      }
      lexem_advance(l);

      if(!next_lexem_is(l,"integer::dec")){
      //  print_parse_error("integer::dec non déclaré", l) ;
        return 0;
      }
      lexem_advance(l);

      if(eol(l)){
      } else {
        return 0;
      }

      return 1;
    }

  int interned_strings(list_t* l){

    if(!next_lexem_is(l,"dir::interned")){
      print_parse_error(".interned manquant", l) ;
      return 0;
    }
    lexem_advance(l);

    if(eol(l)){
    } else {
      return 0;
    }
    int i =1;
    while((next_lexem_is(l, "string"))&& (i=1)){
      list_t save = *l;
      lexem_advance(l);

      if(eol(l)){
      } else {
        *l=save;
        i=0;
      }
    }

    return 1;
  }


  int constants(list_t* l){

    if(!next_lexem_is(l,"dir::consts")){
      print_parse_error(".consts manquant", l) ;
      return 0;
    }
    lexem_advance(l);

    if(eol(l)){
    } else {
      return 0;
    }

    int i=1;
    while((constant(l))&&(i=1)){
      list_t save = *l;
      lexem_advance(l);

      if(eol(l)){
      } else {
        *l=save;
        i=0;
      }
    }
    return 1;
  }

  int constant(list_t* l){

    if(!next_lexem_is(l,"integer::dec")&&!next_lexem_is(l,"integer::hex")&&!next_lexem_is(l,"float")&&!next_lexem_is(l,"string")&&!next_lexem_is(l,"pycst::None")&&!next_lexem_is(l,"pycst::True")&&!next_lexem_is(l,"pycst::False")&&!tuple(l)&&!list(l)){//&&!tuple(l)&&!list(l)
    //  print_parse_error(" pas d'integer, float, string, pycst ni tuple\n", l) ;
      return 0;
    }

    lexem_advance(l);
    return 1;
  }

  int list(list_t* l){

    if(!next_lexem_is(l,"brack::left")){
      //print_parse_error("brack::left manquant", l) ;
      return 0;
    }
    lexem_advance(l);

    if(!next_lexem_is(l,"blank")){
      print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
      return 0;
    }
    lexem_advance(l);

    while(constant(l)){
      lexem_advance(l);
    }

    if(next_lexem_is(l,"blank")){
      lexem_advance(l);
    }

    if(!next_lexem_is(l,"brack::right")){
      print_parse_error("brack::right manquant",l);
      return 0;
    }
    return 1;
  }


  int tuple(list_t* l){

    if(!next_lexem_is(l,"paren::left")){
    //  print_parse_error("paren::left manquant", l) ;
      return 0;
    }
    lexem_advance(l);

    if(!next_lexem_is(l,"blank")){
      print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
      return 0;
    }
    lexem_advance(l);

    while(constant(l)){
      lexem_advance(l);
    }

    if(next_lexem_is(l,"blank")){
      lexem_advance(l);
    }

    if(!next_lexem_is(l,"paren::right")){
      print_parse_error("paren::right manquant",l);
      return 0;
    }
    return 1;

  }

  int names(list_t* l){

    if(!next_lexem_is(l,"dir::names")){
      print_parse_error(".names manquant", l) ;
      return 0;
    }
    lexem_advance(l);

    if(eol(l)){
    } else {
      return 0;
    }
    int i =1;
    while((next_lexem_is(l, "string"))&& (i=1)){
      list_t save = *l;
      lexem_advance(l);

      if(eol(l)){
      } else {
        *l=save;
        i=0;
      }
    }

    return 1;
  }

  int varnames(list_t* l){

    if(!next_lexem_is(l,"dir::varnames")){
      //print_parse_error(".varnames manquant", l) ;
      return 0;
    }
    lexem_advance(l);

    if(eol(l)){
    } else {
      return 0;
    }
    int i =1;
    while((next_lexem_is(l, "string"))&& (i=1)){
      list_t save = *l;
      lexem_advance(l);

      if(eol(l)){
      } else {
        *l=save;
        i=0;
      }
    }

    return 1;

  }

  int freevars(list_t* l){

    if(!next_lexem_is(l,"dir::freevars")){
      print_parse_error(".freevars manquant", l) ;
      return 0;
    }
    lexem_advance(l);

    if(eol(l)){
    } else {
      return 0;
    }
    int i =1;
    while((next_lexem_is(l, "string"))&& (i=1)){
      list_t save = *l;
      lexem_advance(l);

      if(eol(l)){
      } else {
        *l=save;
        i=0;
      }
    }

    return 1;

  }

  int cellvars(list_t* l){

    if(!next_lexem_is(l,"cellvars::names")){
      //print_parse_error(".cellvars manquant", l) ;
      return 0;
    }
    lexem_advance(l);

    if(eol(l)){
    } else {
      return 0;
    }
    int i =1;
    while((next_lexem_is(l, "string"))&& (i=1)){
      list_t save = *l;
      lexem_advance(l);

      if(eol(l)){
      } else {
        *l=save;
        i=0;
      }
    }
    return 1;

  }

  int code(list_t* l){

    if(!next_lexem_is(l,"dir::text")){
      print_parse_error(".text manquant", l) ;
      return 0;
    }
    lexem_advance(l);

    if(eol(l)){
    } else {
      return 0;
    }
    int i =1;
    while((assembly_line(l))&& (i=1)){
      list_t save = list_new();
      save = *l;

      if(eol(l)){
        if(list_empty(*l)){
          return 1;
        }
      } else {
        *l=save;
        i=0;
      }
    }
    return 1;
  }

  int assembly_line(list_t* l){
    /*if(list_empty(*l)){
      exit(EXIT_SUCCESS);
    }*/
    if(!insn(l)&&!source_lineno(l)&&!label(l)){
      print_parse_error("pas de insn ni source_lineno ni label",l);
      return 0;
    }
    return 1;
  }

  int label(list_t* l){

    if(!next_lexem_is(l,"symbol")){
      print_parse_error("symbol non déclaré", l) ;
      return 0;
    }
    lexem_advance(l);
    list_t save = *l;
    save = *l;
    if(!next_lexem_is(l,"blank")){
      //print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
      *l=save;
    } else{
      lexem_advance(l);
    }

    if(!next_lexem_is(l,"colon")){
      print_parse_error("colon non déclaré", l) ;
      return 0;
    }
    lexem_advance(l);
    return 1;
  }

  int source_lineno(list_t* l){

    if(!next_lexem_is(l,"dir::line")){
      //print_parse_error(".line manquant", l) ;
      return 0;
    }
    lexem_advance(l);

    if(!next_lexem_is(l,"blank")){
      print_parse_error("Erreur de syntaxe, il manque un espace ou une tabulation", l) ;
      return 0;
    }
    lexem_advance(l);

    if(!next_lexem_is(l,"integer::dec")){
      print_parse_error(".dec non déclaré", l) ;
      return 0;
    }
    lexem_advance(l);
    return 1;
  }

  int insn(list_t* l){

    if(next_lexem_is(l,"insn::0")){
      lexem_advance(l);
      return 1;
    }
    /*else{
      print_parse_error("pas de insn::0", l) ;
    }*/
    if(!next_lexem_is(l,"insn::1")){
      return 0;
      }
    lexem_advance(l);

    if(!next_lexem_is(l,"blank")){
      return 0;
      }
    lexem_advance(l);

    if((next_lexem_is(l,"integer::dec")||(next_lexem_is(l,"symbol")))) {
      lexem_advance(l);
      return 1;
    } else{
      return 0;
    }
    /*else{
      //print_parse_error("pas de insn::1 ou de .dec ni de symbol", l) ;
    }*/
    return 1;
  }
  /*
  lexem_t lx;
  lx = list_first(*l);
  lexem_print(lx);
  printf("\n");*/
