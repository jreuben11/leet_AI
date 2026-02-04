# Makefile for graphs

CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = ../src
OUTDIR = ../out
TARGET = $(OUTDIR)/9_graphs

all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TARGET): $(SRCDIR)/9_graphs.c
	$(CC) $(CFLAGS) $(SRCDIR)/9_graphs.c -o $@ -lm

clean:
	rm -f $(TARGET)

.PHONY: all clean
