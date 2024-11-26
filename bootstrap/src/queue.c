
/**
 * @file queue.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  2 19:02:17 2021
 * @brief Queue.
 *
 * Queue.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <pyas/queue.h>

queue_t queue_new( void ) {
  return NULL;
}

/*
queue_t queue_del_first(queue_t q, action_t delete)
{
  queue_t next;
  assert(!queue_is_empty(q));
  next = q->next;
  if (NULL != delete)
  {
    delete (q->content);
  }
  free(q);
  return next;
}
*/
queue_t queue_delete(queue_t q,action_t delete)
{
  while(!queue_is_empty(q))
  {
    void* l = dequeue(q, delete);
    delete(l);
  }

  return q;
}


queue_t enqueue(void *content, queue_t q_ref) {
  queue_t q = calloc(1, sizeof(*q));
  if(q == NULL) {
    return NULL ;
  }
  q->content = content;
  if(queue_is_empty(q_ref)) {
    q->next = q ;
    return q;
  }
  q->next = q_ref->next ;
  q_ref->next = q ;
  return q;
}


void* dequeue(queue_t * p_queue, action_t delete){
  assert(!queue_is_empty(*p_queue));

  queue_t q = (*p_queue)->next;
  void* element = q->content;

  if((*p_queue)->next == *p_queue){
    *p_queue = NULL;
  } else {
    (*p_queue)->next = q->next;
  }

  free(q);

  return element;
}

void* queue_peek(queue_t q) {

  assert(!queue_is_empty(q));

  void* element = (q->next)->content;

  return element;

}

int queue_is_empty( queue_t q ) {

  if (q == NULL)
  {
    return 1;
  }

  return 0;
}

list_t  queue_to_list( queue_t q ) {

  // list_t l = q;
  // queue_t p = q;
  // if(q==NULL) return l;
  //
  // while(p->next != q) p = p->next;
  // p->next = NULL;
  //
  // return l;
  if(queue_is_empty(q)) return NULL;
  list_t l = list_new();
  l = q->next; // list pointe sur le premier élément (la tete)
  q->next = NULL; //on enleve la circularité

  return l;
}

void queue_print(queue_t q_ref, action_t print) {

  // assert(!queue_is_empty(q_ref));
  if(queue_is_empty(q_ref)){
    printf("()\n");
  } else {

    queue_t q = q_ref->next;

    do {
      print(q->content);
      printf("; \n");
      q = q->next;
    } while(q != q_ref->next);
  }

}

int queue_count(queue_t q) {

  if(queue_is_empty(q)) return NULL;

  queue_t p_queue = q;
  int nb_element = 0;

  do {
    p_queue = (p_queue)->next;
    nb_element ++ ;
  } while (p_queue != q);

  return nb_element;
}
