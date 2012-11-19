#include "vigenere.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define FREQSIZE 26
#define SHANNON_MEAN 4.08
#ifndef PSIZE
#define PSIZE 5120
#endif
void clear_p(double* p){
    int i;
    for (i=0;i<FREQSIZE;i++){
        p[i]=0;
    }
}
void print_p(double p[]){
    int i;
    for (i=0;i<FREQSIZE;i++){
        printf("%e ",p[i]);
    }
    printf("\n");
}
/**
 * Annahme: p[FREQSIZE] und mit 0 initialisiert
 * */
void freq_analysis(double p[], const char* t, int strlen_t, int shift, int offset){
    int i;
    for (i=0+offset; i<strlen_t; i+=shift){
        p[t[i]-65]++;
    }
    for (i=0; i<FREQSIZE; i++){
        p[i] /= strlen_t;
    }
}
double shannon_entropy(double p[]){
    double res=0.0;
    int i;
    for (i=0; i<FREQSIZE; i++){
        if (p[i]!=0){
            res += p[i] * (log(p[i]) / log(2));
        }
    }
    return -1 * res;
}
int key_length(const char* cipher){
    double shannon;
    double p[FREQSIZE];
    int i=0, len = strlen(cipher);
    do{
        i++;
        clear_p(p);
        freq_analysis(p,cipher,len,i,0);
        shannon = shannon_entropy(p);
    }while(((shannon - SHANNON_MEAN)>0.1) && (i<PSIZE) && (shannon > SHANNON_MEAN));

    printf("Entropie: %e, Delta: %e ",shannon, (shannon - SHANNON_MEAN));

    return i;
}
int main(void){
    char cipher[PSIZE];
    int i;

    printf("Please enter ciphertext:\n");
    readLine(cipher);
    printf("\n");

    i= key_length(cipher);
    printf("Länge: %d\n",i);

    return 0;
}
