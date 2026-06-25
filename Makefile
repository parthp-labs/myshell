CC = gcc

CFLAGS = -Wextra -Iinclude

SRC = src/main.c src/builtins.c src/executor.c src/parser.c

TARGET = myshell

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)