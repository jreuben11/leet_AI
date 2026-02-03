# Master Makefile for all DSA C programs

# List of programs
PROGRAMS = 1_recursion 2_linked_lists skip_list list_search

# Corresponding Makefiles
MAKEFILES = Makefile.recursion Makefile.linked_lists Makefile.skip_list Makefile.list_search

# Default target: build all programs
all:
	@$(MAKE) -f Makefile.recursion
	@$(MAKE) -f Makefile.linked_lists
	@$(MAKE) -f Makefile.skip_list
	@$(MAKE) -f Makefile.list_search

# Individual program targets
1_recursion:
	@$(MAKE) -f Makefile.recursion

2_linked_lists:
	@$(MAKE) -f Makefile.linked_lists

skip_list:
	@$(MAKE) -f Makefile.skip_list

list_search:
	@$(MAKE) -f Makefile.list_search

# Clean all build artifacts
clean:
	@$(MAKE) -f Makefile.recursion clean
	@$(MAKE) -f Makefile.linked_lists clean
	@$(MAKE) -f Makefile.skip_list clean
	@$(MAKE) -f Makefile.list_search clean
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
