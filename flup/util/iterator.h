#ifndef header_1703918869_21e715cb_968d_4b3b_a989_515a06361e97_iterator_h
#define header_1703918869_21e715cb_968d_4b3b_a989_515a06361e97_iterator_h

typedef struct flup_iterator_state flup_iterator_state;

#define FLUP_ITERATOR_OPS_DEFAULT 
typedef struct flup_iterator_ops {
  // Advance current iterator state
  bool (*next)(flup_iterator_state* self);

  // Check if current iterator can advance
  bool (*hasNext)(flup_iterator_state* self);

  // Release resources used by current iterator
  void (*free)(flup_iterator_state* self);
  
  // Reset current iterator to start at beginning
  // 0 if success, -errno on error
  int (*reset)(flup_iterator_state* self);
} flup_iterator_ops;

struct flup_iterator_state {
  const flup_iterator_ops* ops;
  
  // Current position in iteration
  void* current;

  // 0 if loop quit isn't caused by an error
  // Positive is plain ISO C's errno
  // Negative is Flup specific error codes
  int errorCode;
};

#define flup_iterator_foreach(itemType, itemName, iterator) \
  for (struct {flup_iterator_state* state; int onced;} __flup_iterator_state_ ## itemName = {.state = (iterator), .onced = false}; \
       __flup_iterator_state_ ## itemName.state->ops->next(__flup_iterator_state_ ## itemName.state); \
       __flup_iterator_state_ ## itemName.onced = false) \
       for (itemType itemName = *(itemType*) __flup_iterator_state_ ## itemName.state->current; \
            !__flup_iterator_state_ ## itemName.onced; \
            __flup_iterator_state_ ## itemName.onced = true)

#define flup_iterator_foreach_ptr(itemType, itemName, iterator) \
  for (struct {flup_iterator_state* state; int onced;} __flup_iterator_state_ ## itemName = {.state = (iterator), .onced = false}; \
       __flup_iterator_state_ ## itemName.state->ops->next(__flup_iterator_state_ ## itemName.state); \
       __flup_iterator_state_ ## itemName.onced = false) \
       for (itemType* itemName = (itemType*) __flup_iterator_state_ ## itemName.state->current; \
            !__flup_iterator_state_ ## itemName.onced; \
            __flup_iterator_state_ ## itemName.onced = true)

#endif




