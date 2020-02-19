CC = gcc
CFLAGS = -c -g -Wall -I/usr/include/tidy
OBJS = htmltidy.o
TARGET = htmltidy.out
LIBS = -lcurl -ltidy

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

htmltidy.o: htmltidy.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o
	rm -f $(TARGET)
