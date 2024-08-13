CC = gcc
all : chat_thread_sync 

chat_thread_sync :   chat_thread_sync.c
	$(CC) -o chat_thread_sync chat_thread_sync.c -l ncurses -l pthread

clean:
	rm -rf *.o chat_thread_sync 
