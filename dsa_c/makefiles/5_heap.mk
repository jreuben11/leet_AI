# Makefile for heap

CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = ../src
OUTDIR = ../out
TARGET = $(OUTDIR)/5_heap

all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TARGET): $(SRCDIR)/5_heap.c
	$(CC) $(CFLAGS) $(SRCDIR)/5_heap.c -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
