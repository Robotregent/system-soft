#include "printped.h"

/** Gibt den Baum in Inorder-Reihenfolge aus...*/
static void printped(node* root, size_t indent) {
	if(root != NULL) {
       		//printf("Pre print:left; root:%p father:%p\n",root,root->father);
		printped(root->father,indent+1);
        	//printf("Post print:left\n");
    		
		int i;
		for(i = indent; i > 0; i--) {
			printf("   ");
		}
		printf("%s\n",root->name);
        	
		//printf("Pre print:rigth\n");
		printped(root->mother,indent+1);
        	//printf("Post print:right\n");
	}
}

void print(node* root) {
    printped(root,0);
}
