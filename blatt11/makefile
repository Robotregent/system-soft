CC := gcc
CFLAGS := -std=gnu99 -Wall -g
TARGET := updatedir

$(TARGET): main.o copy_file.o up_dir.o dirlib.o
	$(CC) $(CFLAGS) main.o up_dir.o copy_file.o dirlib.o -o $(TARGET)

main.o: main.c up_dir.h 
	$(CC) $(CFLAGS) -c main.c

dirlib.o: dirlib.c dirlib.h
	$(CC) $(CFLAGS) -c dirlib.c

up_dir.o: up_dir.c up_dir.h copy_file.h dirlib.h
	$(CC) $(CFLAGS) -c up_dir.c

copy_file.o: copy_file.c copy_file.h
	$(CC) $(CFLAGS) -c copy_file.c

clean:
	rm -f *.o $(TARGET) core
	
