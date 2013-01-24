#include "readped.h"

static int getline2(FILE* in, char** line) {
	if(in == NULL) { 
		fprintf(stderr,"Paramter 'in' is null.\n");
		exit(EXIT_FAILURE);
	}
	*line = malloc(sizeof(char));
	if(*line == NULL) {
		fprintf(stderr,"Couldn't allocate memmory\n.");
		exit(EXIT_FAILURE);
	}

	char c;
	int len = 1;
	while((c = fgetc(in)) != EOF && c != '\n') {
		(*line)[len-1] = c;
		*line = realloc(*line,++len);
		if(*line == NULL) {
			fprintf(stderr,"Couldn't allocate memmory\n.");
			exit(EXIT_FAILURE);
		}
	}
	if(len == 1) { 
		return 0;
	}
	(*line)[len-1] = '\0';
	return len-1;
}

void readin(FILE* stream, node* root) {
	char* line;
	if(getline2(stream,&line)) {
		root->name = line;
	}
	size_t size = 1024;
	node** queue = malloc(sizeof(node*)*size);
	if(queue == NULL) {
		fprintf(stderr,"Couldn't allocate memory\n.");
		exit(EXIT_FAILURE);
	}
	// die queue braucht mann, 
	// um die daten in level-order reihenfolge 
	// in den baum einzufuegen
	// mir is nix anderes eingefallen
	size_t first = 0;
	size_t last = 0;
	queue[last++] = root->father;
	queue[last] = root->mother;
	size_t count = 2;
	while(getline2(stream,&line)) {
		if(count == 0) {
			// vater und mutter wurden eingelesen
			// und jetzt naechsten knoten aus der queue als root
			root = queue[++first];
			count = 2;
		}
		if(count == 2) {
			add_father(root,line);
			queue[last++] = root->father;
		} else {
			add_mother(root,line);
			queue[last++] = root->mother;
		}
		count--;
	}
}
