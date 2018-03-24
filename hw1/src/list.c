#include "common.h"
#include "list.h"

#define list_begin(l) (&(l)->begin)
#define list_end(l) (&(l)->end)

#define check_list_not_empty(l) \
  do { \
    if (list_begin(l) == list_end(l)) { \
      throw("list is empty."); \
    } \
  } while (0)

struct list *create_list() {
  struct list *l =
    (struct list *) malloc(sizeof(struct list));

  if (l == NULL) {
    throw("faild to allocate list.");
  }

  struct list_node *begin = list_begin(l);
  struct list_node *end = list_end(l);

  begin->prev = NULL;
  begin->next = end;
  end->prev = begin;
  end->next = NULL;

  return l;
}

void delete_list(struct list *l) {
  struct list_node *begin = list_begin(l);
  struct list_node *end = list_end(l);
  struct list_node *curr = begin;

  while (curr->next != end) {
    struct list_node *new = curr->next;
    free(curr);
    curr = new;
  }

  free(l);
}

int list_empty(struct list *l) {
  return list_begin(l) == list_end(l);
}

void push_list(struct list *l, void *item) {
  struct list_node *end = list_end(l);
  struct list_node *new =
    (struct list_node *) malloc(sizeof(struct list_node));

  if (new == NULL) {
    throw("faild to allocate list node.");
  }

  new->prev = end->prev;
  end->prev->next = new;
  end->prev = new;
  new->next = end;

  new->item = item;
}

void *pop_list(struct list *l) {
  check_list_not_empty(l);

  struct list_node *end = list_end(l);
  struct list_node *old = end->prev;

  end->prev = old->prev;
  old->prev->next = end;

  void *item = old->item;
  free(old);

  return item;
}

void *shift_list(struct list *l) {
  check_list_not_empty(l);

  struct list_node *begin = list_begin(l);
  struct list_node *old = begin->next;

  begin->next = old->next;
  old->next->prev = begin;

  void *item = old->item;
  free(old);

  return item;
}

void *get_list(struct list *l, int i) {
  throw("Unimplemented!");

  return NULL;
}

void *get_list_fst(struct list *l) {
  check_list_not_empty(l);

  return list_begin(l)->next->item;
}

void *get_list_lst(struct list *l) {
  check_list_not_empty(l);

  return list_end(l)->prev->item;
}
