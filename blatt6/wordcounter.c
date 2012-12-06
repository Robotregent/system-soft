#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node {
	char* word;
	unsigned int freq;
	struct node* left;
	struct node* right;
} node; 

void* smalloc(unsigned int size) {
	void* mem = malloc(size);
	if(mem) {
		return mem;
	}
	fprintf(stderr, "out of memory -- aborting!\n");
	abort();
}

void* srealloc(void* ptr, int size) {
	void* new_mem = realloc(ptr,size);
	if(new_mem) {
		return new_mem;
	}
	fprintf(stderr, "out of memory -- aborting!\n");
	abort();
}

/**
 * Counts the number of digits of a unsigned integer. 
 */ 
unsigned int digits(unsigned int value) {
	unsigned int count = 0;
	while(value) {
		value /= 10;
		count++;
	}
	return count;
}

/**
 * Converts a node to a c string.
 */ 
char* to_string(node* n) {
	if(n == NULL) {
		return NULL;
	}
	char* str = smalloc(digits(n->freq) + strlen(n->word) + 1);
	sprintf(str,"%s: %d",n->word,n->freq);
	return str;
}

/**
 * Converts a c string to a node.
 */ 
node* new_node(char* str) {
	node* n = (node*) smalloc(sizeof(node));
	n->word = (char*) smalloc((strlen(str)+1));
	strcpy(n->word,str);
	n->freq = 1;
	n->left = NULL;
	n->right = NULL;
	return n;
}

/**
 * Prints the tree from root with an inorder traversal.
 */
void print(node* root) {
	if(root != NULL) {
		print(root->left);
		char* str = to_string(root);
		printf("%s\n",str);
		free(str);
		print(root->right);
	}
}

/**
 * Inserts a new word into the tree from root.
 */
void insert_node(node** root, char* new_word) {
	if(*root == NULL) {
		*root = new_node(new_word);
	} else {
		if(strcmp((*root)->word,new_word) == 0) {
			(*root)->freq++;
		} else if(strcmp((*root)->word,new_word) > 0) {
			return insert_node(&((*root)->left),new_word);
		} else {
			return insert_node(&((*root)->right),new_word);
		}
	}
}

/**
 * Deletes the tree from node n and gives the memory back 
 * to free heap space.
 */
void delete_tree(node** n) {
	if(*n != NULL) {
		delete_tree(&((*n)->left));
		delete_tree(&((*n)->right));
		free((*n)->word);
		free(*n);
		*n = NULL;
	}
}

int read_word(char** word) {
	*word = (char*) smalloc(10);
	unsigned int pos = 0;
	unsigned int len = 10; 
	char ch = getchar();
	while(ch != EOF && isalpha(ch)) {
		(*word)[pos++] = ch;
		if(pos > len-1) {
			len *= 2;
			*word = srealloc(*word,len);
		}
		ch = getchar();
	}
	(*word)[pos] = '\0';
	return ch;
}

int main(int argc, char** argv) {
	node* root = NULL;
	char* str;
	while(read_word(&str) != EOF) {
		if(strlen(str) > 0) {
			insert_node(&root,str);
		}
		free(str);
	}
	print(root);
	delete_tree(&root);
	
	return 0;
}
