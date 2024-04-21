CC=gcc
SRC=src
BIN=bin
VENDOR=vendor
CFLAGS=-std=c99 -g -Wall -Werror -pedantic -D_FORTIFY_SOURCE=2 -DPLATFORM_DESKTOP -I $(VENDOR)/include
LIBS=$(VENDOR)/lib/libraylib.a -lm -lpthread -ldl -lrt

SRCS=$(wildcard $(SRC)/*.c)
BINS=$(SRCS:$(SRC)/%.c=%)

all:
	$(CC) $(CFLAGS) -o $(BIN)/demo $(SRCS) $(LIBS)

clean:
	$(RM) $(BIN)/*
