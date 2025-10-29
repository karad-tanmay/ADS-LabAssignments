/*
Implement red-black tree. Perform the operations such as
a. Insert
b. Search
c. Sort based on timestamp
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct node{
	time_t timestamp;
	struct node* left, *right, *parent;
	int color; // 1 -> Red, 0 -> Black
} node;

node* createNode(time_t timestamp);

// a. Insert
node* insertNode(node *root, time_t timestamp);
void fixup(node *root);

// b. Search
node* searchNode(node *root, time_t timestamp);

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

	srand(time(NULL));

	time_t now = time(NULL);
	time(&now);

	for(int i = 0;i < 12; i++){
		root = insertNode(root, now + rand() % 1000000);
	}

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


node* createNode(time_t timestamp){
	node *nn = (node*)malloc(sizeof(node));
	if(!nn)
		return NULL;

	nn->timestamp = timestamp;
	nn->color = 1;
	nn->parent = nn->left = nn->right = NULL;

	return nn;
}

node* insertNode(node *root, time_t timestamp){
    node *newNode = createNode(timestamp);
    if (!root) {
        return newNode;
    }

    node *current = root;
    node *prev = NULL;

    while (current != NULL) {
        prev = current;
        if (timestamp < current->timestamp) {
            current = current->left;
        } else if (timestamp > current->timestamp) {
            current = current->right;
        } else {
            free(newNode);
            return root;
        }
    }

    if (timestamp < prev->timestamp) {
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

node* searchNode(node *root, time_t timestamp){
	if(!root)
		return NULL;
	
	if(timestamp < root->timestamp)
		return searchNode(root->left, timestamp);
	else if(timestamp > root->timestamp)
		return searchNode(root->right, timestamp);
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
	
	printf("%s ", ctime(&root->timestamp));
	preorder(root->left);
	preorder(root->right);

	return;
}

void inorder(node *root){
	if(!root)
		return;
	
	inorder(root->left);
	printf("%s ", ctime(&root->timestamp));
	inorder(root->right);

	return;
}

void postorder(node *root){
	if(!root)
		return;
	
	postorder(root->left);
	postorder(root->right);
	printf("%s ", ctime(&root->timestamp));

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
