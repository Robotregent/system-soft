#include <stdio.h>
#include <stdlib.h>
#include "up_dir.h"
#define REC_BUCKET 50
int main(int argc, char *argv[]){
    bool v = false;
    char *name = argv[0];

    if (--argc > 0 && **++argv == '-'){
        if (argv[0][1] == 'v' && argv[0][2] == '\0'){
            v = true;
            argc--; argv++;
        }
        else{
            fprintf(stderr,"Usage: %s [-v] <SRCDIR> <DSTDIR>\nOption -v for processinformations\n",name);
            exit(1);
        }
    }
    if (argc != 2){
        fprintf(stderr,"Usage: %s [-v] <SRCDIR> <DSTDIR>\nOption -v for processinformations\n",name);
        exit(1);

    }
    
    update_dir(argv[0],argv[1], REC_BUCKET, v);    
    return 0;

}
