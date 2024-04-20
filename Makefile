CC=gcc
CFLAGS=-std=c17 -Wall -Werror -pedantic -D_FORTIFY_SOURCE=2 -O1
SRC=src
BIN=bin

SRCS=$(wildcard $(SRC)/*.c)
BINS=$(SRCS:$(SRC)/%.c=%)

all:
	$(CC) $(CFLAGS) -o $(BIN)/demo $(SRCS)

clean:
	$(RM) $(BIN)/*
