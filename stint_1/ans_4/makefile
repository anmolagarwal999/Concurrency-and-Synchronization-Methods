CC = gcc
CFLAGS = -Wall -g -c
# CFLAGS = -Wall -Werror -g -c
LDFLAGS =
OBJFILES = main.o utils.o performers.o stages.o
TARGET = a

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -Wall -g $(OBJFILES) -lpthread -o $(TARGET) 

main.o: main.c
	$(CC) $(CFLAGS) main.c

utils.o: utils.c
	$(CC) $(CFLAGS) utils.c

performers.o: performers.c
	$(CC) $(CFLAGS) performers.c

stages.o: stages.c
	$(CC) $(CFLAGS) stages.c



