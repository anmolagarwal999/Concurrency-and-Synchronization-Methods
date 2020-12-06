CC = gcc
CFLAGS = -Wall -g -c
# CFLAGS = -Wall -Werror -g -c
LDFLAGS =
OBJFILES = merge_sort.o
TARGET = a

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -Wall -g $(OBJFILES) -lpthread -o $(TARGET) 

merge_sort.o: merge_sort.c
	$(CC) $(CFLAGS) merge_sort.c



