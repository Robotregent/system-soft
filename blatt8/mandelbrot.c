#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>

float left = -2;
float right= 2;
float bottom= -2;
float top= 2;
float pixel= 1000;
int n = 500;

void print_usage(){
    printf("-l\tMinimum reelle Achse (\"left\")\n");
    printf("-r\tMaximum reelle Achse (\"right\")\n");
    printf("-b\tMinimum imaginäre Achse (\"bottom\")\n");
    printf("-t\tMaximum imaginäre Achse (\"top\")\n");
    printf("-p\tPixel pro Einheit auf imaginärer/reeller Achse\n");
    printf("-i\tMaximale Anzahl der Iterationen\n");
    printf("Leerzeichen zwischen allen Argumenten notwendig\n");
}
double abs_c(double complex c){
    return creal(c)*creal(c)+cimag(c)*cimag(c);
}
void my_assert(bool p, char *s){
    if (!p){
        printf("Fehler:\n%s",s);
        exit(1);
    }
}
int in_mandel(double complex c){
    int i;
    double complex prev=0;
    for (i=2; i <= n; i++){
        prev = prev * prev + c;
        if (abs_c(prev) > 4){
            i = -1;
            break;
        }
    }
    return i;
}
bool string_to_int(char* s,int* val) {
    char* endptr;
    errno = 0; /* reset errno */
    int i = strtol(s, &endptr, 10);
    if (endptr == s) return false; /* conversion failed */
    if (*endptr) return false; /* unexpected characters behind the integer */
    if (errno) return false; /* out of range */
    *val = i;
    return true;
}

bool string_to_double(char* s, double* val) {
    char* endptr;
    errno = 0; /* reset errno */
    double x = strtod(s, &endptr);
    if (endptr == s) return false; /* conversion failed */
    if (*endptr) return false; /* unexpected characters behind the double */
    if (errno) return false; /* out of range */
    *val = x;
    return true;
}
void parse_args(int c,char **v){
    int i; double d;
    while (--c > 0 && **++v == '-'){
        if (v[0][1] == '-' && v[0][2] == '\0'){
            c--; v++;
            break;
        }
        if (v[0][1] == '\0'|| v[0][2]!='\0'){
            print_usage();
            exit(1);
        }
        switch (v++[0][1]){
            case 'l':
                if (string_to_double(v[0],&d))
                    left = d;
                break;
            case 't':
                if (string_to_double(v[0],&d))
                    top = d;
                break;
            case 'b':
                if (string_to_double(v[0],&d))
                    bottom = d;
                break;
            case 'r':
                if (string_to_double(v[0],&d))
                    right = d;
                break;
            case 'p':
                if (string_to_int(v[0],&i))
                    pixel = i;
                break;
            case 'i':
                if (string_to_int(v[0],&i)){
                    n = i;
                    my_assert(n>0,"n muss größer 0 sein\n\0");
                }
                break;
            case 'h':
            default:
                print_usage();
                exit(1);
        }
        c--;
    }
}
int get_green(int it){
    return pow((((double) it)/((double) n)),0.2)*255;
}
double **get_matrix(int row, int col){
    double **res = (double **) malloc(row*sizeof(double **));
    my_assert(res, "Out of memmory\n\0");
    for (int i = 0; i< row; i++){
        res[i] = (double *) malloc(col*sizeof(double *));
        my_assert(res[i], "Out of memmory\n\0");
    }
    return res;
}
double complex get_c_from_pixel(int z, int s){
    return ((s/(double)pixel)+left) + (((-1 * z)/(double) pixel)+top)*I
}
int main(int argc, char **argv){
    parse_args(argc, argv);
    int row = abs(bottom) + abs(top);
    int column = abs(left) + abs(right);
    printf("left:\t%f\n",left);
    printf("right:\t%f\n",right);
    printf("top:\t%f\n",top);
    printf("bottom:\t%f\n",bottom);
    printf("pixel:\t%f\n",pixel);
    printf("iteration:\t%d\n",n);
    printf("Green:\t%d\n",get_green(400));
    return 0;
}
