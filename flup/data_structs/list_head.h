#ifndef header_1703856937_6fc70e5b_0704_4883_81d7_9cbb27af2bc5_linked_flup_list_h
#define header_1703856937_6fc70e5b_0704_4883_81d7_9cbb27af2bc5_linked_flup_list_h

#include <stddef.h>

#include "flup/container_of.h"
#include "flup/bug.h"

// Adapted from Linux kernel

// This were meant to be embedded inside another
// structure
typedef struct flup_list_head {
  struct flup_list_head* next;
  struct flup_list_head* prev;
} flup_list_head;

static inline void flup_list_head_init(struct flup_list_head* list) {
  list->next = list;
  list->prev = list;
}

#define FLUP_LIST_HEAD_INIT(x) {.next = &x, .prev = &x}

#define flup_list_is_valid(head) ((head)->next != ((void*) 0xDEADBEE1) && (head)->next != NULL)

static inline void __flup_list_add(struct flup_list_head *new, struct flup_list_head *prev, struct flup_list_head *next) {
  next->prev = new;
  new->next = next;
  new->prev = prev;
  prev->next = new;
}

static inline void __flup_list_del(struct flup_list_head* prev, struct flup_list_head* next) {
  BUG_ON(!flup_list_is_valid(prev));
  BUG_ON(!flup_list_is_valid(next));
  next->prev = prev;
  prev->next = next;
}

static inline void flup_list_add_head(struct flup_list_head* head, struct flup_list_head* new) {
	__flup_list_add(new, head, head->next);
}

static inline void flup_list_add_tail(struct flup_list_head* head, struct flup_list_head* new) {
	__flup_list_add(new, head->prev, head);
}

static inline void flup_list_init_as_invalid(struct flup_list_head* entry) {
  entry->next = (void*) 0xDEADBEE1;
	entry->prev = (void*) 0xDEADBEE2;
}

static inline void flup_list_del(struct flup_list_head* entry) {
  BUG_ON(!flup_list_is_valid(entry));

  __flup_list_del(entry->prev, entry->next);
  flup_list_init_as_invalid(entry);
}

#define flup_list_first(head) ((head)->next)
#define flup_list_last(head) ((head)->prev)
#define flup_list_is_empty(head) ((head)->next == (head))
#define flup_list_is_head(head, node) ((node) == (head))
#define flup_list_entry(nodePtr, type, member) container_of(nodePtr, type, member)
#define flup_list_first_entry(head, type, member) flup_list_entry(flup_list_first(head), type, member)
#define flup_list_last_entry(head, type, member) flup_list_entry(flup_list_last(head), type, member)

#define flup_list_for_each(head, current) \
  for (current = (head)->next; \
       !flup_list_is_head(current, (head)); \
       current = current->next)
#define flup_list_for_each_safe(head, current, nextVar) \
	for (current = (head)->next, nextVar = current->next; \
	     !flup_list_is_head(current, (head)); \
	     current = nextVar, nextVar = current->next)

#endif

