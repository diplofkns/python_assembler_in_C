#include <stdio.h>
#include <stdlib.h>

struct label{
  char* value;
  int position;
  char rel;
};

typedef struct label *label_t;

int label_delete( void *_lab );
int label_print( void *_lab );
label_t label_new(char* value, int pos, int rel);
