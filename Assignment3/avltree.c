/*
	avl tree - insert, display ops
*/

#include <stdio.h>
#include <stdlib.h>

#define max(a, b) (a > b ? a : b)

enum months{
	JAN = 1, 
	FEB = 2, 
	MAR = 3, 
	APR = 4, 
	MAY = 5, 
	JUN = 6,
	JUL = 7, 
	AUG = 8, 
	SEP = 9, 
	OCT = 10, 
	NOV = 11, 
	DEC = 12
};

typedef struct node{
	int val;
	int height;
	struct node *left, *right;
} node;

// basic ops for avl tree creation, deletion, traversals
node* createNode(int value);
node* insertNode(node *root, int value);
void freeBST(node *root);

void preorder(node *root);
void inorder(node *root);
void postorder(node *root);

// rotation functions -
node* leftRotate(node *root);
node* rightRotate(node *root);

// get height func
int height(node *root){
	if(!root)
		return 0;
	return root->height;
}

// helper func for printing month name
void printMonth(int month);


int main(){
	node *root = NULL;
	root = insertNode(root, JAN);
	root = insertNode(root, FEB);
	root = insertNode(root, APR);
	root = insertNode(root, MAR);
	root = insertNode(root, MAY);
	root = insertNode(root, JUN);
	root = insertNode(root, DEC);

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
	nn->height = 1;
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
	else
		return root;

	root->height = max(height(root->left), height(root->right)) + 1;

	int balance = height(root->left) - height(root->right);
	
	// left-left
	if(balance > 1 && value < root->left->val)
		return rightRotate(root);
	
	// right-right
	if(balance < -1 && value > root->right->val)
		return leftRotate(root);
	
	// left-right 
	if(balance > 1 && value > root->left->val){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	
	// right-left 
	if(balance < -1 && value < root->right->val){
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	
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
	
	printMonth(root->val);
	preorder(root->left);
	preorder(root->right);

	return;
}

void inorder(node *root){
	if(!root)
		return;
	
	inorder(root->left);
	printMonth(root->val);
	inorder(root->right);

	return;
}

void postorder(node *root){
	if(!root)
		return;
	
	postorder(root->left);
	postorder(root->right);
	printMonth(root->val);

	return;
}

node* leftRotate(node *root){
	if(!root)
		return NULL;
	node *temp = root->right;
	root->right = temp->left;
	temp->left = root;

	root->height = max(height(root->left), height(root->right)) + 1;
	temp->height = max(height(temp->left), height(root->right)) + 1;

	return temp;
}

node* rightRotate(node *root){
	if(!root)
		return NULL;
	
	node *temp = root->left;
	root->left = temp->right;
	temp->right = root;
	
	root->height = max(height(root->left), height(root->right)) + 1;
	temp->height = max(height(temp->left), height(root->right)) + 1;

	return temp;
}

void printMonth(int month){
	const char *months[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	if(month < 1 || month > 12)
		printf("Invalid Month\n");
	else
		printf("%s ", months[month]);
}