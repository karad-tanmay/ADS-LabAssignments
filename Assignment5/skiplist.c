/* Implement a basic skip list. Perform:
	a. Insert
	b. Search
	c. Delete
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_LEVEL 6

typedef struct node {
	int val;
	struct node *forward[MAX_LEVEL];
} node;

typedef struct SkipList {
	int level;
	node *header;
} SkipList;

node* createNode(int value, int level);
int randomLevel();
SkipList* createSkipList();
void insert(SkipList* list, int value);
node* search(SkipList* list, int value);
void delete(SkipList* list, int value);
void display(SkipList* list);


int main() {
	srand((unsigned)time(NULL));

	SkipList* list = createSkipList();

	insert(list, 3);
	insert(list, 6);
	insert(list, 7);
	insert(list, 9);
	insert(list, 12);
	insert(list, 19);
	display(list);

	node* f = search(list, 9);
	if (f)
		printf("Found: %d\n", f->val);

	delete(list, 9);
	display(list);

	return 0;
}


node* createNode(int value, int level) {
	node* n = (node*)malloc(sizeof(node));
	n->val = value;
	for (int i = 0; i < MAX_LEVEL; i++)
		n->forward[i] = NULL;
	return n;
}

int randomLevel() {
	int lvl = 1;
	while ((rand() % 2) && lvl < MAX_LEVEL)
		lvl++;
	return lvl;
}

SkipList* createSkipList() {
	SkipList* list = (SkipList*)malloc(sizeof(SkipList));
	list->level = 1;
	list->header = createNode(INT_MIN, MAX_LEVEL);
	return list;
}

void insert(SkipList* list, int value) {
	node* update[MAX_LEVEL];
	node* x = list->header;
	for (int i = list->level - 1; i >= 0; i--) {
		while (x->forward[i] && x->forward[i]->val < value)
			x = x->forward[i];
		update[i] = x;
	}
	x = x->forward[0];
	if (x == NULL || x->val != value) {
		int lvl = randomLevel();
		if (lvl > list->level) {
			for (int i = list->level; i < lvl; i++)
				update[i] = list->header;
			list->level = lvl;
		}
		node* newNode = createNode(value, lvl);
		for (int i = 0; i < lvl; i++) {
			newNode->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = newNode;
		}
	}
	return;
}

node* search(SkipList* list, int value) {
	node* x = list->header;
	for (int i = list->level - 1; i >= 0; i--) {
		while (x->forward[i] && x->forward[i]->val < value)
			x = x->forward[i];
	}
	x = x->forward[0];
	if (x && x->val == value)
		return x;
	return NULL;
}

void delete(SkipList* list, int value) {
	node* update[MAX_LEVEL];
	node* x = list->header;
	for (int i = list->level - 1; i >= 0; i--) {
		while (x->forward[i] && x->forward[i]->val < value)
			x = x->forward[i];
		update[i] = x;
	}
	x = x->forward[0];
	if (x && x->val == value) {
		for (int i = 0; i < list->level; i++) {
			if (update[i]->forward[i] != x)
				break;
			update[i]->forward[i] = x->forward[i];
		}
		free(x);
		while (list->level > 1 && list->header->forward[list->level - 1] == NULL)
			list->level--;
	}
	return;
}

void display(SkipList* list) {
	for (int i = 0; i < list->level; i++) {
		node* Node = list->header->forward[i];
		printf("Level %d: ", i + 1);
		while (Node) {
			printf("%d ", Node->val);
			Node = Node->forward[i];
		}
		printf("\n");
	}
	return;
}
