#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int init(int game[]){
    int i, ans;
    
    srand(time(0));
    for (i=0; i<3; i++){
        game[i]=rand() % 5 + 5;
    }
    printf("Willst Du beginnen? (j/n):\n");
    i=1;
    do {
        ans = getchar();
        if (ans == 'n')
        {
            i=-1;
            break;
        }
    } while (ans != '\n');
    return i;
}
/* Eingabe einlesen */

/* Bot */

/* Hauptprogramm */
int main (void){
    int game[3], i, j;
    i = init(game);
    printf("i: %d\n",i);

    return 0;
}
