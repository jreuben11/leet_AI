# Makefile for trees

CC = gcc
CFLAGS = -Wall -Wextra -g
SRCDIR = ../src
OUTDIR = ../out
TARGET = $(OUTDIR)/4_trees
OBJ = $(OUTDIR)/2_linked_lists.o
STACKS_OBJ = $(OUTDIR)/3_stacks_and_queues.o

all: $(OUTDIR) $(OBJ) $(STACKS_OBJ) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

# Compile 2_linked_lists.c to object file with SKIP_MAIN flag
$(OBJ): $(SRCDIR)/2_linked_lists.c $(SRCDIR)/2_linked_lists.h
	$(CC) $(CFLAGS) -c -DSKIP_MAIN $< -o $@

# Compile 3_stacks_and_queues.c to object file (need stack and queue functions)
$(STACKS_OBJ): $(SRCDIR)/3_stacks_and_queues.c $(SRCDIR)/2_linked_lists.h $(SRCDIR)/stacks_queues.h
	$(CC) $(CFLAGS) -c -DSKIP_MAIN $< -o $@

# Link trees with linked_lists and stacks_queues object files
$(TARGET): $(SRCDIR)/4_trees.c $(OBJ) $(STACKS_OBJ) $(SRCDIR)/stacks_queues.h
	$(CC) $(CFLAGS) $(SRCDIR)/4_trees.c $(OBJ) $(STACKS_OBJ) -o $@

clean:
	rm -f $(TARGET) $(STACKS_OBJ)

.PHONY: all clean
