CC = gcc
CFLAGS = -Wall -g -c
# CFLAGS = -Wall -Werror -g -c
LDFLAGS =
OBJFILES = q3.o utils.o performers.o
TARGET = a

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -Wall -g $(OBJFILES) -lpthread -o $(TARGET) 

q3.o: q3.c
	$(CC) $(CFLAGS) q3.c

utils.o: utils.c
	$(CC) $(CFLAGS) utils.c

performers.o: performers.c
	$(CC) $(CFLAGS) performers.c



