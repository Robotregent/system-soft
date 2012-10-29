#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* Helfer */
void printGame(int game[]){
    int i, j;
    for(i=0; i<3; i++){
        printf("%d: ",i+1);
        for(j=0; j<game[i]; j++){
            printf("I");
            if ((j+1) % 5 == 0){
                printf(" ");
            }
        }
        printf("\n");
    }
}
int init(int game[]){
    int i, ans;
    srand(time(0));
    for (i=0; i<3; i++){
        game[i]=rand() % 6 + 5;
    }
    printGame(game);
    printf("Willst Du beginnen? (j/n):\n");
    i=1;
    do {
        ans = getchar();
        if (ans == 'n')
        {
            i=0;
            break;
        }
    } while (ans != '\n');
    return i;
}
int gameCount(int game[]){
    int r=0,i;
    for (i=0; i<3; i++){
        r+=game[i];
    }
    return r;
}
/* Human einlesen */
int read(){
    int res;
    if (scanf(" %d",&res)!=1)
        exit(1);
    if ((res>3)||(res<1)){
        res = -1;
    }
    return res;
}
void human(int game[]){
    int h,s;
    do{
        printf("Haufen:");
        h=read();
    }while((h<0)||(game[h-1]<=0));

    do{
        printf("Anzahl:");
        s=read();
        if (s>game[h-1])
            s=-1;
    }while(s<=0);
    game[h-1]=game[h-1] - s;
}
/* Bot einlesen */
void bot(int game[]){
    //i = rand() % 3;
    if (game[0] > 0)
        game[0]--;
    else if (game[1]>0)
        game[1]--;
    else if (game[2]>0)
        game[2]--;
}
int main (void){
    int game[3], turn, counter=1;
    turn = init(game);
    while (gameCount(game)>0){
        printf("Runde: %d\n",counter);
        printGame(game);
        if(turn){
            human(game);
        }
        else{
            bot(game);
        }
        printf("--------\n");
        turn = !turn;
        counter++;
    }
    if (turn){
        printf("Bot hat gewonnen :(\n");
    }
    else{
        printf("Glückwunsch, Du hast gewonnen!\n");
    }
    return 0;
}
