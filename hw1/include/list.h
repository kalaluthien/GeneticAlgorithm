#ifndef __LIST_H_
#define __LIST_H_

struct list_node {
  void *elem;
  struct list_node *prev;
  struct list_node *next;
};

struct list {
  unsigned size;
  struct list_node begin;
  struct list_node end;
};

#define list_begin(l) (&(l)->begin)
#define list_end(l) (&(l)->end)

struct list *create_list();
void insert_list(struct list *l, void *elem);

#endif
