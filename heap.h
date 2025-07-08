#ifndef HEAP_H_
#define HEAP_H_

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#define TODO()                                                                 \
  do {                                                                         \
    fprintf(stderr, "%s:%d: TODO: %s not yet implemented\n", __FILE__,         \
            __LINE__, __func__);                                               \
    abort();                                                                   \
  } while (0)

#define HEAP_CAPACITY 640000

static_assert(HEAP_CAPACITY % sizeof(uintptr_t) == 0,
	      "The pointer of the platform is of upsupported size.");
extern uintptr_t heap[HEAP_CAPACITY];

void *alloc(size_t size_bytes);
void free(void *ptr);
void collect_heap();


#define TOTAL_ALLOCATION 1024 // Total number of allocation possible

typedef struct {
  uintptr_t *start;
  size_t size;
} Heap_Chunk;

// struct to store all the chunks of Heap
typedef struct {
  size_t count;
  Heap_Chunk chunks[HEAP_CAPACITY];
} Chunk_list;

extern Chunk_list alloced_chunk; //to store the heaps
extern Chunk_list freed_chunk;
extern Chunk_list temp_chunk;

void merge_chunk(Chunk_list *dest, Chunk_list *src);
void insert_chunk(Chunk_list *list, void *start_ptr, size_t size);
int find_chunk(Chunk_list *list, void *start_ptr);
void dump_heap(const Chunk_list *list);
void remove_chunk(Chunk_list *list, size_t index);

#endif
 
