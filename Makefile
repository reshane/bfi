PREFIX ?= /usr/local

CC = gcc

CFLAGS = -std=gnu89 -Wall -pedantic

SRC = bfi.c
OBJ = bfi

$(OBJ): $(SRC)
	$(CC) $(CLFAGS) $(SRC) -o $@

clean:
	rm -f $(OBJ)