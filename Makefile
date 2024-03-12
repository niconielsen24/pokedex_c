CC = gcc
CFLAGS = -Wall -Wextra -lcurl -lncurses -g
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
TARGET = client

all:$(TARGET)

$(OBJS):$(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -r *.o $(TARGET)

run:
	./$(TARGET) ditto