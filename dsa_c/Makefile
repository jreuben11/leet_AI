# Master Makefile for all DSA C programs

MAKEFILE_DIR = makefiles

# Default target: build all programs
all:
	@$(MAKE) -C $(MAKEFILE_DIR) -f recursion.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f linked_lists.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f skip_list.mk
	@$(MAKE) -C $(MAKEFILE_DIR) -f list_search.mk

# Individual program targets
1_recursion:
	@$(MAKE) -C $(MAKEFILE_DIR) -f recursion.mk

2_linked_lists:
	@$(MAKE) -C $(MAKEFILE_DIR) -f linked_lists.mk

skip_list:
	@$(MAKE) -C $(MAKEFILE_DIR) -f skip_list.mk

list_search:
	@$(MAKE) -C $(MAKEFILE_DIR) -f list_search.mk

# Clean all build artifacts
clean:
	@$(MAKE) -C $(MAKEFILE_DIR) -f recursion.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f linked_lists.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f skip_list.mk clean
	@$(MAKE) -C $(MAKEFILE_DIR) -f list_search.mk clean
	@rm -rf out

# Rebuild everything from scratch
rebuild: clean all

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build all programs (default)"
	@echo "  1_recursion  - Build 1_recursion only"
	@echo "  2_linked_lists - Build 2_linked_lists only"
	@echo "  skip_list    - Build skip_list only"
	@echo "  list_search  - Build list_search only"
	@echo "  clean        - Remove all build artifacts"
	@echo "  rebuild      - Clean and rebuild everything"
	@echo "  help         - Show this help message"

.PHONY: all clean rebuild help 1_recursion 2_linked_lists skip_list list_search
