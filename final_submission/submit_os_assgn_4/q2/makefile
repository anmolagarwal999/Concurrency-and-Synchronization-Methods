CC = gcc
# CFLAGS = -Wall -g -c
CFLAGS = -Wall -g -c
LDFLAGS =
OBJFILES = q2.o companies.o students.o hospitals.o utils.o
TARGET = a

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -Wall -g $(OBJFILES) -lpthread -o $(TARGET) 

q2.o: q2.c
	$(CC) $(CFLAGS) q2.c

companies.o: companies.c
	$(CC) $(CFLAGS) companies.c

students.o: students.c
	$(CC) $(CFLAGS) students.c

hospitals.o: hospitals.c
	$(CC) $(CFLAGS) hospitals.c


utils.o: utils.c
	$(CC) $(CFLAGS) utils.c


