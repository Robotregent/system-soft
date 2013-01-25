#include <string.h>
#include <assert.h>
#include "dirlib.h"


char *create_path(const char * path, const char * filename){
    int p_len = strlen(path);
    int f_len = strlen(filename);
    char *ret = (char *) malloc(sizeof(char) * (p_len + f_len) + 2);
    assert(ret);
    strncpy(ret,path,p_len);
    strncat(ret,"/",1);
    strncat(ret,filename,f_len);
    strncat(ret,"\0",1);
    return ret;
}
