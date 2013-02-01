#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main (void){
    int a=-1, b=-1;
    char map[36]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H',
                'I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    do{
        printf("a: ");
        if (scanf("%d",&a)!= 1){
            exit(2);
        }
    }while(a<0);

    do{
        printf("b: ");
        if (scanf("%d",&b)!= 1){
            exit(2);
        }
    } while((b<2)||(b>36));
    int size =floor(log(a)/log(b))+1;
    printf("Size: %d\n",size);
    char numbers[size];
    
    for (int i=size; i>0; i--){
        numbers[i-1]=map[a%b];
        a /= b;
    }
    for (int i=0; i<size; i++){
        printf("%c",numbers[i]);
    }
    printf("\n");
    return 0;
}
