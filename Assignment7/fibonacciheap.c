/* Fibonacci Heap ADT in C
   Supports:
    a. Insert keys
    b. Find Minimum key
    c. Extract Minimum Key
    d. Unite two Fibonacci Heaps
    e. Decrease key
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

typedef struct node {
    int val;
    int degree;
    int mark;
    struct node *parent, *child, *left, *right;
} node;

typedef struct FibHeap {
    node *min;
    int n;
} FibHeap;

node* createNode(int value);

FibHeap* createHeap();

void insertNodeToRootList(FibHeap* h, node* x);

void link(node* y, node* x);

void mergeRootLists(FibHeap* a, FibHeap* b);

void insert(FibHeap* h, int value);

node* findMin(FibHeap* h) ;

void consolidate(FibHeap* h);

node* extractMin(FibHeap* h);

void cut(FibHeap* h, node* x, node* y);

void cascadingCut(FibHeap* h, node* y);

void decreaseKey(FibHeap* h, node* x, int k);

void unite(FibHeap* a, FibHeap* b);

void displayTree(node* Node, int depth);

void display(FibHeap* h);

node* findNodeByValue(node* start, int value);

node* findInHeap(FibHeap* h, int value);


int main() {
    FibHeap* h1 = createHeap();
    insert(h1, 10);
    insert(h1, 3);
    insert(h1, 7);
    insert(h1, 1);
    insert(h1, 8);
    printf("Heap H1 after inserts:\n");
    display(h1);

    node* m = findMin(h1);
    if (m) printf("Min H1: %d\n", m->val);

    node* ex = extractMin(h1);
    if (ex) {
        printf("Extracted Min: %d\n", ex->val);
        free(ex);
    }
    printf("Heap H1 after extractMin:\n");
    display(h1);

    FibHeap* h2 = createHeap();
    insert(h2, 2);
    insert(h2, 5);
    printf("Heap H2:\n");
    display(h2);

    unite(h1, h2);
    printf("After unite H1 and H2:\n");
    display(h1);

    insert(h1, 20);
    node* node20 = findInHeap(h1, 20);
    if (node20) {
        decreaseKey(h1, node20, 4);
        printf("After decreasing key 20 -> 4:\n");
        display(h1);
    }

    return 0;
}


node* createNode(int value) {
    node* x = (node*)malloc(sizeof(node));
    x->val = value;
    x->degree = 0;
    x->mark = 0;
    x->parent = NULL;
    x->child = NULL;
    x->left = x;
    x->right = x;
    return x;
}

FibHeap* createHeap() {
    FibHeap* h = (FibHeap*)malloc(sizeof(FibHeap));
    h->min = NULL;
    h->n = 0;
    return h;
}

void insertNodeToRootList(FibHeap* h, node* x) {
    if (!h->min) {
        h->min = x;
        x->left = x->right = x;
    } else {
        x->left = h->min;
        x->right = h->min->right;
        h->min->right->left = x;
        h->min->right = x;
        if (x->val < h->min->val) h->min = x;
    }
}

void link(node* y, node* x) {
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;
    if (!x->child) {
        x->child = y;
        y->left = y->right = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = 0;
}

void mergeRootLists(FibHeap* a, FibHeap* b) {
    if (!b->min) return;
    if (!a->min) {
        a->min = b->min;
        a->n = b->n;
        return;
    }
    node* a_right = a->min->right;
    node* b_left = b->min->left;
    a->min->right = b->min;
    b->min->left = a->min;
    a_right->left = b_left;
    b_left->right = a_right;
    if (b->min->val < a->min->val) a->min = b->min;
    a->n += b->n;
}

void insert(FibHeap* h, int value) {
    node* x = createNode(value);
    insertNodeToRootList(h, x);
    h->n++;
}

node* findMin(FibHeap* h) {
    return h->min;
}

void consolidate(FibHeap* h) {
    if (!h->min) return;
    int D = (int)(floor(log2((h->n > 0) ? h->n : 1))) + 2;
    node** A = (node**)malloc(sizeof(node*) * (D + 5));
    for (int i = 0; i < D + 5; i++) A[i] = NULL;
    node* start = h->min;
    node* w = start;
    if (!w) { free(A); return; }
    node* rootList[D + 5];
    int count = 0;
    do {
        rootList[count++] = w;
        w = w->right;
    } while (w != start);
    for (int i = 0; i < count; i++) {
        node* x = rootList[i];
        int d = x->degree;
        while (A[d]) {
            node* y = A[d];
            if (x->val > y->val) { node* tmp = x; x = y; y = tmp; }
            link(y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    }
    h->min = NULL;
    for (int i = 0; i < D + 5; i++) {
        if (A[i]) {
            A[i]->left = A[i]->right = A[i];
            if (!h->min) h->min = A[i];
            else insertNodeToRootList(h, A[i]);
        }
    }
    free(A);
}

node* extractMin(FibHeap* h) {
    node* z = h->min;
    if (!z) return NULL;
    if (z->child) {
        node* c_start = z->child;
        node* cur = c_start;
        do {
            node* next = cur->right;
            cur->parent = NULL;
            cur->left = cur->right = cur;
            insertNodeToRootList(h, cur);
            cur = next;
        } while (cur != c_start);
        z->child = NULL;
    }
    z->left->right = z->right;
    z->right->left = z->left;
    if (z == z->right) {
        h->min = NULL;
    } else {
        h->min = z->right;
        consolidate(h);
    }
    h->n--;
    z->left = z->right = z;
    return z;
}

void cut(FibHeap* h, node* x, node* y) {
    if (x->right == x) y->child = NULL;
    else {
        x->left->right = x->right;
        x->right->left = x->left;
        if (y->child == x) y->child = x->right;
    }
    y->degree--;
    x->parent = NULL;
    x->left = x->right = x;
    x->mark = 0;
    insertNodeToRootList(h, x);
}

void cascadingCut(FibHeap* h, node* y) {
    node* z = y->parent;
    if (z) {
        if (!y->mark) y->mark = 1;
        else {
            cut(h, y, z);
            cascadingCut(h, z);
        }
    }
}

void decreaseKey(FibHeap* h, node* x, int k) {
    if (!x || k > x->val) return;
    x->val = k;
    node* y = x->parent;
    if (y && x->val < y->val) {
        cut(h, x, y);
        cascadingCut(h, y);
    }
    if (x->val < (h->min ? h->min->val : INT_MAX)) h->min = x;
}

void unite(FibHeap* a, FibHeap* b) {
    if (!a || !b) return;
    mergeRootLists(a, b);
    b->min = NULL;
    b->n = 0;
}

void displayTree(node* Node, int depth) {
    if (!Node) return;
    node* start = Node;
    node* cur = start;
    do {
        for (int i = 0; i < depth; i++) printf("  ");
        printf("%d\n", cur->val);
        if (cur->child) displayTree(cur->child, depth + 1);
        cur = cur->right;
    } while (cur != start);
}

void display(FibHeap* h) {
    if (!h->min) { printf("Heap is empty\n"); return; }
    printf("Root list:\n");
    node* start = h->min;
    node* cur = start;
    do {
        printf("Tree (degree %d):\n", cur->degree);
        displayTree(cur, 1);
        cur = cur->right;
    } while (cur != start);
}

node* findNodeByValue(node* start, int value) {
    if (!start) return NULL;
    node* cur = start;
    do {
        if (cur->val == value) return cur;
        node* res = findNodeByValue(cur->child, value);
        if (res) return res;
        cur = cur->right;
    } while (cur != start);
    return NULL;
}

node* findInHeap(FibHeap* h, int value) {
    return findNodeByValue(h->min, value);
}
