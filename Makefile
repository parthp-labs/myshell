CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/builtins.c src/execute_external.c

TARGET = myshell

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)