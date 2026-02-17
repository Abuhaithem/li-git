CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -pedantic -O2 -Iinclude
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:.c=.o)

all: lgit

lgit: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f $(OBJ) lgit

.PHONY: all clean
