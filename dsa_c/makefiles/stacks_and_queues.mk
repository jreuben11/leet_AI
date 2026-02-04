# Makefile for stacks_and_queues

CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = ../src
OUTDIR = ../out
TARGET = $(OUTDIR)/3_stacks_and_queues
OBJ = $(OUTDIR)/2_linked_lists.o

all: $(OUTDIR) $(OBJ) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

# Compile 2_linked_lists.c to object file with SKIP_MAIN flag
$(OBJ): $(SRCDIR)/2_linked_lists.c $(SRCDIR)/2_linked_lists.h
	$(CC) $(CFLAGS) -c -DSKIP_MAIN $< -o $@

# Link stacks_and_queues with linked_lists object file
$(TARGET): $(SRCDIR)/3_stacks_and_queues.c $(OBJ) $(SRCDIR)/2_linked_lists.h $(SRCDIR)/stacks_queues.h
	$(CC) $(CFLAGS) $(SRCDIR)/3_stacks_and_queues.c $(OBJ) -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
