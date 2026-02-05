# Makefile for dynamic_programming

CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = ../src
OUTDIR = ../out
TARGET = $(OUTDIR)/8_dynamic_programming

all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TARGET): $(SRCDIR)/8_dynamic_programming.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
