#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "dirlib.h"


char *create_path(const char * path, const char * filename){
    int p_len = strlen(path);
    int f_len = strlen(filename);
    int offset=1;
    if (p_len > 0){
        if (path[p_len-1] !='/')
            offset = 2;
    }
    else{
        offset = 0;
    }
    char *ret = (char *) calloc((p_len + f_len) + offset, sizeof(char));
    assert(ret);
    strncpy(ret,path,p_len);
    if (offset > 1)
        strncat(ret,"/",1);
    strncat(ret,filename,f_len);
    strncat(ret,"\0",1);
    return ret;
}
