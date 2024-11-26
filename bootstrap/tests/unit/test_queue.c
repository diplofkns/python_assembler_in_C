#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pyas/queue.h"
#include "pyas/chargroup.h"
#include "pyas/lexem.h"

main(){
  queue_t q = queue_new();

  char_group* c = char_group_new(1);

  q = enqueue(c,q) ;

  q = enqueue(c,q);

  char_group* c2 = char_group_new(2);
  q = enqueue(c2,q);

  char_group* c3 = char_group_new(3);
  q = enqueue(c3,q);
  q = enqueue(c,q);
  q = enqueue(c3,q);

  queue_print(q,char_group_print_cb);

  printf("----------------------\n");

  dequeue(&q,char_group_delete_cb);

  queue_print(q,char_group_print_cb);

  // list_t l = queue_to_list(q);
  //
  // list_print(l,char_group_print_cb);

}
