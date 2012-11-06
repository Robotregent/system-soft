#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include  <stdbool.h>
/* Helfer */
bool even(int game[]){
    int i, j;
    int value[]={0,0,0,0};
    bool res = true;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 4; j++) {
            if(game[i] & (1 << j)) {
                value[j] += 1;
            }        
        }
    }
    for (i=0;i<4;i++){
        if (value[i] % 2 != 0){
            res = false;
            break;
        }
    }
    return res;
}
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
void botuneven(int game[]){
    int i, j, old;
    bool iseven;
    for (i=0;i<3;i++){
        if (game[i]>0){
            old=game[i];
            iseven = false;
            for (j=0;j<3;j++){
                game[i]--;
                iseven =  even(game);
                if(iseven){
                    return; // Ausstieg, wenn Gewinnbedingung erreicht
                }
            }
            game[i]=old;
        }
    }
    //Wenn Gewinnbedingugn nicht erreicht werden kann, nur eins abziehen
    for (i=0;i<3;i++){
        if (game[i]>0){
            game[i]--;
            break;
        }
    }
}
void boteven(int game[]){
    int i;
    for (i=0;i<3;i++){
        if (game[i]>0){
            game[i]--;
            break;
        }
    }
}
void bot(int game[]){
    if ( even(game)){
        boteven(game);
    }
    else{
        botuneven(game);
    }
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
