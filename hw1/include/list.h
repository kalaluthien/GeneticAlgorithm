#ifndef __LIST_H_
#define __LIST_H_

struct list_node {
  void *item;
  struct list_node *prev;
  struct list_node *next;
};

struct list {
  int size;
  struct list_node begin;
  struct list_node end;
};

struct list *create_list();
void delete_list(struct list *l);

int list_empty(struct list *l);
int list_length(struct list *l);

void append_list(struct list *l, void *item);
void *remove_list(struct list *l);
void *head_list(struct list *l);

void push_list(struct list *l, void *item);
void *pop_list(struct list *l);
void *tail_list(struct list *l);

void *index_list(struct list *l, int i);

#endif
