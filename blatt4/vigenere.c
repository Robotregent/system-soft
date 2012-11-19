#include <stdio.h>
#include <stdlib.h>
#define PSIZE 5120
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
    while((c = getchar())!=EOF && i<PSIZE-1 ){
        if (c > 64 && c < 91){
            text[i]=(char) c;
            i++;
        }
    }
    text[i]='\0';
}
/*
int main(void){
    char key[PSIZE], plain[PSIZE];
    printf("Please enter key:\n");
    readLine(key);
 
    printf("Please enter plaintext:\n ");
    readLine(plain);
    printf("\n");

    encrypt(plain, key);
    printf("Ciphertext:\t");                
    printf("%s\n",plain);
    decrypt(plain, key);
    printf("Plaintext:\t");                
    printf("%s\n",plain);
    
    return 0;
}
*/
