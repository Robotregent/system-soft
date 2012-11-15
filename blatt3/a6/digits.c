#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(void){
    unsigned int a=-1;
    do{
        printf("a: ");
        if (scanf("%u",&a)!=1){
            printf("NaN\n");
            exit(2);
        }
    }while(a<0);
    
    int firstOne = 0;
    unsigned int r = a; 
    unsigned int s = sizeof(a) * 8 - 1;

    for (a >>= 1; a; a >>= 1){   
        r <<= 1;
        r |= a & 1;
        s--;
    }
    r <<= s; 
    for (int i =0;i<(sizeof(r) * 8);i++){   
        if (r & 1){
            printf("1");
            if(!firstOne)
                firstOne=1;
        }
        else{
            if(firstOne)
                printf("0");
        }
        r >>= 1;
    }
    printf("\n");
    return 0;
}
