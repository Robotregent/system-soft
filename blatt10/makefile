# nuetzliche Variablen definieren
CC := gcc
CFLAGS := -std=gnu99 -Wall
TARGET := pedigree

$(TARGET): main.o pedigree.o readped.o printped.o
	$(CC) $(CFLAGS) main.o pedigree.o readped.o printped.o -o $(TARGET)

main.o: main.c pedigree.h readped.h printped.h
	$(CC) $(CFLAGS) -c main.c

readped.o: readped.c readped.h pedigree.h
	$(CC) $(CFLAGS) -c readped.c

printped.o: printped.c printped.h pedigree.h
	$(CC) $(CFLAGS) -c printped.c

pedigree.o: pedigree.c pedigree.h 
	$(CC) $(CFLAGS) -c pedigree.c
	
clean:
	rm -f *.o $(TARGET)
	
