#ifndef header_1703856937_6fc70e5b_0704_4883_81d7_9cbb27af2bc5_linked_flup_list_h
#define header_1703856937_6fc70e5b_0704_4883_81d7_9cbb27af2bc5_linked_flup_list_h

/**
 * @file
 * @copydoc flup_list_head
 */

#include <stddef.h>

#include "flup/container_of.h"
#include "flup/bug.h"

/**
 * @brief Simple header only doubly linked list
 *
 * This is adapted from Linux kernel and to use
 * this embed @ref flup_list_head into an struct
 * which want to be used as node in a list.
 *
 * Used in a struct like
 * @code{.c}
 * struct device {
 *   flup_list_head node;
 *   // <other data>
 * };
 * @endcode
 *
 * This useful to avoid inefficiency of generic
 * linked list for objects which will only ever
 * appear in a list at any moment.
 *
 * To retrieve the item itself you can use
 * @ref flup_list_entry on the node pointer
 * @code{.c}
 * struct device* dev = flup_list_entry(node, struct device, node);
 * @endcode
 */
typedef struct flup_list_head {
  /// Points to next node
  struct flup_list_head* next;
  /// Points to previous node
  struct flup_list_head* prev;
} flup_list_head;

/**
 * @brief Initialize @p list
 * @public @memberof flup_list_head
 *
 * Initialize list so that it points to itself
 *
 * @param list List to be initialize
 */
static inline void flup_list_head_init(struct flup_list_head* list) {
  list->next = list;
  list->prev = list;
}

/**
 * @brief Used for initializing in a initialization expression.
 *
 * Its used to initialize @ref flup_list_head in
 * @code{.c}
 * struct list_of_lists {
 *   flup_list_head listA;
 *   flup_list_head listB;
 *   flup_list_head listC;
 * };
 * 
 * struct list_of_lists listOfLists = {
 *   .listA = FLUP_LIST_HEAD_INIT(listOfLists.listA),
 *   .listB = FLUP_LIST_HEAD_INIT(listOfLists.listB),
 *   .listC = FLUP_LIST_HEAD_INIT(listOfLists.listC)
 * };
 * @endcode
 *
 * @param x The list
 *
 * @return Initializer for list
 */
#define FLUP_LIST_HEAD_INIT(x) {.next = &x, .prev = &x}

/**
 * @brief Check if @p head is valid
 *
 * Check if the @ref flup_list_head.next and
 * if @ref flup_list_head.prev is not a NULL.
 *
 * @note This is not 100% accurate, its served for quick
 *   bug check if a poisoned list is used without initializing
 *
 * @param head Head to be checked
 *
 * @return bool value whether its valid or not
 */
#define flup_list_is_valid(head) ((head)->prev != NULL && (head)->next != NULL)

/// @cond
static inline void flup_list__add(struct flup_list_head *new, struct flup_list_head *prev, struct flup_list_head *next) {
  next->prev = new;
  new->next = next;
  new->prev = prev;
  prev->next = new;
}

static inline void flup_list__del(struct flup_list_head* prev, struct flup_list_head* next) {
  BUG_ON(!flup_list_is_valid(prev));
  BUG_ON(!flup_list_is_valid(next));
  next->prev = prev;
  prev->next = next;
}
/// @endcond

/**
 * @brief Add @p new node into head of @p head
 * @public @memberof flup_list_head
 *
 * @param head The list to be added to
 * @param new The node to be added
 */
static inline void flup_list_add_head(struct flup_list_head* head, struct flup_list_head* new) {
	flup_list__add(new, head, head->next);
}

/**
 * @brief Add @p new node into tail of @p head
 * @public @memberof flup_list_head
 *
 * @param head The list to be added to
 * @param new The node to be added
 */
static inline void flup_list_add_tail(struct flup_list_head* head, struct flup_list_head* new) {
	flup_list__add(new, head->prev, head);
}

/**
 * @brief Mark @p head as invalid
 * @public @memberof flup_list_head
 *
 * @param head The list to be marked as invalid
 */
static inline void flup_list_init_as_invalid(struct flup_list_head* head) {
  head->next = NULL;
	head->prev = NULL;
}

/**
 * @brief Delete a @p node
 * @public @memberof flup_list_head
 *
 * @param node Node to be deleted/unlinked
 */
static inline void flup_list_del(struct flup_list_head* node) {
  BUG_ON(!flup_list_is_valid(node));

  flup_list__del(node->prev, node->next);
  flup_list_init_as_invalid(node);
}

/**
 * @brief Get first node
 *
 * @param head The head of list
 * @return @ref flup_list_head Pointer to first node
 */
#define flup_list_first(head) ((head)->next)

/**
 * @brief Get last node
 *
 * @param head The head of list
 * @return @ref flup_list_head Pointer to last node
 */
#define flup_list_last(head) ((head)->prev)

/**
 * @brief Check if list empty
 *
 * @param head List's head to check
 * @return true if empty else false if not empty
 */
#define flup_list_is_empty(head) ((head)->next == (head))

/**
 * @brief Check if node is head
 *
 * @param head The head of list
 * @param node Node to check with
 *
 * @return true if @p node is head and false if not head
 */
#define flup_list_is_head(head, node) ((node) == (head))

/**
 * @brief Get entry from @p nodePtr
 *
 * @param nodePtr Pointer to node
 * @param type The type of entry
 * @param member The member which the @ref flup_list_head reside
 *
 * @return The list entry casted to @p type
 */
#define flup_list_entry(nodePtr, type, member) container_of(nodePtr, type, member)

/**
 * @brief Get first entry
 *
 * @param head The head of list
 * @param type The type of entry
 * @param member The member which the @ref flup_list_head reside
 *
 * @return The first list entry casted to @p type
 */
#define flup_list_first_entry(head, type, member) flup_list_entry(flup_list_first(head), type, member)

/**
 * @brief Get last entry
 *
 * @param head The head of list
 * @param type The type of entry
 * @param member The member which the @ref flup_list_head reside
 *
 * @return The last list entry casted to @p type
 */
#define flup_list_last_entry(head, type, member) flup_list_entry(flup_list_last(head), type, member)

/**
 * @brief Iterate list (move/deletion unsafe)
 *
 * Behaves like 'for' syntax.
 *
 * @warning This macro is unsafe if you are modifying
 *   the list structure like moving, deleting, and
 *   swapping any entry.
 *
 * @param head Variable name for head of list
 * @param current Variable name for current entry
 */
#define flup_list_for_each(head, current) \
  for (current = (head)->next; \
       !flup_list_is_head(current, (head)); \
       current = current->next)

/**
 * @brief Iterate list (move/deletion unsafe)
 *
 * Behaves like 'for' syntax but safe for moving,
 * deleting and swapping list entry.
 *
 * @param head Variable name for head of list
 * @param current Variable name for current entry
 * @param nextVar Variable name for storing next entry
 */
#define flup_list_for_each_safe(head, current, nextVar) \
	for (current = (head)->next, nextVar = current->next; \
	     !flup_list_is_head(current, (head)); \
	     current = nextVar, nextVar = current->next)

#endif

