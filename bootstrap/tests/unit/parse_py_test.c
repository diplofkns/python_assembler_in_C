#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pyas/parse_py.h"
#include "pyas/queue.h"

main(){

  list_t lx = lex("tests/banque_py/pys/test1.pys","files/regexp_file_pyt3.txt");
  pyobj_t p = pyobj_parse(&lx,"files/regexp_file.txt");
  //
  // // pyobj_t p1 = pyobj_new(1);
  // // queue_t q = queue_new();
  // // q = enqueue(p1,q);
  // // dequeue(&q,pyobj_delete);
  //
  // // queue_print(q,pyobj_print);
  //
  pyobj_print(p);

  list_delete(lx,lexem_delete);

  exit(EXIT_SUCCESS);
}
