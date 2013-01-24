#include "pedigree.h"
#include "readped.h"
#include "printped.h"

int main(int argc, char **argv) {
	
	node* root = create_root();
	readin(stdin,root);
	print(root);

	return 0;
}
