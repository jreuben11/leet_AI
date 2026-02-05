# Makefile for sorting algorithms

CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = ../src
OUTDIR = ../out
TARGET = $(OUTDIR)/11_sort

all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TARGET): $(SRCDIR)/11_sort.c $(SRCDIR)/2_linked_lists.c
	$(CC) $(CFLAGS) -DSKIP_MAIN $(SRCDIR)/11_sort.c $(SRCDIR)/2_linked_lists.c -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
