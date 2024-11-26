

/*
 * @file queue.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Jul  2 19:02:34 2021
 * @brief Queue.
 *
 * Queue.
*/

#ifndef _QUEUE_H_
#define _QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pyas/list.h>  /* list_t */
#include <pyas/callbacks.h>

// 1. le type fifo_t est défini comme "pointeur sur maillon"

typedef struct link_t {
  void  *content; /* un élément de la liste*/
  struct link_t *next; /* l'adresse du maillon suivant */
} * queue_t;

  queue_t queue_new( void );
  /*
  queue_t queue_del_first(queue_t q, action_t delete);
  queue_t queue_delete(queue_t q, action_t delete);
  */
  queue_t enqueue(void *content, queue_t q_ref);
  void* dequeue(queue_t * p_queue, action_t delete);
  void* queue_peek(queue_t q);
  int     queue_is_empty( queue_t q );
  list_t  queue_to_list( queue_t q );
  void queue_print(queue_t q_ref, action_t print);
  int queue_count(queue_t q);


#ifdef __cplusplus
}
#endif

#endif  /*_QUEUE_H_*/
