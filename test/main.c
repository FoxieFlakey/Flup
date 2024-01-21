#include <stdio.h>
#include <stdlib.h>
#include <stdbit.h>
#include <stdckdint.h>

#include "flup/attributes.h"
#include "flup/container_of.h"
#include "flup/data_structs/linked_list.h"
#include "flup/data_structs/squeue.h"
#include "flup/interface/ilist.h"
#include "flup/util/iterator.h"
#include "flup/data_structs/dyn_array.h"

#include "main.h"

FLUP_PUBLIC
int fluffedup_main(FLUP_UNUSED int argc, FLUP_UNUSED const char** argv) {
  printf("C23 stdbit.h test\n");
  printf("Bit ceil: %u to %u\n", 43, stdc_bit_ceil_ui(43));
  printf("Bit floor: %u to %u\n", 43, stdc_bit_floor_ui(43));
  
  {
    printf("linked list test\n");
    
    flup_linked_list* list = flup_linked_list_new(sizeof(int));
    flup_linked_list_add_head(list, &(int) {123});
    flup_linked_node* node = flup_linked_list_add_head(list, &(int) {124});
    flup_linked_list_add_tail(list, &(int) {122});
   
    flup_iterator_state* iterator = flup_linked_list_get_iterator(list);
    flup_iterator_foreach(int, x, iterator)
      printf("1st data iteration: %d, has next %d\n", x, iterator->ops->hasNext(iterator));
    
    flup_linked_list_del(list, node);
    iterator->ops->reset(iterator);

    flup_iterator_foreach(int, x, iterator)
      printf("2nd data iteration: %d, has next %d\n", x, iterator->ops->hasNext(iterator));
    iterator->ops->free(iterator);

    flup_linked_list_free(list);
  }

  {
    printf("squeue test\n");
    
    flup_squeue* queue = flup_squeue_new(33, 0);
    struct message {
      flup_squeue_item super;
      int integer;
    };

    struct message* msg1 = malloc(sizeof(*msg1));
    *msg1 = (struct message) {
      .super = { FLUP_SQUEUE_ITEM_DEFAULTS },
      .super.dealloc = (void*) free,
      .integer = 106
    };

    struct message* msg2 = malloc(sizeof(*msg2));
    *msg2 = *msg1;
    msg2->integer = 200;
    
    struct message* msg3 = malloc(sizeof(*msg3));
    *msg3 = *msg1;
    msg3->integer = 301;
    
    struct message* msg4 = malloc(sizeof(*msg4));
    *msg4 = *msg1;
    msg4->integer = 407;
    
    struct message* msg5 = malloc(sizeof(*msg5));
    *msg5 = *msg1;
    msg5->integer = 502;

    flup_squeue_enqueue(queue, &msg1->super);
    flup_squeue_enqueue(queue, &msg2->super);
    flup_squeue_enqueue(queue, &msg3->super);
    flup_squeue_enqueue(queue, &msg4->super);
    flup_squeue_enqueue(queue, &msg5->super);
    
    while (queue->length > 0) {
      flup_squeue_item* dequeuedMsg = flup_squeue_dequeue_filtered_blocks(queue, ^bool (flup_squeue_item* item) {
            struct message* msg = container_of(item, struct message, super);
            // Only if last digit is smaller than 5
            return msg->integer % 10 < 5;
          });
      if (dequeuedMsg) {
        struct message* msg = container_of(dequeuedMsg, struct message, super);
        printf("Filtered dequeue: %d\n", msg->integer);
        continue;
      }

      dequeuedMsg = flup_squeue_try_dequeue(queue);
      struct message* msg = container_of(dequeuedMsg, struct message, super);
      printf("Non filtered dequeue: %d\n", msg->integer);
    }

    flup_squeue_free(queue);
  }

  {
    printf("Dynamic array test\n");
    flup_ilist* list = &flup_dyn_array_new(sizeof(int), 3)->interface.IList;
    int n;
    n = 2025;
    list->ops->append(list, &n);
    n = 2026;
    list->ops->append(list, &n);
    n= 8086;
    list->ops->prepend(list, &n);
    
    srand(9096);

    for (int i = 0; i < 5; i++) {
      n = rand();
      list->ops->append(list, &n);
    }
    n = 2024;
    list->ops->append(list, &n);
    // Delete the appended "2025" and "2026" from earlier
    list->ops->remove(list, 1, 2);

    for (unsigned int i = 0; i < list->ops->length(list); i++) {
      int* addr;
      list->ops->get(list, i, (void**) &addr);
      printf("arr[%d] = %d\n", i, *addr);
    }

    list->ops->dealloc(list);
  }
  return 0;
}

