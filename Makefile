#These are variables. We define CC, CFALGS, CCLINK, and OBJS

CC     = gcc
CFLAGS = -Wall -g -std=c99
CCLINK=$(CC)
OBJS   = grades.o libgrades.so
EXEC=libgrades.so
RM=rm -f *.o *.exe libgrades.so


libgrades.so: grades.o
	$(CCLINK) -shared grades.o -o libgrades.so -llinked-list -L.

grades.o: grades.h grades.c liblinked-list.so
	$(CC) $(CFLAGS) -c -fpic grades.c
	
clean:
	$(RM))
