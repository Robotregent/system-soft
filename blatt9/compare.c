#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h> 
// Filterliste
typedef struct filters{
    void (*filter)(char *);
    struct filters *next;
} filters;

// Hilfsfunktion
void my_assert(bool p, char *s){
    if (!p){
        printf("Fehler:\n%s",s);
        exit(1);
    }
}
bool has_white(char *str){
    int i=0;
    while(str[i]){
        if (isspace(str[i++]))
            return true;
    }
    return false;
}
void trimWhite(char *str, bool makeSingleSpace){
    int str_len = strlen(str);
    int buffer_len =0;
    int token_len =0;
    char *buffer = (char *)calloc(str_len,sizeof(char  *));
    my_assert(buffer, "Out of Memory\n\0");
    char *token = strtok(str," \t\n\v\f\r");   // Begrenzungen entsprechend der isspace Funktion

    token_len = strlen(token);
    strncpy(buffer,token,token_len);
    if (makeSingleSpace){
        strncat(buffer," ",1);             
        buffer_len += 1;
    }
    buffer_len += token_len;
    token = strtok(0," \t\n\v\f\r");

    while(token!=0){                        // string in tokens splitten
        token_len = strlen(token);
        strncat(buffer,token,token_len);    // tokens verketten
        buffer_len += token_len;
        if (makeSingleSpace){               // für Option -s wieder ein Leerzeichen einfügen
            strncat(buffer," ",1);             
            buffer_len += 1;
        }
        token = strtok(0," \t\n\v\f\r");
    }
    my_assert(buffer_len < str_len,"Possible overflow detected\n\0");
    
    strncpy(str,buffer,buffer_len);
    for (int i = buffer_len; i<str_len; i++)
        str[i]='\0';

    free(buffer);
}

// Filterfunktionen
void allUpper(char *str){
    int i = 0;
    while (str[i]){
        str[i] = toupper(str[i]);
        i++;
    }
}
void noWhite(char *str){
    if (has_white(str))
        trimWhite(str,false);
}
void singleSpace(char *str){
    if (has_white(str))
        trimWhite(str,true);
}
void onlyAlpha(char *str){
    int i = 0;
    while (str[i]){
        if(!isalnum(str[i]))
            str[i] = ' ';
        i++;
    }
}

int main (int argc,char* argv[]){
    char stringeling [] = "St. 8)3 ; ringMit\tLucke am Ende   ";
    printf("%s",stringeling);
    printf("\n");
    onlyAlpha(stringeling);
    allUpper(stringeling);
    singleSpace(stringeling);
    printf("%s",stringeling);
    printf("\n");
    return 0;
}
