CC = gcc
all : read write 

write :   write.c
	$(CC) -o write write.c -l ncurses

read :   read.c
	$(CC) -o read read.c -l ncurses

clean:
	rm -rf *.o write read 
