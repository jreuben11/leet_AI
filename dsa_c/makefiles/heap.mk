# Makefile for heap

CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = ../src
OUTDIR = ../out
TARGET = $(OUTDIR)/7_heap

all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TARGET): $(SRCDIR)/7_heap.c
	$(CC) $(CFLAGS) $(SRCDIR)/7_heap.c -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
