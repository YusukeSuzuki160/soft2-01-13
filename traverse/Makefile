CC = gcc
CFLAGS = -Wall -Iinclude 

TARGET = bin/traverse
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:%.c=%.o)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^


.PHONY: clean tmpclean
clean: tmpclean
	rm -f $(OBJS) $(TARGET) 
tmpclean:
	find . -name *~ -exec rm -f {} \;
