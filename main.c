#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "./heap.h"

#define N 10
void *ptrs[N] = {0};

int main() {
  for (size_t i = 0; i < N; ++i) {
    ptrs[i] = alloc(i);
  }
  alloc(10);
	for(size_t i = 0; i<N; i++){
		if (i%2 == 0){
     	free(ptrs[i]);
		}
	}
  dump_heap(&alloced_chunk);
  dump_heap(&freed_chunk);
	
  return 0;
}
