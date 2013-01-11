.PHONY:	clean all
all:	mandelbrot
mandelbrot.o: 
	gcc -g -Wall -std=gnu99 -c mandelbrot.c `pkg-config --cflags --libs gtk+-2.0`
mandelbrot: mandelbrot.o
	gcc -g -Wall -std=gnu99 -o mandelbrot mandelbrot.o `pkg-config --cflags --libs gtk+-2.0` -lm
clean:
	rm -f *.o core a.out mandelbrot 
