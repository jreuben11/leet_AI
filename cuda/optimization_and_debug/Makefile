NVCC = nvcc
NVCC_FLAGS = -O3 -arch=sm_89 -std=c++20
OUT_DIR = out
TARGET = $(OUT_DIR)/vectorization

# Default target
all: $(TARGET)

# Create output directory
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Compile the CUDA program
$(TARGET): vectorization.cu | $(OUT_DIR)
	$(NVCC) $(NVCC_FLAGS) $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OUT_DIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
