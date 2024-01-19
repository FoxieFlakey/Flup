#ifndef header_1703927662_e8d6a758_cbf1_44e3_85b5_4c0ebfcb26fe_foreach_h
#define header_1703927662_e8d6a758_cbf1_44e3_85b5_4c0ebfcb26fe_foreach_h

// nextBlock is type of bool (^)(itemType* item)
// return false if nothing else to continue else 
// true and Block_release won't be called
#define flup_foreach(itemType, itemName, nextBlock) \
  for (struct {itemType item; int onced; bool (^next)(itemType*);} __flup_foreach_state_ ## itemName = {.item = {}, .onced = false, .next = (nextBlock) }; \
       __flup_foreach_state_ ## itemName.next(&__flup_foreach_state_ ## itemName.item);\
       __flup_foreach_state_ ## itemName.onced = false) \
       for (itemType itemName = __flup_foreach_state_ ## itemName.item; \
            !__flup_foreach_state_ ## itemName.onced; \
            __flup_foreach_state_ ## itemName.onced = true)

// Same as above but with init
#define flup_foreach_with_init(itemType, itemName, nextBlock, ...) \
  for (struct {itemType item; int onced; bool (^next)(itemType*);} __flup_foreach_state_ ## itemName = {.item = (__VA_ARGS__), .onced = false, .next = (nextBlock) }; \
       __flup_foreach_state_ ## itemName.next(&__flup_foreach_state_ ## itemName.item);\
       __flup_foreach_state_ ## itemName.onced = false) \
       for (itemType itemName = __flup_foreach_state_ ## itemName.item; \
            !__flup_foreach_state_ ## itemName.onced; \
            __flup_foreach_state_ ## itemName.onced = true)

#endif

