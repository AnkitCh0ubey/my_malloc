#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define HEAP_CAPACITY 640000
#define TOTAL_ALLOCATION 1024 //Total number of allocation possible


// struct to store chunks of heap alloced
typedef struct {
	void* start;
	size_t size;
} Heap_Chunk;


char heap[HEAP_CAPACITY] = {0};
size_t heap_size = 0;

Heap_Chunk alloced_chunk[TOTAL_ALLOCATION] = {0}; //to store the heaps 
size_t n_of_alloced = 0; //to keep track of the number of allocs


void* alloc(size_t size){
	if(size>0){
		assert(heap_size + size <= HEAP_CAPACITY);
		void* result = heap + heap_size;
		heap_size += size;	
	
		const Heap_Chunk chunk =  {
			.start = result,
			.size = size,
		};
	
		assert(n_of_alloced < TOTAL_ALLOCATION);
		alloced_chunk[n_of_alloced++] = chunk; 

		return result;
	}
	else {
		return NULL;
	}
}


void dump(void) {
	printf("Allocated Chunks (%zu):\n", n_of_alloced);
	for(size_t i = 0; i < n_of_alloced; ++i){
		printf("start: %p, size: %zu\n", 
			alloced_chunk[i].start, 
			alloced_chunk[i].size);
	}
}


void free(void *ptr){
	assert(false && "todo: free");
}


void collect_heap() {
	assert(false && "todo: garbage collector");
}


int main() {
	for(size_t i = 0; i<100; ++i){
		alloc(i);
	}
	dump();
	
	return 0;
}
