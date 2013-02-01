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

filters *filterlist = 0;

// Hilfsfunktion
void my_assert(bool p, char *s){
    if (!p){
        printf("Fehler:\n%s",s);
        exit(1);
    }
}
void apply_filters(char *orig_str,char **buffer_str, int len){
    *buffer_str = (char *) realloc(*buffer_str,len);
    strncpy(*buffer_str, orig_str, len);
    filters *f = filterlist;
    while (f != 0){
        (*f->filter)(*buffer_str);
        f = f->next;
    }
}
char *readline(FILE *file,int *line_len){
    int len = 32;
    char * cp = malloc(len);
    my_assert(cp,"Kein Speicher mehr!\n\0");
    int i = 0, ch;
    while((ch = getc(file))!=EOF && ch != '\n'){
        cp[i++] = ch;
        if (i == len) {
            len <<= 2;
            char *newcp = realloc(cp,len);
            my_assert(newcp,"Kein Speicher mehr!\n\0");
            cp = newcp;
        }
    }
    if (i == 0 && ch == EOF){
        free(cp);
        return 0;
    }
    cp[i++]=0;
    *line_len = i;
    return realloc(cp,i);
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
    char c = str[str_len-1];
    int buffer_len =0;
    int token_len =0;
    char *buffer = (char *)calloc(str_len,sizeof(char  *));
    my_assert(buffer, "Out of Memory\n\0");
    char *token = strtok(str," \t\n\v\f\r");// Begrenzungen entsprechend der isspace Funktion

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
    if (makeSingleSpace){
        if (!isspace(c))
            buffer_len--;                    // Zusätliches Whitespace am Ende
    }
    my_assert(buffer_len <=  str_len,"Possible overflow detected\n\0");
    
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
// Parameter verarbeiten
void pars_opt(int argc, char **argv, char **file_names){
    int opt;
    filters *a_filter=0;
    bool w = false, s = false;
    while ((opt = getopt (argc, argv, "cwsa")) != -1){
        a_filter = (filters *) malloc(sizeof(filters));
        my_assert(a_filter,"Speicher aus!\n\0");
        a_filter->next=filterlist;
        filterlist=a_filter;
        switch (opt){
            case 'c':
                a_filter->filter = &allUpper;
            printf("Groß-/Kleinschreibeung ignorieren\n");
            break;
        case 'w':
            a_filter->filter = &noWhite;
            w = true;
            printf("Whitespace ignorieren\n");
            break;
        case 's':
            a_filter->filter = &singleSpace;
            s = true;
            printf("Alle Whitespace als ein Leerzeichen interpretieren\n");
            break;
        case 'a':
            a_filter->filter = &onlyAlpha;
            printf("Nur alphanumerische Zeichen\n");
            break;
        default:
            free(a_filter);
            my_assert(false,"Ungültige Parameter\n\0");
        }
    }
    my_assert(!(w&&s),"Parameter -w und -s können nicht paralell auftreten.\n\0");
    int i;
    for(i = 0; i + optind < argc && i < 2; i++){
         file_names[i] = argv[i + optind];
    }
    my_assert(i==2,"Bitte zwei Dateinamen angeben!\n\0");
}

int main (int argc,char* argv[]){
    char *file_names[2], *orig_lines[2], *filter_buffer[2];
    int len[2], eq;
    FILE *file_a, *file_b;
    pars_opt(argc,argv,file_names);
    file_a = fopen(file_names[0],"r"); 
    my_assert(file_a,"Konnte Datei eins nicht öffnen.\n\0");
    file_b = fopen(file_names[1],"r"); 
    my_assert(file_b,"Konnte Datei zwei nicht öffnen.\n\0");

    filter_buffer[0]=0;
    filter_buffer[1]=0;
    orig_lines[0]=readline(file_a,&len[0]);
    orig_lines[1]=readline(file_b,&len[1]);
    while (orig_lines[0] && orig_lines[1]){
        for (int i = 0; i < 2; i++){
            apply_filters(orig_lines[i],&filter_buffer[i],len[i]);
        }
        if (filterlist)
            eq = strcmp(filter_buffer[0],filter_buffer[1]);
        else
            eq = strcmp(orig_lines[0],orig_lines[1]);
        if (eq){
            for (int i = 0; i < 2; i++)
                printf("%s:\t%s\n",file_names[i],orig_lines[i]);
        }
        free(orig_lines[0]);
        free(orig_lines[1]);
        orig_lines[0]=readline(file_a,&len[0]);
        orig_lines[1]=readline(file_b,&len[1]);
    }

    // CleanUp
    if(orig_lines[0]){
        printf("Vorzeitiges Ende von Datei zwei\n");
        free(orig_lines[0]);
    }
    if(orig_lines[1]){
        printf("Vorzeitiges Ende von Datei eins\n");
        free(orig_lines[1]);
    }
    for (int i = 0; i<2;i++){
        if (filter_buffer[i])
            free(filter_buffer[i]);
    }
    filters *cleaner;
    while (filterlist){
        cleaner = filterlist->next;
        free(filterlist);
        filterlist = cleaner;
    }
    return 0;
}
