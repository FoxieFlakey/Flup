#include "flup/hashing/hash_func.h"
#define _GNU_SOURCE
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <sys/mman.h>
#include <mimalloc.h>
#include <stdbool.h>
#include <locale.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include "flup/data_structs/string_map.h"
#include "flup/hashing/noncrypto/xxhash.h"
#include "flup/data_structs/dyn_array.h"
#include "flup/data_structs/buffer.h"
#include "flup/data_structs/tree/btree.h"
#include "flup/thread/thread.h"
#include "flup/attributes.h"
#include "flup/core/logger.h"
#include "flup/core/panic.h"
#include "flup/bug.h"

#include "main.h"

static void mimalloc_play();
FLUP_PUBLIC
int fluffedup_main(FLUP_UNUSED int argc, FLUP_UNUSED const char** argv) {
  // Main thread is not attached by default
  if (flup_attach_thread("Main-Thread") == NULL)
    flup_panic("Cannot attach thread!");
  
  pr_info("Hello World!");
  
  enum command {
    COMMAND_SAY,
    COMMAND_SHUTDOWN,
    COMMAND_LOWERCASE,
    COMMAND_UPPERCASE
  };
  
  struct command_packet {
    enum command cmd;
    char* arg;
  };
  struct command_result {
    int status;
  };
  
  flup_buffer* commandBuffer = flup_buffer_new(sizeof(struct command_packet) * 32);
  flup_buffer* completionBuffer = flup_buffer_new(sizeof(struct command_result) * 32);
  
  flup_thread* thread = flup_thread_new_with_block(^() {
    struct command_packet packet;
    struct command_result result;
    
    do {
      flup_buffer_read_no_fail(commandBuffer, &packet, sizeof(packet));
      
      int ret = 0;
      switch (packet.cmd) {
        case COMMAND_SAY:
          printf("[String Processor] Received command to say the string\n");
          printf("[String Processor] Saying '%s'\n", packet.arg);
          break;
        case COMMAND_LOWERCASE:
          printf("[String Processor] Received command to lowercase a string\n");
          for (size_t i = 0; packet.arg[i] != '\0'; i++)
            packet.arg[i] = (char) tolower(packet.arg[i]);
          printf("[String Processor] Processed with succes!\n");
          ret = 0;
          break;
        case COMMAND_UPPERCASE:
          printf("[String Processor] Received command to uppercase a string\n");
          for (size_t i = 0; packet.arg[i] != '\0'; i++)
            packet.arg[i] = (char) toupper(packet.arg[i]);
          printf("[String Processor] Processed with succes!\n");
          ret = 0;
          break;
        case COMMAND_SHUTDOWN:
          printf("[String Processor] Received shutdown command, shutting down bye!\n");
          break;
      }
      
      result.status = ret;
      flup_buffer_write_no_fail(completionBuffer, &result, sizeof(result));
    } while (packet.cmd != COMMAND_SHUTDOWN);
  });
  
  struct command_packet cmd;
  struct command_result result;
  
  char str[] = "Hello World!";
  
  cmd = (struct command_packet) {COMMAND_LOWERCASE, str};
  flup_buffer_write_no_fail(commandBuffer, &cmd, sizeof(cmd));
  printf("[Parent] Sent command to lower case a string\n");
  
  flup_buffer_read_no_fail(completionBuffer, &result, sizeof(result));
  printf("[Parent] Lower cased string, result is %s\n", str);
  
  cmd = (struct command_packet) {COMMAND_UPPERCASE, str};
  flup_buffer_write_no_fail(commandBuffer, &cmd, sizeof(cmd));
  printf("[Parent] Sent command to upper case a string\n");
  
  flup_buffer_read_no_fail(completionBuffer, &result, sizeof(result));
  printf("[Parent] Upper cased string, result is %s\n", str);
  
  cmd = (struct command_packet) {COMMAND_SHUTDOWN, NULL};
  flup_buffer_write_no_fail(commandBuffer, &cmd, sizeof(cmd));
  printf("[Parent] Sent shutdown command\n");
  
  flup_buffer_read_no_fail(completionBuffer, &result, sizeof(result));
  printf("[Parent] Shutdown successful\n");
  
  flup_thread_free(thread);
  flup_buffer_free(commandBuffer);
  flup_buffer_free(completionBuffer);
  // mimalloc_play();
  
  flup_dyn_array* a = flup_dyn_array_new(sizeof(int), 5);
  int e = 0;
  int ret;
  
  for (int i = 0; i < 5; i++) {
    ret = flup_dyn_array_append(a, &e);
    BUG_ON(ret < 0);
  }
  
  ret = flup_dyn_array_remove(a, 0, 5);
  BUG_ON(ret < 0);
  
  flup_dyn_array_free(a);
  
  flup_btree* tree = flup_btree_new();
  uintptr_t tmp;
  int res;
  
  flup_btree_set(tree, 0, 0x8084);
  res = flup_btree_get(tree, 0, &tmp);
  pr_info("Res: %d  Fetched: 0x%" PRIxPTR, res, tmp);
  
  flup_btree_set(tree, 129874, 0x1279);
  res = flup_btree_get(tree, 129874, &tmp);
  pr_info("Res: %d  Fetched: 0x%" PRIxPTR, res, tmp);
  
  flup_btree_free(tree);
  
  const char* string = "Im just a cute fox and perhaps a furry ^w^ *fuwa*";
  flup_hash128 quadWordHash = flup_xxhash3_hash_128bits(string, strlen(string));
  uint64_t doubleWordHash = flup_xxhash3_hash_64bits(string, strlen(string));
  pr_info("Resulting XXH3 128 bits hash: 0x%" PRIx64 "%" PRIx64, (uint64_t) (quadWordHash >> 64), (uint64_t) (quadWordHash & 0xFFFF'FFFF'FFFF'FFFF));
  pr_info("Resulting XXH3 64  bits hash: 0x%" PRIx64, doubleWordHash);
  
  flup_string_map* map = flup_string_map_new();
  flup_string_map_set(map, "UwU1", "Hiiii ^w^");
  flup_string_map_set(map, "UwU2", "Hiiii222 ^w^");
  flup_string_map_set(map, "UwU", "Fuwa fuwa");
  
  const char* stringRes;
  flup_string_map_get(map, "UwU1", (void**) &stringRes);
  pr_info("map['UwU1'] = %s", stringRes);
  
  flup_string_map_get(map, "UwU2", (void**) &stringRes);
  pr_info("map['UwU2'] = %s", stringRes);
  
  flup_string_map_get(map, "UwU", (void**) &stringRes);
  pr_info("map['UwU'] = %s", stringRes);
  
  flup_string_map_remove(map, "UwU", NULL);
  
  ret = flup_string_map_get(map, "UwU", (void**) &stringRes);
  pr_info("map['UwU'] = %s", ret == -ENOENT ? "<Not found>" : stringRes);
  
  flup_string_map_free(map);
  
  // This thread is attached so try detach
  // and free the handle
  flup_thread_free(flup_detach_thread());
  // flup_panic("Panic test");
  return 0;
}

static bool visitFunc(const mi_heap_t*,
  const mi_heap_area_t*,
  void* block,
  size_t,
  void*)
{
  if (!block)
    return true;
    
  printf("Block: %s\n", (const char*) block);
  return true;
}

[[maybe_unused]]
static void mimalloc_play() {
  mi_option_enable(mi_option_limit_os_alloc);
  mi_option_enable(mi_option_show_errors);
  
  // 64 megabyte backing memory
  size_t heapSize = 64 * 1024 * 1024;
  void* heapMem = mmap((void*) 0x329200000000,
    heapSize,
    PROT_NONE,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE,
    -1,
    0);
  assert(heapMem != MAP_FAILED);
  
  printf("Heap mem at %p to %p\n", heapMem, &((char*) heapMem)[heapSize - 1]);
  
  mi_arena_id_t arena;
  bool ret = mi_manage_os_memory_ex(heapMem,
    heapSize, 
    false,
    false,
    false,
    -1,
    true,
    &arena);
  assert(ret == true);
  
  {
    size_t size;
    void* ptr = mi_arena_area(arena, &size);
    printf("Arena id %d at %p sized %zu\n", arena, ptr, size);
  }
  
  mi_heap_t* heap = mi_heap_new_in_arena(arena);
  assert(heap != NULL);
  
  void* str = mi_heap_strdup(heap, "Hello im foxie UwU");
  assert(str != NULL);
  
  str = mi_heap_strdup(heap, "Hello im bad foxie UwU");
  assert(str != NULL);
  mi_free(str);
  
  mi_heap_visit_blocks(heap, true, visitFunc, NULL);
  
  mi_heap_delete(heap);
}
