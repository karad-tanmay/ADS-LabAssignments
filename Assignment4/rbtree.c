/*
Implement red-black tree. Perform the operations such as
a. Insert
b. Search
c. Sort based on timestamp
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int val;
	struct node* left, *right, *parent;
	int color; // 1 -> Red, 0 -> Black
} node;

node* createNode(int value);

// a. Insert
node* insertNode(node *root, int value);
void fixup(node *root);

// b. Search
node* searchNode(node *root, int value);

// c. Sort based on timestamp (simply inorder traversal ig)
void inorder(node *root);

// funcs for freeing mem, traversals for verification
void freeBST(node *root);
void preorder(node *root);
void postorder(node *root);

// rotation functions -
node* leftRotate(node *root);
node* rightRotate(node *root);


int main(){
	node *root = NULL;
	root = insertNode(root, 7);
	root = insertNode(root, 3);
	root = insertNode(root, 18);
	root = insertNode(root, 10);
	root = insertNode(root, 22);
	root = insertNode(root, 8);
	root = insertNode(root, 11);
	root = insertNode(root, 26);
	root = insertNode(root, 2);
	root = insertNode(root, 6);

	printf("Preorder Traversal: ");
	preorder(root);
	printf("\nInorder Traversal: ");
	inorder(root);
	printf("\nPostorder Traversal: ");
	postorder(root);
	printf("\n");

	freeBST(root);
	
	return 0;
}


node* createNode(int value){
	node *nn = (node*)malloc(sizeof(node));
	if(!nn)
		return NULL;

	nn->val = value;
	nn->color = 1;
	nn->parent = nn->left = nn->right = NULL;

	return nn;
}

node* insertNode(node *root, int value){
    node *newNode = createNode(value);
    if (!root) {
        return newNode;
    }

    node *current = root;
    node *prev = NULL;

    while (current != NULL) {
        prev = current;
        if (value < current->val) {
            current = current->left;
        } else if (value > current->val) {
            current = current->right;
        } else {
            free(newNode);
            return root;
        }
    }

    if (value < prev->val) {
        prev->left = newNode;
    } else {
        prev->right = newNode;
    }

	newNode->parent = prev;

    fixup(root);

    return root;
}

void fixup(node *root){
    node *newNode = root;
    
    while (newNode != root && newNode->parent && newNode->parent->color == 1) {
        // Case 1: Parent is red
        if (newNode->parent == newNode->parent->parent->left) {
            node *uncle = newNode->parent->parent->right;
            
            // Case 1a: Uncle is red
            if (uncle && uncle->color == 1) {
                newNode->parent->color = 0;  // Parent becomes black
                uncle->color = 0;            // Uncle becomes black
                newNode->parent->parent->color = 1; // Grandparent becomes red
                newNode = newNode->parent->parent;
            } else {
                // Case 1b: Uncle is black and newNode is a right child
                if (newNode == newNode->parent->right) {
                    newNode = newNode->parent;
                    leftRotate(newNode);
                }
                
                // Case 1c: Uncle is black and newNode is a left child
                newNode->parent->color = 0;
                newNode->parent->parent->color = 1;
                rightRotate(newNode->parent->parent);
            }
        } else {
            // Symmetric to the above case (parent is right child of grandparent)
            node *uncle = newNode->parent->parent->left;
            if (uncle && uncle->color == 1) {
                newNode->parent->color = 0;
                uncle->color = 0;
                newNode->parent->parent->color = 1;
                newNode = newNode->parent->parent;
            } else {
                if (newNode == newNode->parent->left) {
                    newNode = newNode->parent;
                    rightRotate(newNode);
                }

                newNode->parent->color = 0;
                newNode->parent->parent->color = 1;
                leftRotate(newNode->parent->parent);
            }
        }
    }

    // Root should always be black
    root->color = 0;
}

node* searchNode(node *root, int value){
	if(!root)
		return NULL;
	
	if(value < root->val)
		return searchNode(root->left, value);
	else if(value > root->val)
		return searchNode(root->right, value);
	else
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

void preorder(node *root){
	if(!root)
		return;
	
	printf("%d ", root->val);
	preorder(root->left);
	preorder(root->right);

	return;
}

void inorder(node *root){
	if(!root)
		return;
	
	inorder(root->left);
	printf("%d ", root->val);
	inorder(root->right);

	return;
}

void postorder(node *root){
	if(!root)
		return;
	
	postorder(root->left);
	postorder(root->right);
	printf("%d ", root->val);

	return;
}

node* leftRotate(node *root){
	if(!root)
		return NULL;

	node *temp = root->right;

	temp->parent = root->parent;
	root->parent = temp;
	if(temp->left)
		temp->left->parent = root;

	root->right = temp->left;
	temp->left = root;

	return temp;
}

node* rightRotate(node *root){
	if(!root)
		return NULL;
	
	node *temp = root->left;

	temp->parent = root->parent;
	root->parent = temp;
	if(temp->right)
		temp->right->parent = root;
	
	root->left = temp->right;
	temp->right = root;

	return temp;
}
