#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Node {
	int key, degree;
	struct Node *parent, *child, *sibling;
} Node;

typedef struct BinomialHeap {
	Node *head;
} BinomialHeap;

Node* createNode(int key);

Node* mergeRoots(Node* h1, Node* h2);
Node* linkTrees(Node* y, Node* z);
Node* unionHeaps(Node* h1, Node* h2);

BinomialHeap* createHeap();

void insert(BinomialHeap* heap, int key);

Node* findMinNode(Node* head);

Node* reverseList(Node* node);

Node* extractMin(BinomialHeap* heap);

void decreaseKey(Node* node, int newKey);

void display(Node* head);


int main() {
	BinomialHeap* h1 = createHeap();
	insert(h1, 10);
	insert(h1, 20);
	insert(h1, 5);
	insert(h1, 15);
	printf("Initial Heap:\n");
	display(h1->head);
	
	Node* min = findMinNode(h1->head);
	printf("\nMinimum: %d\n", min->key);
	
	Node* extracted = extractMin(h1);
	printf("\nAfter extracting min (%d):\n", extracted->key);
	display(h1->head);

	BinomialHeap* h2 = createHeap();
	insert(h2, 3);
	insert(h2, 8);
	insert(h2, 25);
	printf("\nSecond Heap:\n");
	display(h2->head);
	
	h1->head = unionHeaps(h1->head, h2->head);
	printf("\nAfter merging both heaps:\n");
	display(h1->head);

	printf("\nDecreasing key 20 to 2:\n");
	Node* n = findMinNode(h1->head->sibling); 
	decreaseKey(n, 2);
	display(h1->head);

	return 0;
}


Node* createNode(int key) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->key = key;
	n->degree = 0;
	n->parent = n->child = n->sibling = NULL;
	return n;
}

Node* mergeRoots(Node* h1, Node* h2) {
	if (!h1) return h2;
	if (!h2) return h1;
	Node* head = NULL;
	Node** pos = &head;
	while (h1 && h2) {
		if (h1->degree <= h2->degree) {
			*pos = h1;
			h1 = h1->sibling;
		} else {
			*pos = h2;
			h2 = h2->sibling;
		}
		pos = &((*pos)->sibling);
	}
	*pos = (h1) ? h1 : h2;
	return head;
}

Node* linkTrees(Node* y, Node* z) {
	y->parent = z;
	y->sibling = z->child;
	z->child = y;
	z->degree++;
	return z;
}

Node* unionHeaps(Node* h1, Node* h2) {
	Node* newHead = mergeRoots(h1, h2);
	if (!newHead) return NULL;
	Node *prev = NULL, *curr = newHead, *next = curr->sibling;
	while (next) {
		if ((curr->degree != next->degree) ||
			(next->sibling && next->sibling->degree == curr->degree)) {
			prev = curr;
			curr = next;
		} else {
			if (curr->key <= next->key) {
				curr->sibling = next->sibling;
				curr = linkTrees(next, curr);
			} else {
				if (prev) prev->sibling = next;
				else newHead = next;
				curr = linkTrees(curr, next);
			}
		}
		next = curr->sibling;
	}
	return newHead;
}

BinomialHeap* createHeap() {
	BinomialHeap* heap = (BinomialHeap*)malloc(sizeof(BinomialHeap));
	heap->head = NULL;
	return heap;
}

void insert(BinomialHeap* heap, int key) {
	Node* node = createNode(key);
	BinomialHeap temp = {node};
	heap->head = unionHeaps(heap->head, temp.head);
}

Node* findMinNode(Node* head) {
	if (!head) return NULL;
	Node* y = NULL;
	Node* x = head;
	int min = INT_MAX;
	while (x) {
		if (x->key < min) {
			min = x->key;
			y = x;
		}
		x = x->sibling;
	}
	return y;
}

Node* reverseList(Node* node) {
	Node* prev = NULL;
	Node* next;
	while (node) {
		next = node->sibling;
		node->sibling = prev;
		node->parent = NULL;
		prev = node;
		node = next;
	}
	return prev;
}

Node* extractMin(BinomialHeap* heap) {
	if (!heap->head) return NULL;
	Node* prevMin = NULL, *minNode = heap->head;
	Node* prev = NULL, *curr = heap->head;
	int min = curr->key;
	while (curr) {
		if (curr->key < min) {
			min = curr->key;
			prevMin = prev;
			minNode = curr;
		}
		prev = curr;
		curr = curr->sibling;
	}
	if (prevMin) prevMin->sibling = minNode->sibling;
	else heap->head = minNode->sibling;
	Node* childRev = reverseList(minNode->child);
	heap->head = unionHeaps(heap->head, childRev);
	return minNode;
}

void decreaseKey(Node* node, int newKey) {
	if (!node || newKey > node->key) return;
	node->key = newKey;
	Node* y = node;
	Node* z = y->parent;
	while (z && y->key < z->key) {
		int temp = y->key;
		y->key = z->key;
		z->key = temp;
		y = z;
		z = y->parent;
	}
}

void display(Node* head) {
	for (Node* tree = head; tree; tree = tree->sibling) {
		printf("Tree of degree %d:\n", tree->degree);
		Node* queue[100];
		int front = 0, rear = 0;
		queue[rear++] = tree;
		queue[rear++] = NULL;
		while (front < rear) {
			Node* node = queue[front++];
			if (!node) {
				printf("\n");
				if (front < rear) queue[rear++] = NULL;
				continue;
			}
			printf("%d ", node->key);
			Node* child = node->child;
			while (child) {
				queue[rear++] = child;
				child = child->sibling;
			}
		}
		printf("\n");
	}
}
