# Master Makefile for all DSA C programs

MAKEFILE_DIR = makefiles

# Default target: build all programs
all:
	@$(MAKE) -C $(MAKEFILE_DIR) -f recursion.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f linked_lists.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f stacks_and_queues.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f trees.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f 5_heap.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f 6_skip_list.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f 7_list_search.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f 8_dynamic_programming.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f graphs.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f hash_tables.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f sort.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f 12_strings.mk

# Individual program targets
1_recursion:
	@$(MAKE) -C $(MAKEFILE_DIR) -f recursion.mk

2_linked_lists:
	@$(MAKE) -C $(MAKEFILE_DIR) -f linked_lists.mk

3_stacks_and_queues:
	@$(MAKE) -C $(MAKEFILE_DIR) -f stacks_and_queues.mk

4_trees:
	@$(MAKE) -C $(MAKEFILE_DIR) -f trees.mk

5_heap:
	@$(MAKE) -C $(MAKEFILE_DIR) -f 5_heap.mk

6_skip_list:
	@$(MAKE) -C $(MAKEFILE_DIR) -f 6_skip_list.mk

7_list_search:
	@$(MAKE) -C $(MAKEFILE_DIR) -f 7_list_search.mk

8_dynamic_programming:
	@$(MAKE) -C $(MAKEFILE_DIR) -f 8_dynamic_programming.mk

9_graphs:
	@$(MAKE) -C $(MAKEFILE_DIR) -f graphs.mk

10_hash_tables:
	@$(MAKE) -C $(MAKEFILE_DIR) -f hash_tables.mk

11_sort:
	@$(MAKE) -C $(MAKEFILE_DIR) -f sort.mk

12_strings:
	@$(MAKE) -C $(MAKEFILE_DIR) -f 12_strings.mk

# Clean all build artifacts
clean:
	@$(MAKE) -C $(MAKEFILE_DIR) -f recursion.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f linked_lists.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f stacks_and_queues.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f trees.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f 5_heap.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f 6_skip_list.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f 7_list_search.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f 8_dynamic_programming.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f graphs.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f hash_tables.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f sort.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f 12_strings.mk clean
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
	@echo "  5_heap               - Build 5_heap only"
	@echo "  6_skip_list          - Build 6_skip_list only"
	@echo "  7_list_search        - Build 7_list_search only"
	@echo "  8_dynamic_programming - Build 8_dynamic_programming only"
	@echo "  9_graphs             - Build 9_graphs only"
	@echo "  10_hash_tables       - Build 10_hash_tables only"
	@echo "  11_sort              - Build 11_sort only"
	@echo "  12_strings           - Build 12_strings only"
	@echo "  clean                - Remove all build artifacts"
	@echo "  rebuild              - Clean and rebuild everything"
	@echo "  help                 - Show this help message"

.PHONY: all clean rebuild help 1_recursion 2_linked_lists 3_stacks_and_queues 4_trees 5_heap 6_skip_list 7_list_search 8_dynamic_programming 9_graphs 10_hash_tables 11_sort 12_strings
