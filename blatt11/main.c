#include <stdio.h>
#include <stdlib.h>
#include "copy_file.h"

int main(void){
    char *src = "test_file\0";
    char *dst = "dst_file\0";
    
    copy_file(src, dst);
    
    return 0;

}
