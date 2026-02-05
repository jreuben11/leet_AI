# Master Makefile for all DSA C programs

MAKEFILE_DIR = makefiles

# Default target: build all programs
all:
	@$(MAKE) -C $(MAKEFILE_DIR) -f recursion.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f linked_lists.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f stacks_and_queues.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f trees.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f heap.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f skip_list.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f list_search.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f dynamic_programming.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f graphs.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f hash_tables.mk

# Individual program targets
1_recursion:
	@$(MAKE) -C $(MAKEFILE_DIR) -f recursion.mk

2_linked_lists:
	@$(MAKE) -C $(MAKEFILE_DIR) -f linked_lists.mk

3_stacks_and_queues:
	@$(MAKE) -C $(MAKEFILE_DIR) -f stacks_and_queues.mk

4_trees:
	@$(MAKE) -C $(MAKEFILE_DIR) -f trees.mk

7_heap:
	@$(MAKE) -C $(MAKEFILE_DIR) -f heap.mk

skip_list:
	@$(MAKE) -C $(MAKEFILE_DIR) -f skip_list.mk

list_search:
	@$(MAKE) -C $(MAKEFILE_DIR) -f list_search.mk

dynamic_programming:
	@$(MAKE) -C $(MAKEFILE_DIR) -f dynamic_programming.mk

9_graphs:
	@$(MAKE) -C $(MAKEFILE_DIR) -f graphs.mk

10_hash_tables:
	@$(MAKE) -C $(MAKEFILE_DIR) -f hash_tables.mk

# Clean all build artifacts
clean:
	@$(MAKE) -C $(MAKEFILE_DIR) -f recursion.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f linked_lists.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f stacks_and_queues.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f trees.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f heap.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f skip_list.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f list_search.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f dynamic_programming.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f graphs.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f hash_tables.mk clean
	@rm -rf out

# Rebuild everything from scratch
rebuild: clean all

# Help target
help:
	@echo "Available targets:"
	@echo "  all                  - Build all programs (default)"
	@echo "  1_recursion          - Build 1_recursion only"
	@echo "  2_linked_lists       - Build 2_linked_lists only"
	@echo "  3_stacks_and_queues  - Build 3_stacks_and_queues only"
	@echo "  4_trees              - Build 4_trees only"
	@echo "  7_heap               - Build 7_heap only"
	@echo "  skip_list            - Build skip_list only"
	@echo "  list_search          - Build list_search only"
	@echo "  dynamic_programming  - Build dynamic_programming only"
	@echo "  9_graphs             - Build 9_graphs only"
	@echo "  10_hash_tables       - Build 10_hash_tables only"
	@echo "  clean                - Remove all build artifacts"
	@echo "  rebuild              - Clean and rebuild everything"
	@echo "  help                 - Show this help message"

.PHONY: all clean rebuild help 1_recursion 2_linked_lists 3_stacks_and_queues 4_trees 7_heap skip_list list_search dynamic_programming 9_graphs 10_hash_tables
