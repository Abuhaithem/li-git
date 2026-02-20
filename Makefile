CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -pedantic -O2 -Iinclude
LDFLAGS = -lcrypto
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:.c=.o)

all: lgit

lgit: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) lgit

.PHONY: all clean
