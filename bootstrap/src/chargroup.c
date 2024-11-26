#include "pyas/chargroup.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char_group* char_group_new(char o) {
  char_group *p ;

  p = calloc(1,sizeof(*p)) ;

  if(p!=NULL){
    int i;
    for(i=0;i<256;i++) p->group[i] = 0;
    p->op = o ;
  }

  return p ;
}

static void char_group_delete(char_group *c){
  if(c!=NULL){
    free(c) ;
  }
}

int char_group_delete_cb(void* content){
  char_group_delete(content);
  return EXIT_SUCCESS;
}

static void char_group_print(char_group *c) {
  int i = 0 ;
  int deb;
  printf("[") ;
  while(i<256) {
    if(c->group[i]){
      deb = i;
      i++;
      while(c->group[i]&&i<256){
        i++;
      }
      i--;
      if (i!=deb) {
        printf("%d-%d ; ", deb, i) ;
      } else {
        printf("%d ; ", i) ;
      }
      i++;
    } else {
      i++;
    }
  }
  printf("] -> %d |", c->op);
}

int char_group_print_cb(void *content){
  char_group_print(content);
  return EXIT_SUCCESS;
}

list_t char_group_parse(char* regexp_char){

  /*Initialisations*/
  unsigned char regexp = (unsigned char)(*regexp_char);
  queue_t q = queue_new() ;
  unsigned char* p = regexp_char ;
  char_group* c ;
  char neg[2] ;
  neg[0] = 0 ; neg[1] = 0 ;
  unsigned char* t;

  /*Début de la recherche de group_char*/

  while(*p!='\0'){
    switch(*p){
      case 92: /*\ : cas d'un échappement*/
        p++;
        if(*p == '\0'){
          printf("Erreur dans l'expression régulière, l'antislash n'est suivi de rien.\n") ;
          exit(EXIT_FAILURE) ;
        }
        if(*p == 'n'){
          c = char_group_new(operator(*(p+1))) ;
          c->group[10] = 1;
        } else if(*p=='t') {
          c = char_group_new(operator(*(p+1))) ;
          c->group[9] = 1;
        } else {
          c = char_group_new(operator(*(p+1))) ;
          c->group[(int)*p] = 1;
        }
        p++;
        if(operator(*p)) p++ ;
        break;
      case 94: /*^ : cas d'une négation*/
        neg[0] = 1; //on retient que le prochain group_char créé sera à inverser
        p++;
        break;
      case 91: /*[ : cas d'un intervalle de caractère*/
        t = p+1;
        char check = 1;
        /*Recherche du crochet fermant*/
        while(check && *t!='\0'){
          if(*t == ']' && *(t-1) != '\\') check = 0 ;
          t++;
        }
        if(check==1){
          printf("Erreur dans l'expression régulière, il n'y a pas de crochet fermant.\n");
          exit(EXIT_FAILURE) ;
        }
        /*Recopie dans une chaîne de caractère du contenu entre les crochets*/
        int n;
        unsigned char* h = p;
        unsigned char *croch = calloc(t-p+2,sizeof(char));
        for(n=0;n<t-p;n++){
          *(croch+n) = *h ;
          h++;
        }
        n++ ; *(croch+n) = '\0' ;

        c = char_group_new(operator(*t)) ;
        check_inter(croch,c->group);
        p=t;
        if(operator(*t)) p++;

        free(croch);
        break;
      default: /*Cas d'un caractère quelconque*/
        /*Gestion des caractères interdits lorsqu'ils sont isolés : *,+,?,]*/
        if(*p=='*' || *p == '+' || *p == '?' || *p == ']'){
          printf("Erreur dans l'expression régulière, un opérateur ne peut pas être appelé tout seul, ou doit être échappé.\n") ;
          exit(EXIT_FAILURE) ;
        }
        c = char_group_new(operator(*(p+1))) ;
        if(*p == '.'){
          negation(c) ;
        }
        c->group[(int)*p] = 1 ;
        p++;
        if(operator(*p)) p++ ;
      }

      /*Gestion de la négation*/
      if(neg[0] == 1 && neg[1] == 1){
        printf("Erreur dans l'expression régulière, deux négations d'affilée.") ;
        exit(EXIT_FAILURE) ; //si deux ^ se suivent, il faut retourner une erreur
      } else if(neg[0] == 0 && neg[1] == 1) {
        negation(c) ;
        q = enqueue(c, q) ;
        neg[1] = 0 ;
      } else if(neg[0] == 1) {
        neg[1] = 1 ;
        neg[0] = 0 ;
      } else {
        q = enqueue(c,q) ;
      }
    }

  /*Remise de la liste dans le bon sens*/
  list_t l = queue_to_list(q);
  // list_print(l,char_group_print);
  return l ;

}

char operator(char op){
  switch(op){
    case 42:
      return 1;
      break;
    case 43:
      return 2;
      break;
    case 63:
      return 3;
      break;
    default:
      return 0;
  }
}

void negation(char_group* c){
  unsigned char* p = c->group;
  while(p < c->group+256){
    *p= 1 - *p;
    p++;
  }
}

void check_inter(unsigned char* inter,unsigned char grup[256]){


  int i;
  for(i = 0; i< 256; i++){ //initilisation du char group à 0
    grup[i]=0;

  }
  if( *inter == '['){     // on saute le premier crochet ouvrant
    inter++;
  }

  while(*inter != ']'){   // tant que le morceau d'expression régulière n'est pas terminé

    if(*inter != 92){       /* si on ne rencontre pas d'antislash*/

      if(*(inter+1)=='-'){
        if(*inter > *(inter+2)){  // si l'intervalle est mal écrit
          printf("Erreur dans l'expression régulière, l'intervalle est mal défini.\n");
          exit(EXIT_FAILURE);
        }
        int k;
        for(k = *inter; k<=*(inter+2);k++){
          grup[k]=1;
        }
        inter+=2;
      }
      else{
        grup[*inter]=1;
      }
    }
    else{
      inter++;
      switch(*inter){
        case 110:
          grup[10] = 1;
          break;
        case 116:
          grup[9] = 1;
          break;
        default:
          grup[*inter]=1;
          break;
      }

    }
    inter++;
  }
}
