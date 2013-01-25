CC := gcc
CFLAGS := -std=gnu99 -Wall -g
TARGET := updatedir

$(TARGET): main.o copy_file.o
	$(CC) $(CFLAGS) main.o copy_file.o -o $(TARGET)

main.o: main.c copy_file.h 
	$(CC) $(CFLAGS) -c main.c

dirlib.o: dirlib.c dirlib.h
	$(CC) $(CFLAGS) -c dirlib.c

copy_file.o: copy_file.c copy_file.h
	$(CC) $(CFLAGS) -c copy_file.c

clean:
	rm -f *.o $(TARGET) core
	
