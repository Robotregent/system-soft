#ifndef PEDIGREE_H
#define PEDIGREE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node {
	char* name;
	struct node* mother;
	struct node* father;
} node;

node* create_root();
void add_father(node* curr, char* name);
void add_mother(node* curr, char* name);

#endif /*PEDIGREE_H*/
