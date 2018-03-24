#ifndef __LIST_H_
#define __LIST_H_

struct list_node {
  void *item;
  struct list_node *prev;
  struct list_node *next;
};

struct list {
  struct list_node begin;
  struct list_node end;
};

struct list *create_list();
void delete_list(struct list *l);

int list_empty(struct list *l);

void push_list(struct list *l, void *item);
void *pop_list(struct list *l);
void *shift_list(struct list *l);

void *get_list(struct list *l, int i);
void *get_list_fst(struct list *l);
void *get_list_lst(struct list *l);

#endif
