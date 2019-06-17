#include "stdio.h"
#include "stdlib.h"

typedef struct Heap {
	int** items;	//Vector 
	int size;	//Maximum items
	int count;	//actual no. items
}Heap;

Heap* newHeap(int size);
Heap* enqueue(Heap* heap, int* item);
Heap* reheapUp(Heap* heap, int first, int last);
int dequeue(Heap* heap);
Heap* reheapDown(Heap* heap, int first);

int main() {
	Heap* heap = newHeap(10);
	FILE* pFile = fopen("Data.txt", "r");
	int item;
	fscanf(pFile,"%i", &item);
	while (!feof(pFile)) {
		heap = enqueue(heap, item); printf("Queue %i:\n\t", item);
		for (int i = 0; i < heap->count; i++) {
			printf("%i  ", heap->items[i]);
		}
		printf("\n");
		fscanf(pFile, "%i", &item);
	}

	while(heap->count) {
		printf("[DequeueResult %i] Remaining queue: ", dequeue(heap));
		for (int i = 0; i < heap->count; i++) {
			printf("%i  ", heap->items[i]);
		}
		printf("\n");
	}


	return 0;
}

Heap* newHeap(int size) {
	Heap* heap = (Heap*)malloc(sizeof(Heap));
	heap->size = size;
	heap->count = 0;
	heap->items = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++) {
		heap->items[i] = NULL;
	}
	return heap;
}
Heap* enqueue(Heap* heap, int* item) {
	if (heap) {
		if (heap->count < heap->size) {
			heap->items[heap->count] = item;
			heap = reheapUp(heap, 0, heap->count);
			heap->count++;
		}
	}
	return heap;
}
Heap* reheapUp(Heap* heap, int first, int last) {
	if (first < last) {
		int indexParent = (last - 1) / 2;
		int* parent = heap->items[indexParent];
		if (parent < heap->items[last]) {
			int* tmp = parent;
			heap->items[indexParent] = heap->items[last];
			heap->items[last] = parent;
			heap = reheapUp(heap, 0, indexParent);
		}
	}
	return heap;
}
int dequeue(Heap* heap) {
	int item = heap->items[0];
	heap->count--;
	heap->items[0] = heap->items[heap->count];
	heap->items[heap->count] = NULL;
	heap = reheapDown(heap, 0);
	return item;
}
Heap* reheapDown(Heap* heap, int first) {
	int* parent = heap->items[first];
	int left = 2 * first + 1;	//left child index
	int right = 2 * first + 2;	//right child index
	int indexMax;
	int* max;	//max item;
	if (left <= heap->count-1) {
		if (left == heap->count-1) {
			max = heap->items[left];
			indexMax = left;
		}
		else {
			if (heap->items[left] > heap->items[right]) {
				indexMax = left;
				max = heap->items[left];
			}
			else if (heap->items[left] < heap->items[right]) {
				indexMax = right;
				max = heap->items[right];
			}
		}
		if (max > parent) {
			heap->items[first] = max;
			heap->items[indexMax] = parent;
			heap = reheapDown(heap, indexMax);
		}
	}
	return heap;
}