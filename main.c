#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_CAPACITY 640000
#define TOTAL_ALLOCATION 1024 // Total number of allocation possible

#define TODO()                                                                 \
  do {                                                                         \
    fprintf(stderr, "%s:%d: TODO: %s not yet implemented\n", __FILE__,         \
            __LINE__, __func__);                                               \
    abort();                                                                   \
  } while (0)

// struct to store the detail of a chunk
typedef struct {
  char *start;
  size_t size;
} Heap_Chunk;

// struct to store all the chunks of Heap
typedef struct {
  size_t count;
  Heap_Chunk chunks[HEAP_CAPACITY];
} Chunk_list;

char heap[HEAP_CAPACITY] = {0};

Chunk_list alloced_chunk = {0}; //to store the heaps
Chunk_list freed_chunk = {
    .count = 1,
    .chunks = {[0] = {.start = heap, .size = sizeof(heap)}},
};
Chunk_list temp_chunk = {0};

// linear search
int find_chunk(Chunk_list *list, void *start_ptr) {
  for (size_t i = 0; i < list->count; ++i) {
    if (list->chunks[i].start == start_ptr) {
      return (int)i;
    }
  }
  return -1;
}

// to add the chunks into a chunklist
void insert_chunk(Chunk_list *list, void *start_ptr, size_t size) {
  assert(list->count < TOTAL_ALLOCATION);
  list->chunks[list->count].start = start_ptr;
  list->chunks[list->count].size = size;

  for (size_t i = list->count;
       i > 0 && list->chunks[i].start < list->chunks[i - 1].start; --i) {
    const Heap_Chunk temp = list->chunks[i];
    list->chunks[i] = list->chunks[i - 1];
    list->chunks[i - 1] = temp;
  }
  list->count += 1;
}

void merge_chunk(Chunk_list *dest, Chunk_list *src) {
  dest->count = 0;
  for (size_t i = 0; i < src->count; i++) {
    const Heap_Chunk chunk = src->chunks[i];

    if (dest->count > 0) {
      Heap_Chunk *top = &dest->chunks[dest->count - 1];

      if (top->start + top->size == chunk.start) {
        top->size += chunk.size;
      } else {
        insert_chunk(dest, chunk.start, chunk.size);
      }
    } else {
      insert_chunk(dest, chunk.start, chunk.size);
    }
  }
}

// to remove a chunk at an index
void remove_chunk(Chunk_list *list, size_t index) {
  assert(index < list->count);
  for (size_t i = index; i < list->count - 1; ++i) {
    list->chunks[i] = list->chunks[i + 1];
  }
  list->count -= 1;
}

// intention: to dump the heap, but it currently just prints it.
void dump_heap(const Chunk_list *list) {
  printf("Allocated Chunks (%zu):\n", list->count);
  for (size_t i = 0; i < list->count; ++i) {
    printf("start: %p, size: %zu\n", list->chunks[i].start,
           list->chunks[i].size);
  }
}

void *alloc(size_t size) {

  if (size > 0) {
    merge_chunk(&temp_chunk, &freed_chunk);
    freed_chunk = temp_chunk;

    for (size_t i = 0; i < freed_chunk.count; ++i) {
      const Heap_Chunk chunk = freed_chunk.chunks[i];

      // since they are sorted in ascending order,
      // this condition will ensure the best freed memory is allocated next
      if (chunk.size >= size) {
        remove_chunk(&freed_chunk, i);
        const size_t tail_size = chunk.size - size;
        insert_chunk(&alloced_chunk, chunk.start, size);

        if (tail_size > 0) {
          insert_chunk(&freed_chunk, chunk.start + size, tail_size);
        }

        return chunk.start;
      }
    }
  }
  return NULL;
}

void free(void *ptr) {
  if (ptr != NULL) {
    const int index = find_chunk(&alloced_chunk, ptr);
	// printf("%p\n", ptr);
      	// fflush(stdout);
	// abort();
    assert(index >= 0);
    Heap_Chunk chunk = alloced_chunk.chunks[index];
    insert_chunk(&freed_chunk, chunk.start, chunk.size);
    remove_chunk(&alloced_chunk, (size_t)index);
  }
}

void collect_heap() {
  alloced_chunk.count = 0;
  printf("alloced pointer: %p\n", &alloced_chunk);
  printf("temp pointer: %p\n", &temp_chunk);
  printf("freed pointer: %p\n", &freed_chunk);
}
#define N 10
void *ptrs[N] = {0};

int main() {
  for (size_t i = 1; i < N; ++i) {
    ptrs[i] = alloc(i);
  }
  for (size_t i = 0; i < N; ++i) {
    if (i % 2 == 0) {
      free(ptrs[i]);
    }
  }
  alloc(10);
  dump_heap(&alloced_chunk);
  dump_heap(&freed_chunk);

  return 0;
}
