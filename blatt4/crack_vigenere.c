#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define PSIZE 26
#define SHANNON_MEAN 4.08
#define TEXTSIZE 5121 //5KB + '\0'
/**
 * Annahme: Alle Felder ausreichend (TEXTSIZE) groß
 **/ 
enum vigenere_type { encryption = 1, decryption = -1 };
void vigenere(char* text, const char* key, enum vigenere_type flag){
    int keyCount=0 ;
    while(*text!= '\0'){
        *text =(((*text -65) + ( (key[keyCount++]-65) * flag) + 26) % 26) + 65;
        text++;
        if(key[keyCount]=='\0')
            keyCount=0;
    }
}
void encrypt(char* text, const char* key){
    vigenere(text,key, encryption);
}
void decrypt(char* text, const char* key){
    vigenere(text,key, decryption);
}
void readLine(char* text){
    int c, i=0;
    while((c = getchar())!=EOF && i<TEXTSIZE-1 ){
        if (c > 64 && c < 91){
            text[i++]=(char) c;
        }
    }
    text[i]='\0';
}
void clear_p(double* p){
    int i;
    for (i=0;i<PSIZE;i++){
        p[i]=0;
    }
}
/**
 * Annahme: p[PSIZE] und mit 0 initialisiert
 * */
void freq_analysis(double p[], const char* t, int strlen_t, int shift, int offset){
    int i, n;
    for (i=0+offset; i<strlen_t; i+=shift){
        p[t[i]-65]++;
    }
    n = (i - offset) / shift;
    for (i=0; i<PSIZE; i++){
        p[i] /= n;
    }
}
double shannon_entropy(double p[]){
    double res=0.0;
    int i;
    for (i=0; i<PSIZE; i++){
        if (p[i]!=0){
            res += p[i] * (log(p[i]) / log(2));
        }
    }
    return -1 * res;
}
int key_length(const char* cipher){
    double shannon;
    double p[PSIZE];
    int i=0, len = strlen(cipher);
    do{
        i++;
        clear_p(p);
        freq_analysis(p,cipher,len,i,0);
        shannon = shannon_entropy(p);
    }while(((shannon - SHANNON_MEAN)>0.1) && (i<TEXTSIZE) && (shannon > SHANNON_MEAN));

    return i;
}

char encrypted_E(double p[]){
    int i,m=0;
    for (i=0; i<PSIZE;i++){
        if (p[i]>p[m])
            m=i;
    }
    return 'A' + m ;
}

void find_key(char cipher[], char key[]){
    int i, k, l;
    double p[PSIZE];
    l=strlen(cipher);
    k = key_length(cipher);
    for (i = 0; i < k; i++){
        clear_p(p);
        freq_analysis(p,cipher,l,k,i);
        key[i] = (((encrypted_E(p) - 'A') - 4 + 26) % 26) + 'A';
    }
    key[i]='\0';
}



int main(void){
    char cipher[TEXTSIZE], key[TEXTSIZE], plain[TEXTSIZE];

    printf("Please enter ciphertext:\n");
    readLine(cipher);
    printf("\n");

    find_key(cipher,key);
    strncpy(plain,cipher,TEXTSIZE);
    decrypt(plain,key);

    printf("Key: \t\t%s\n",key);
    printf("Plaintext: \t%s\n",plain);

    return 0;
}
