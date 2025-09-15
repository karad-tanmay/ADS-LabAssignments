/*
	1. Write the following functions for a binary search tree implementation:
		a. Searches the maximum value in the tree and return a pointer to the node
		b. Pre-order, in-order and post-order traversal without using recursion
		c. Print the binary tree so that it looks like a tree.
*/

#include <stdio.h>
#include <stdlib.h>

#define STACKSIZE 32

typedef struct node{
	int val;
	struct node *left, *right;
} node;

// basic ops for tree creation, deletion
node* createNode(int value);
node* insertNode(node *root, int value);
void freeBST(node *root);

// a. ->
node* maxValueNode(node *root);

// b. ->
void iterativePreorder(node *root);
void iterativeInorder(node *root);
void iterativePostorder(node *root);

// c. ->
void displayTree(node *root);



int main(){
	printf("test1\n");
	node *root = NULL;
	root = insertNode(root, 10);
	root = insertNode(root, 5);
	root = insertNode(root, 15);
	root = insertNode(root, 2);
	root = insertNode(root, 7);
	root = insertNode(root, 12);
	root = insertNode(root, 17);

	node *maxNode = maxValueNode(root);
	if(maxNode)
		printf("Max Value Node: %d\n", maxNode->val);

	printf("Iterative Preorder: ");
	iterativePreorder(root);
	printf("\nIterative Inorder: ");
	iterativeInorder(root);
	printf("\nIterative Postorder: ");
	iterativePostorder(root);
	printf("\n");

	freeBST(root);
	
	return 0;
}



// basic ops for tree creation, deletion
node* createNode(int value){
	node *nn = (node*)malloc(sizeof(node));
	if(!nn)
		return NULL;

	nn->val = value;
	nn->left = nn->right = NULL;

	return nn;
}

node* insertNode(node *root, int value){
	if(!root)
		return createNode(value);

	if(value < root->val)
		root->left = insertNode(root->left, value);
	else if(value > root->val)
		root->right = insertNode(root->right, value);

	return root;
}

void freeBST(node *root){
	if(!root)
		return;
	
	freeBST(root->left);
	freeBST(root->right);

	free(root);
	root = NULL;

	return;
}

// a. ->
node* maxValueNode(node *root){
	if(!root || !root->right)
		return root;

	return maxValueNode(root->right);
}

// b. ->
void iterativePreorder(node *root){
	node* st[STACKSIZE];
	int top = -1; // push -> top++, insert; pop -> remove, top--; top = -1 -> empty stack.
	
	node *p = root;
	while(p || top >= 0){
		if(p){
			printf("%d ", p->val);
			st[++top] = p;
			p = p->left;
		}
		else{
			p = st[top--];
			p = p->right;
		}
	}
	return;
}

void iterativeInorder(node *root){
	node* st[STACKSIZE];
	int top = -1; // push -> top++, insert; pop -> remove, top--; top = -1 -> empty stack.

	node *p = root;
	while(p || top >= 0){
		if(p){
			st[++top] = p;
			p = p->left;
		}
		else{
			p = st[top--];
			printf("%d ", p->val);
			p = p->right;
		}
	}
	return;
}

void iterativePostorder(node *root){
	node* st[STACKSIZE];
	int top = -1; // push -> top++, insert; pop -> remove, top--; top = -1 -> empty stack.
	
	node *p = root;
	while(p || top >= 0){
		if(p){
			st[++top] = p->right;
			st[++top] = p;
			p = p->left;
		}
		else{
			p = st[top--];
			if(p->right == st[top]){
				top--;
				st[++top] = p;
				p = p->right;
			}
			else{
				printf("%d ", p->val);
				p = NULL;
			}
		}
	}
	return;
}

// c. ->
void displayTree(node *root);