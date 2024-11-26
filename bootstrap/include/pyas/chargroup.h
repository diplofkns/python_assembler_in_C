#include "pyas/list.h"
#include "pyas/queue.h"

typedef struct {
  unsigned char group[256] ;
  char op ;
  /*
  op = ...
    0 -> une seule fois
    1 -> *, 0 fois ou plus
    2 -> +, 1 fois ou plus
    3 -> ?, 0 ou une fois
  */
} char_group;


char_group* char_group_new(char o);

int char_group_delete_cb(void* content);

int char_group_print_cb(void *content);

list_t char_group_parse(char* regexp) ;

char operator(char op) ;

void negation(char_group* c);

void check_inter(unsigned char* inter,unsigned char grup[]);

void array_copy(unsigned char* k, unsigned char m[]);
