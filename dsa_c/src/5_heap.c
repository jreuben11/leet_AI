/*
 * Max Heap Implementation using Dynamic Array
 *
 * A max heap is a complete binary tree where each parent node is greater than
 * or equal to its children. The maximum element is always at the root.
 *
 * Array representation:
 * - For node at index i:
 *   - Parent: (i-1)/2
 *   - Left child: 2*i + 1
 *   - Right child: 2*i + 2
 *
 * Operations:
 * - Insert: O(log n)
 * - Extract Max: O(log n)
 * - Heapify (Percolate Down): O(log n)
 * - Build Heap: O(n)
 * - Heap Sort: O(n log n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ============================================================
// DATA STRUCTURES
// ============================================================

typedef struct {
    int* data;      // Dynamic array to store heap elements
    int size;       // Current number of elements
    int capacity;   // Maximum capacity
} MaxHeap;

// ============================================================
// HELPER FUNCTIONS - ARRAY INDEX CALCULATIONS
// ============================================================

/**
 * Get parent index
 */
int parent(int i) {
    return (i - 1) / 2;
}

/**
 * Get left child index
 */
int left_child(int i) {
    return 2 * i + 1;
}

/**
 * Get right child index
 */
int right_child(int i) {
    return 2 * i + 2;
}

/**
 * Swap two elements in the array
 */
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// ============================================================
// HEAP CREATION AND MANAGEMENT
// ============================================================

/**
 * Create a new max heap
 *
 * @param capacity  Initial capacity of the heap
 * @return          Pointer to newly created heap
 */
MaxHeap* heap_create(int capacity) {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    if (!heap) {
        printf("Memory allocation error\n");
        exit(1);
    }

    heap->data = (int*)malloc(capacity * sizeof(int));
    if (!heap->data) {
        printf("Memory allocation error\n");
        free(heap);
        exit(1);
    }

    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

/**
 * Destroy heap and free memory
 */
void heap_destroy(MaxHeap* heap) {
    if (heap) {
        free(heap->data);
        free(heap);
    }
}

/**
 * Print heap as array
 */
void heap_print_array(MaxHeap* heap) {
    printf("Heap array: [");
    for (int i = 0; i < heap->size; i++) {
        printf("%d%s", heap->data[i], i < heap->size - 1 ? ", " : "");
    }
    printf("]\n");
}

/**
 * Print heap in tree format (simple visualization)
 */
void heap_print_tree(MaxHeap* heap) {
    if (heap->size == 0) {
        printf("Empty heap\n");
        return;
    }

    printf("Heap tree (level-order):\n");
    int level = 0;
    int nodes_in_level = 1;
    int i = 0;

    while (i < heap->size) {
        printf("Level %d: ", level);
        for (int j = 0; j < nodes_in_level && i < heap->size; j++, i++) {
            printf("%d ", heap->data[i]);
        }
        printf("\n");
        level++;
        nodes_in_level *= 2;
    }
}

/**
 * Helper for heap_display_tree: Print subtree recursively
 */
static void display_tree_helper(MaxHeap* heap, int i, int indent, bool is_right) {
    if (i >= heap->size) {
        return;
    }

    // Print right subtree first (top of display)
    int right = right_child(i);
    if (right < heap->size) {
        display_tree_helper(heap, right, indent + 4, true);
    }

    // Print current node with proper indentation
    if (indent > 0) {
        for (int j = 0; j < indent - 4; j++) {
            printf(" ");
        }
        printf(is_right ? " ┌──" : " └──");
    }
    printf("%d\n", heap->data[i]);

    // Print left subtree (bottom of display)
    int left = left_child(i);
    if (left < heap->size) {
        display_tree_helper(heap, left, indent + 4, false);
    }
}

/**
 * Display heap as a visual tree structure
 *
 * Shows the heap in a sideways tree format with proper structure
 * Better visualization than level-order for understanding tree relationships
 */
void heap_display_tree(MaxHeap* heap) {
    if (heap->size == 0) {
        printf("Empty heap\n");
        return;
    }

    printf("Heap structure (rotated 90° clockwise):\n");
    printf("(Right child above, left child below)\n\n");
    display_tree_helper(heap, 0, 0, false);
    printf("\n");
}

// ============================================================
// HEAP OPERATIONS
// ============================================================

/**
 * Percolate Up (Bubble Up)
 *
 * Maintains max heap property by moving element up the tree
 * Used after insertion
 *
 * @param heap  Pointer to the heap
 * @param i     Index of element to percolate up
 *
 * Time: O(log n) - height of heap
 */
void percolate_up(MaxHeap* heap, int i) {
    // While not at root and current element is greater than parent
    while (i > 0 && heap->data[i] > heap->data[parent(i)]) {
        // Swap with parent
        swap(&heap->data[i], &heap->data[parent(i)]);
        i = parent(i);  // Move up
    }
}

/**
 * Percolate Down (Heapify)
 *
 * Maintains max heap property by moving element down the tree
 * Used after extraction and in heap sort
 *
 * @param heap  Pointer to the heap
 * @param i     Index of element to percolate down
 *
 * Algorithm:
 * 1. Find largest among node, left child, and right child
 * 2. If node is not largest, swap with largest child
 * 3. Recursively heapify the affected subtree
 *
 * Time: O(log n) - height of heap
 */
void percolate_down(MaxHeap* heap, int i) {
    int largest = i;           // Initialize largest as root
    int left = left_child(i);
    int right = right_child(i);

    // Check if left child exists and is greater than root
    if (left < heap->size && heap->data[left] > heap->data[largest]) {
        largest = left;
    }

    // Check if right child exists and is greater than largest so far
    if (right < heap->size && heap->data[right] > heap->data[largest]) {
        largest = right;
    }

    // If largest is not root, swap and continue heapifying
    if (largest != i) {
        swap(&heap->data[i], &heap->data[largest]);
        percolate_down(heap, largest);  // Recursively heapify affected subtree
    }
}

/**
 * Insert element into heap
 *
 * @param heap   Pointer to the heap
 * @param value  Value to insert
 *
 * Algorithm:
 * 1. Add element at end (maintains complete tree property)
 * 2. Percolate up to maintain max heap property
 *
 * Time: O(log n)
 */
void heap_insert(MaxHeap* heap, int value) {
    if (heap->size >= heap->capacity) {
        printf("Heap is full! Cannot insert %d\n", value);
        return;
    }

    // Insert at end
    heap->data[heap->size] = value;
    heap->size++;

    // Percolate up to maintain heap property
    percolate_up(heap, heap->size - 1);
}

/**
 * Extract maximum element (root)
 *
 * @param heap  Pointer to the heap
 * @return      Maximum element, or -1 if heap is empty
 *
 * Algorithm:
 * 1. Save root element (maximum)
 * 2. Move last element to root
 * 3. Percolate down to maintain heap property
 *
 * Time: O(log n)
 */
int heap_extract_max(MaxHeap* heap) {
    if (heap->size <= 0) {
        printf("Heap is empty!\n");
        return -1;
    }

    if (heap->size == 1) {
        heap->size--;
        return heap->data[0];
    }

    // Store max value (root)
    int max = heap->data[0];

    // Move last element to root
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    // Percolate down to maintain heap property
    percolate_down(heap, 0);

    return max;
}

/**
 * Peek at maximum element without removing it
 */
int heap_peek_max(MaxHeap* heap) {
    if (heap->size <= 0) {
        printf("Heap is empty!\n");
        return -1;
    }
    return heap->data[0];
}

/**
 * Build heap from unsorted array
 *
 * @param arr   Array of elements
 * @param size  Number of elements
 * @return      Pointer to created heap
 *
 * Algorithm:
 * 1. Copy all elements to heap array
 * 2. Start from last non-leaf node (size/2 - 1)
 * 3. Heapify each node going backwards to root
 *
 * Time: O(n) - surprisingly linear, not O(n log n)!
 */
MaxHeap* heap_build_from_array(int arr[], int size) {
    MaxHeap* heap = heap_create(size * 2);  // Extra capacity

    // Copy all elements
    for (int i = 0; i < size; i++) {
        heap->data[i] = arr[i];
    }
    heap->size = size;

    // Start from last non-leaf and heapify all nodes
    for (int i = size / 2 - 1; i >= 0; i--) {
        percolate_down(heap, i);
    }

    return heap;
}

// ============================================================
// HEAP SORT
// ============================================================

/**
 * Heapify for heap sort (works on array directly)
 *
 * @param arr   Array to heapify
 * @param n     Size of heap
 * @param i     Index to heapify from
 */
void heapify_array(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify_array(arr, n, largest);
    }
}

/**
 * Heap Sort
 *
 * @param arr   Array to sort
 * @param size  Number of elements
 *
 * Algorithm:
 * 1. Build max heap from array - O(n)
 * 2. Repeatedly:
 *    a. Swap root (max) with last element
 *    b. Reduce heap size
 *    c. Heapify root - O(log n)
 * 3. Repeat n times
 *
 * Time: O(n log n)
 * Space: O(1) - in-place sorting
 *
 * Result: Array sorted in ascending order
 */
void heap_sort(int arr[], int size) {
    // Step 1: Build max heap
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify_array(arr, size, i);
    }

    // Step 2: Extract elements one by one
    for (int i = size - 1; i > 0; i--) {
        // Move current root (max) to end
        swap(&arr[0], &arr[i]);

        // Heapify root with reduced heap
        heapify_array(arr, i, 0);
    }
}

// ============================================================
// TEST FUNCTIONS
// ============================================================

void test_basic_operations() {
    printf("\n=== Test 1: Basic Heap Operations ===\n");

    MaxHeap* heap = heap_create(20);

    printf("\nInserting: 10, 20, 15, 30, 40\n");
    heap_insert(heap, 10);
    heap_insert(heap, 20);
    heap_insert(heap, 15);
    heap_insert(heap, 30);
    heap_insert(heap, 40);

    heap_print_array(heap);
    heap_print_tree(heap);
    heap_display_tree(heap);

    printf("\nPeek max: %d\n", heap_peek_max(heap));

    printf("\nExtracting max elements:\n");
    while (heap->size > 0) {
        printf("Extracted: %d, ", heap_extract_max(heap));
        heap_print_array(heap);
    }

    heap_destroy(heap);
}

void test_percolate_down() {
    printf("\n=== Test 2: Percolate Down (Heapify) ===\n");

    // Create array where only root violates heap property
    // Children are valid heaps, but root is too small
    printf("\nScenario: Root element is smaller than children\n");
    printf("(Both subtrees are valid max heaps)\n");
    int arr[] = {10, 30, 20, 15, 25};
    int size = 5;

    printf("\nInitial array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("Tree structure: Root=10, but left child=30 (violates max heap!)\n");
    printf("                Left subtree (30,15,25) is valid\n");
    printf("                Right subtree (20) is valid\n");

    // Create heap and copy data
    MaxHeap* heap = heap_create(10);
    for (int i = 0; i < size; i++) {
        heap->data[i] = arr[i];
    }
    heap->size = size;

    printf("\nBefore percolate down:\n");
    heap_print_array(heap);
    heap_print_tree(heap);
    heap_display_tree(heap);

    // Percolate down from root
    printf("\nPercolating down from root (index 0)...\n");
    percolate_down(heap, 0);

    printf("\nAfter percolate down:\n");
    heap_print_array(heap);
    heap_print_tree(heap);
    heap_display_tree(heap);

    printf("\nVerifying max heap property:\n");
    bool valid = true;
    for (int i = 0; i < heap->size / 2; i++) {
        int left = left_child(i);
        int right = right_child(i);

        if (left < heap->size && heap->data[i] < heap->data[left]) {
            printf("Violation at %d: parent %d < left child %d\n",
                   i, heap->data[i], heap->data[left]);
            valid = false;
        }
        if (right < heap->size && heap->data[i] < heap->data[right]) {
            printf("Violation at %d: parent %d < right child %d\n",
                   i, heap->data[i], heap->data[right]);
            valid = false;
        }
    }

    if (valid) {
        printf("✓ Heap property satisfied!\n");
    }

    heap_destroy(heap);
}

void test_build_heap() {
    printf("\n=== Test 3: Build Heap from Array ===\n");

    int arr[] = {4, 10, 3, 5, 1, 8, 9, 2, 7, 6};
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("\nOriginal array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    MaxHeap* heap = heap_build_from_array(arr, size);

    printf("\nHeap after build:\n");
    heap_print_array(heap);
    heap_print_tree(heap);
    heap_display_tree(heap);

    printf("\nExtracting all elements (should be in descending order):\n");
    while (heap->size > 0) {
        printf("%d ", heap_extract_max(heap));
    }
    printf("\n");

    heap_destroy(heap);
}

void test_heap_sort() {
    printf("\n=== Test 4: Heap Sort ===\n");

    int arr1[] = {12, 11, 13, 5, 6, 7};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);

    printf("\nTest 4a: Small array\n");
    printf("Before sort: ");
    for (int i = 0; i < size1; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");

    heap_sort(arr1, size1);

    printf("After sort:  ");
    for (int i = 0; i < size1; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");

    // Test with larger array
    int arr2[] = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50, 23, 36, 18, 77, 29};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);

    printf("\nTest 4b: Larger array\n");
    printf("Before sort: ");
    for (int i = 0; i < size2; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");

    heap_sort(arr2, size2);

    printf("After sort:  ");
    for (int i = 0; i < size2; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");

    // Verify sorted
    bool sorted = true;
    for (int i = 1; i < size2; i++) {
        if (arr2[i] < arr2[i-1]) {
            sorted = false;
            break;
        }
    }
    printf("\n%s Array is correctly sorted!\n", sorted ? "✓" : "✗");
}

// ============================================================
// MAIN
// ============================================================

int main() {
    char choice;

    while (1) {
        printf("\n=== Max Heap Menu ===\n");
        printf("1. Test Basic Operations (Insert, Extract)\n");
        printf("2. Test Percolate Down (Heapify)\n");
        printf("3. Test Build Heap from Array\n");
        printf("4. Test Heap Sort\n");
        printf("x. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            test_basic_operations();
        } else if (choice == '2') {
            test_percolate_down();
        } else if (choice == '3') {
            test_build_heap();
        } else if (choice == '4') {
            test_heap_sort();
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
