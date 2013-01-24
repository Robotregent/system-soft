#include "pedigree.h"

node* create_root() {
	node* root = malloc(sizeof(node));
	if(root == NULL) {
		fprintf(stderr,"Couldn't allocate memory\n.");
		exit(EXIT_FAILURE);
	}
	root->father = 0;
	root->mother = 0;
	return root;
}

void add_father(node* curr, char* name) {
	curr->father = malloc(sizeof(node));
	if(curr->father == NULL) {
		fprintf(stderr,"Couldn't allocate memory\n.");
		exit(EXIT_FAILURE);
	}
	curr->father->name = name;
	curr->father->father = 0;
	curr->father->mother = 0;
}

void add_mother(node* curr, char* name) {
	curr->mother = malloc(sizeof(node));
	if(curr->mother == NULL) {
		fprintf(stderr,"Couldn't allocate memory\n.");
		exit(EXIT_FAILURE);
	}
	curr->mother->name = name;
	curr->mother->father = 0;
	curr->mother->mother = 0;
}
