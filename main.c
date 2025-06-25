#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define HEAP_CAPACITY 640000
#define TOTAL_ALLOCATION 1024 //Total number of allocation possible
#define	MAX_FREE 1024
#define TODO() \
	do { \
		fprintf(stderr, "%s:%d: TODO: %s not yet implemented\n", \
				__FILE__, __LINE__, __func__); \
		abort(); \
	} while(0)

// struct to store chunks of heap alloced
typedef struct {
	void* start;
	size_t size;
} Heap_Chunk;

typedef struct{
	size_t count;
	Heap_Chunk chunks[HEAP_CAPACITY];
} Chunk_list;

int find_chunk(Chunk_list* list, void* ptr)
{
	TODO();
}

// to add the chunks into the chunklist
void insert_chunk(Chunk_list* list, void* ptr, size_t size)
{
	TODO();
}

// to remove at an index
void remove_chunk(Chunk_list* list, size_t index)
{
	TODO();
}

void dump_heap(const Chunk_list* list)
{	printf("Allocated Chunks (%zu):\n", list->count );
	for(size_t i = 0; i < list->count; ++i){
		printf("start: %p, size: %zu\n", 
			list->chunks[i].start, 
			list->chunks[i].size);
	}
}


char heap[HEAP_CAPACITY] = {0};
size_t heap_size = 0;

Chunk_list alloced_chunk = {0}; //to store the heaps 
Chunk_list freed_chunk = {0}; //to treack the freed heaps 

void* alloc(size_t size){
	if(size>0){
		assert(heap_size + size <= HEAP_CAPACITY);
		void* ptr= heap + heap_size;
		heap_size += size;	
		insert_chunk(&alloced_chunk, ptr, heap_size);
		return ptr;
	}
	else {
		return NULL;
	}
}

void free(void *ptr)
{
	TODO();
}


void collect_heap() 
{
	TODO();
}


int main() {
	for(size_t i = 0; i<100; ++i){
		void* p = alloc(i);
		if( i%2 == 0){
			free(p);
		}
	}
	
	
	return 0;
}





