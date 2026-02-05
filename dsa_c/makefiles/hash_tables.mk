# Makefile for hash_tables

CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = ../src
OUTDIR = ../out
TARGET = $(OUTDIR)/10_hash_tables

all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TARGET): $(SRCDIR)/10_hash_tables.c
	$(CC) $(CFLAGS) $(SRCDIR)/10_hash_tables.c -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
