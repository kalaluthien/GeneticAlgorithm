#include "common.h"
#include "list.h"

struct list *create_list() {
  struct list *l =
    (struct list *) malloc(sizeof(struct list));

  l->size = 0;
  l->begin.prev = NULL;
  l->begin.next = &l->end;
  l->end.prev = &l->begin;
  l->end.next = NULL;

  return l;
}

void insert_list(struct list *l, void *elem) {
  struct list_node *node =
    (struct list_node *) malloc(sizeof(struct list_node));

  node->elem = elem;

  struct list_node *end = list_end(l);

  node->prev = end->prev;
  end->prev->next = node;

  end->prev = node;
  node->next = end;

  l->size++;
}
