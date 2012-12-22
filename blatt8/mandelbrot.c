/*
 Auf Thales bitte mit dem c99 übersetztn:  
 c99 -o mandelbrot mandelbrot.c `pkg-config --cflags --libs gtk+-2.0` -lm
 
 Oder mitgelifertes Makefile nutzen.
*/


#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#define LEFT    -1.5
#define RIGHT   1.5
#define TOP     1.5
#define BOTTOM  -1.5
#define PIXEL   1000
#define N       500

double left = LEFT;
double right= RIGHT;
double bottom= BOTTOM;
double top= TOP;
double pixel= PIXEL;
int n = N;

void print_usage(){
    printf("-l\tMinimum reelle Achse (\"left\") Default: %f\n",LEFT);
    printf("-r\tMaximum reelle Achse (\"right\") Default: %f\n",RIGHT);
    printf("-b\tMinimum imaginäre Achse (\"bottom\") Default: %f\n",BOTTOM);
    printf("-t\tMaximum imaginäre Achse (\"top\") Default: %f\n",TOP);
    printf("-p\tPixel pro Einheit auf imaginärer/reeller Achse. Default: %d\n", PIXEL);
    printf("-i\tMaximale Anzahl der Iterationen. Default: %d\n",N);
    printf("Leerzeichen zwischen allen Argumenten notwendig\n");
}
// Nicht notwendig
//double **get_matrix(int row, int col){
//    double **res = (double **) malloc(row*sizeof(double **));
//    my_assert(res, "Out of memmory\n\0");
//    for (int i = 0; i< row; i++){
//        res[i] = (double *) malloc(col*sizeof(double *));
//        my_assert(res[i], "Out of memmory\n\0");
//    }
//    return res;
//}


double d_abs(double d){
    if (d<0)
        d = -1 * d;
    return d;
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
// Prüft, ob c in m
// Returnvalue:
//	-1 wenn c in m
//	Escapetime wenn c nicht in m
int mandel(double complex c){
    int i;
    double complex prev=0; 
    for (i=2; i <= n; i++){
        prev = prev * prev + c;
        if (abs_c(prev) > 4){
            return i;
        }
    }
    return -1;
}
// String verarbeitung wie im Beispiel
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


// Verarbeitung der Kommandozeile
// Belibige Reihenfolge der Argumente. Leerzeichen zwischen Options-Flag und Options-Wert.
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
    if (c>0 || *++v != '\0'){
        print_usage();
        exit(1);
    }
    my_assert(left<right,"Left muss kleiner right sein.\n\0");
    my_assert(bottom<top,"Bottom muss kleiner top sein.\n\0");
    my_assert(pixel>0,"Keine Pixel?\n\0");
    my_assert(n>0,"Zu wenig Iterationen.\n\0");
}

// Ermittelt Grünwert entsprechend der Escapetime
int get_green(int it){
    return pow((((double) it)/((double) n)),0.2)*255;
}
// Transformiert Index des zweidimensionalen Arrays auf die kartesischen Koordinaten der komplexen Ebene
double complex get_c_from_pixel(int z, int s){
    return ((s/(double)pixel)+left) + (((-1 * z)/(double) pixel)+top)*I;
}
int main(int argc, char **argv){
    parse_args(argc, argv);
    int row =(d_abs(bottom) + d_abs(top))*pixel;
    int column =(d_abs(left) + d_abs(right))*pixel;
    int green;
    int div, i;
    printf("left:\t%f\n",left);
    printf("right:\t%f\n",right);
    printf("top:\t%f\n",top);
    printf("bottom:\t%f\n",bottom);
    printf("pixel:\t%f\n",pixel);
    printf("iteration:\t%d\n",n);
    
    // Verwendung der GTK+ Bibliothek wie im Beispiel
    g_type_init();
    GdkPixbuf* pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, column, row);
    int n_channels = gdk_pixbuf_get_n_channels (pixbuf); 
    int rowstride = gdk_pixbuf_get_rowstride (pixbuf); 
    guchar* pixels = gdk_pixbuf_get_pixels (pixbuf);

    for (int z = 0; z < row; ++z) {
        for (int s = 0; s < column; ++s) {
            guchar* p = pixels + z * rowstride + s * n_channels;
            green = mandel(get_c_from_pixel(z,s));			// Hole komplexe Zahl. Teste, ob c in m
            if(green)
                green = get_green(green);				// RGB Wert errechen
            else
                green = 0;

            p[0] = 0; 
            p[1] = green; 
            p[2] = 0; 

        }
        //Status
        printf("Rechne: [");
        div =(z /(double) row)*100;
        for (i=0; i<div/2;i++){
            putchar('=');
        }
        for (++i;i<50;i++){
            putchar(' ');
        }
        printf("]");
        fflush(stdout);

        printf("\r");
    }
    printf("\n");
    
    gdk_pixbuf_save(pixbuf, "mandel.jpg", "jpeg", NULL, "quality", "100", NULL);
    return 0;
}
