/* Implement Huffman Coding Tree
	a. Display the codes for every character along with the frequency.
	b. Decode the binary message to text.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_SIZE 16 //max no. of nodes

typedef struct node {
	char c;
	int freq;
	struct node *left, *right;
} node;

typedef struct minHeap {
	int size;
	node** arr;
} minHeap;


node* createNode(char c, int freq);

minHeap* createMinHeap(int capacity);

void swapNode(node** a, node** b);

void heapify(minHeap* heap, int i);

node* extractMin(minHeap* heap);

void insertMinHeap(minHeap* heap, node* node);

node* buildHuffmanTree(minHeap* heap);

void printCodes(node* root, int arr[], int top);

void decode(node* root, const char* code);


int main() {
	minHeap* heap = createMinHeap(HEAP_SIZE);

	insertMinHeap(heap, createNode('A', 5));
	insertMinHeap(heap, createNode('B', 9));
	insertMinHeap(heap, createNode('C', 12));
	insertMinHeap(heap, createNode('D', 13));
	insertMinHeap(heap, createNode('E', 16));
	insertMinHeap(heap, createNode('F', 45));


	node* root = buildHuffmanTree(heap);
	int arr[100];
	printf("Character Codes:\n");
	printCodes(root, arr, 0);

	const char* encoded = "11001101110000";
	decode(root, encoded);

	return 0;
}


node* createNode(char c, int freq) {
	node* n = (node*)malloc(sizeof(node));
	n->c = c;
	n->freq = freq;
	n->left = n->right = NULL;
	return n;
}

minHeap* createMinHeap(int capacity) {
	minHeap* heap = (minHeap*)malloc(sizeof(minHeap));
	heap->size = 0;
	heap->arr = (node**)malloc(capacity * sizeof(node*));
	return heap;
}

void swapNode(node** a, node** b) {
	node* t = *a;
	*a = *b;
	*b = t;
}

void heapify(minHeap* heap, int i) {
	int smallest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	if (l < heap->size && heap->arr[l]->freq < heap->arr[smallest]->freq)
		smallest = l;
	if (r < heap->size && heap->arr[r]->freq < heap->arr[smallest]->freq)
		smallest = r;

	if (smallest != i) {
		swapNode(&heap->arr[i], &heap->arr[smallest]);
		heapify(heap, smallest);
	}
}

node* extractMin(minHeap* heap) {
	node* temp = heap->arr[0];
	heap->arr[0] = heap->arr[heap->size - 1];
	heap->size--;
	heapify(heap, 0);
	return temp;
}

void insertMinHeap(minHeap* heap, node* node) {
	heap->size++;
	int i = heap->size - 1;
	heap->arr[i] = node;
	while (i && heap->arr[(i - 1) / 2]->freq > heap->arr[i]->freq) {
		swapNode(&heap->arr[i], &heap->arr[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

node* buildHuffmanTree(minHeap* heap) {
	while (heap->size > 1) {
		node* left = extractMin(heap);
		node* right = extractMin(heap);
		node* top = createNode('$', left->freq + right->freq);
		top->left = left;
		top->right = right;
		insertMinHeap(heap, top);
	}
	return extractMin(heap);
}

void printCodes(node* root, int arr[], int top) {
	if (root->left) {
		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}
	if (root->right) {
		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}
	if (!root->left && !root->right) {
		printf("%c (%d): ", root->c, root->freq);
		for (int i = 0; i < top; i++)
			printf("%d", arr[i]);
		printf("\n");
	}
}

void decode(node* root, const char* code) {
	node* curr = root;
	printf("Decoded text: ");
	for (int i = 0; code[i] != '\0'; i++) {
		if (code[i] == '0')
			curr = curr->left;
		else
			curr = curr->right;

		if (!curr->left && !curr->right) {
			printf("%c", curr->c);
			curr = root;
		}
	}
	printf("\n");
}
