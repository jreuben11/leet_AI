# Makefile for list_search

CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = ../src
OUTDIR = ../out
TARGET = $(OUTDIR)/list_search
OBJ = $(OUTDIR)/2_linked_lists.o

all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

# Compile 2_linked_lists.c as object file without main
$(OBJ): $(SRCDIR)/2_linked_lists.c $(SRCDIR)/2_linked_lists.h
	$(CC) $(CFLAGS) -c -DSKIP_MAIN $< -o $@

# Compile and link list_search with the object file
$(TARGET): $(SRCDIR)/list_search.c $(OBJ) $(SRCDIR)/2_linked_lists.h
	$(CC) $(CFLAGS) $(SRCDIR)/list_search.c $(OBJ) -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
