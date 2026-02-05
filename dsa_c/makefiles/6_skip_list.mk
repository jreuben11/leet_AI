# Makefile for skip_list

CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = ../src
OUTDIR = ../out
TARGET = $(OUTDIR)/6_skip_list

all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TARGET): $(SRCDIR)/6_skip_list.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
