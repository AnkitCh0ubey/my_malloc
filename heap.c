#include "./heap.h"
#include <stdio.h>

uintptr_t heap[HEAP_CAPACITY] = {0};

Chunk_list alloced_chunk = {0}; //to store the heaps
Chunk_list freed_chunk = {
    .count = 1,
    .chunks = {[0] = {.start = heap, .size = sizeof(heap)}},
};
Chunk_list temp_chunk = {0};



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
    printf("start: %p, size: %zu\n",(void*) list->chunks[i].start,
           list->chunks[i].size);
  }
}

void *alloc(size_t size_bytes) {
	const size_t relative_pos = (size_bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
  if (relative_pos > 0) {
    merge_chunk(&temp_chunk, &freed_chunk);
    freed_chunk = temp_chunk;

    for (size_t i = 0; i < freed_chunk.count; ++i) {
      const Heap_Chunk chunk = freed_chunk.chunks[i];

      // since they are sorted in ascending order,
      // this condition will ensure the best freed memory is allocated next
      if (chunk.size >= relative_pos) {
        remove_chunk(&freed_chunk, i);
        const size_t padding = chunk.size - relative_pos;
        insert_chunk(&alloced_chunk, chunk.start, relative_pos);

        if (padding > 0) {
          insert_chunk(&freed_chunk, chunk.start + relative_pos, padding);
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
