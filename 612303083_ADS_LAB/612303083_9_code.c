/*  Create a dictionary of at least 100 English words.
	Insert all words from the dataset into the trie.
	Perform Search and auto-suggest operation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
};


struct TrieNode *createNode();

void insert(struct TrieNode *root, char *word);

int search(struct TrieNode *root, char *word);

void printWords(struct TrieNode *node, char *prefix, int level);
void autoSuggest(struct TrieNode *root, char *prefix);

void insertDictionary(struct TrieNode *root);


int main() {
    struct TrieNode *root = createNode();
    insertDictionary(root);

    char word[50];
    printf("Enter word to search: ");
    scanf("%s", word);

    if (search(root, word))
        printf("'%s' found in dictionary.\n", word);
    else
        printf("'%s' not found in dictionary.\n", word);

    printf("suggestion for prefix 'mo':\n");
    autoSuggest(root, "mo");

    return 0;
}


struct TrieNode *createNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    node->isEndOfWord = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}

void insert(struct TrieNode *root, char *word) {
    struct TrieNode *curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        char ch = tolower(word[i]);
        if (ch < 'a' || ch > 'z') continue;
        int index = ch - 'a';
        if (!curr->children[index])
            curr->children[index] = createNode();
        curr = curr->children[index];
    }
    curr->isEndOfWord = 1;
}

int search(struct TrieNode *root, char *word) {
    struct TrieNode *curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        char ch = tolower(word[i]);
        if (ch < 'a' || ch > 'z') continue;
        int index = ch - 'a';
        if (!curr->children[index])
            return 0;
        curr = curr->children[index];
    }
    return curr->isEndOfWord;
}

void insertDictionary(struct TrieNode *root) {
    char *dictionary[] = {
        "apple","banana","grape","orange","pear","peach","cherry","mango","melon","berry",
        "table","chair","sofa","cupboard","shelf","desk","lamp","door","window","floor",
        "book","pen","pencil","notebook","paper","marker","eraser","sharpener","folder","file",
        "computer","keyboard","mouse","monitor","printer","scanner","speaker","camera","charger","cable",
        "river","mountain","forest","desert","valley","ocean","island","hill","lake","waterfall",
        "dog","cat","lion","tiger","elephant","horse","cow","goat","sheep","monkey",
        "red","blue","green","yellow","black","white","purple","pink","brown","gray",
        "car","bus","train","truck","plane","ship","bicycle","motorcycle","scooter","subway",
        "sun","moon","star","cloud","rain","snow","wind","storm","light","dark",
        "happy","sad","angry","proud","scared","brave","kind","funny","smart","strong"
    };
    int n = sizeof(dictionary) / sizeof(dictionary[0]);
    for (int i = 0; i < n; i++)
        insert(root, dictionary[i]);
}

void printWords(struct TrieNode *node, char *prefix, int level) {
    if (node->isEndOfWord) {
        prefix[level] = '\0';
        printf("%s\n", prefix);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            prefix[level] = i + 'a';
            printWords(node->children[i], prefix, level + 1);
        }
    }
}

void autoSuggest(struct TrieNode *root, char *prefix) {
    struct TrieNode *curr = root;
    int len = strlen(prefix);

    for (int i = 0; i < len; i++) {
        char ch = tolower(prefix[i]);
        if (ch < 'a' || ch > 'z') continue;
        int index = ch - 'a';
        if (!curr->children[index]) {
            printf("No suggestions found for '%s'\n", prefix);
            return;
        }
        curr = curr->children[index];
    }

    char buffer[100];
    strcpy(buffer, prefix);
    printWords(curr, buffer, strlen(prefix));
}
