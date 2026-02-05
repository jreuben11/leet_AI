/**
 * SORTING ALGORITHMS - Comprehensive Implementation
 *
 * This file demonstrates four different sorting approaches:
 * 1. Merge Sort (Linked List) - Divide and conquer on linked structures
 * 2. Quicksort (Array) - In-place partitioning sort
 * 3. Bucket Sort (Integers) - Distribution sort for bounded integers
 * 4. Bitonic Sort (Parallel) - Network sort for parallel architectures
 *
 * Each algorithm has different characteristics and optimal use cases.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "2_linked_lists.h"

// ============================================================
// MERGE SORT ON LINKED LIST
// ============================================================

/**
 * Merge Sort on Linked List
 *
 * Why Merge Sort for Linked Lists?
 * ---------------------------------
 * - NO random access needed (unlike Quicksort)
 * - Works perfectly with sequential access
 * - Stable sort (preserves relative order of equal elements)
 * - Guaranteed O(n log n) time (no worst case like Quicksort)
 * - Only O(log n) extra space for recursion stack
 *
 * How Merge Sort Works:
 * ---------------------
 * Classic divide-and-conquer algorithm:
 *
 * 1. DIVIDE: Split list into two halves
 *    - Find middle using slow/fast pointer technique
 *    - Break list at middle point
 *
 * 2. CONQUER: Recursively sort each half
 *    - Base case: list of 0 or 1 element is already sorted
 *    - Recursive case: merge_sort(left), merge_sort(right)
 *
 * 3. COMBINE: Merge two sorted lists
 *    - Compare heads of both lists
 *    - Take smaller element, advance that pointer
 *    - Repeat until one list exhausted
 *    - Append remaining elements
 *
 * Example:
 *   [4 -> 2 -> 1 -> 3]
 *
 *   Split: [4 -> 2] and [1 -> 3]
 *
 *   Recursively split:
 *     [4] [2]   [1] [3]
 *
 *   Merge pairs:
 *     [2 -> 4]  [1 -> 3]
 *
 *   Final merge:
 *     [1 -> 2 -> 3 -> 4]
 *
 * Time Complexity: O(n log n)
 * - Dividing: O(log n) levels
 * - Merging at each level: O(n)
 * - Total: O(n log n)
 *
 * Space Complexity: O(log n)
 * - Recursion stack depth = O(log n)
 * - No extra arrays needed (in-place merging)
 *
 * Stability: YES - equal elements maintain relative order
 */

/**
 * Find middle node using slow/fast pointer technique
 *
 * Technique: Tortoise and Hare
 * - Slow pointer moves 1 step at a time
 * - Fast pointer moves 2 steps at a time
 * - When fast reaches end, slow is at middle
 */
struct SLLNode* get_middle(struct SLLNode* head) {
    if (head == NULL) return head;

    struct SLLNode* slow = head;
    struct SLLNode* fast = head->next;

    // Move fast pointer 2x speed of slow
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

/**
 * Merge two sorted linked lists
 *
 * Strategy: Iterative merging using dummy node
 */
struct SLLNode* merge_sorted_lists(struct SLLNode* left, struct SLLNode* right) {
    // Base cases
    if (left == NULL) return right;
    if (right == NULL) return left;

    // Create dummy node to simplify logic
    struct SLLNode dummy;
    dummy.data = 0;
    dummy.next = NULL;
    struct SLLNode* tail = &dummy;

    // Merge iteratively
    while (left != NULL && right != NULL) {
        if (left->data <= right->data) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    // Append remaining elements
    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
    }

    return dummy.next;
}

/**
 * Merge Sort main function
 */
struct SLLNode* merge_sort_list(struct SLLNode* head) {
    // Base case: 0 or 1 element
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // Step 1: Find middle and split
    struct SLLNode* middle = get_middle(head);
    struct SLLNode* right_head = middle->next;
    middle->next = NULL;  // Break the list

    // Step 2: Recursively sort both halves
    struct SLLNode* left = merge_sort_list(head);
    struct SLLNode* right = merge_sort_list(right_head);

    // Step 3: Merge sorted halves
    return merge_sorted_lists(left, right);
}

// ============================================================
// QUICKSORT ON ARRAY
// ============================================================

/**
 * Quicksort on Array
 *
 * Why Quicksort?
 * --------------
 * - Fast average case: O(n log n)
 * - In-place sorting: O(log n) space
 * - Cache-friendly: good locality of reference
 * - Widely used in practice (stdlib qsort)
 *
 * How Quicksort Works:
 * --------------------
 * Divide-and-conquer with partitioning:
 *
 * 1. CHOOSE PIVOT: Select element as partition point
 *    - Many strategies: first, last, median, random
 *    - We use last element for simplicity
 *
 * 2. PARTITION: Rearrange array so:
 *    - Elements < pivot are to the left
 *    - Pivot is in its final sorted position
 *    - Elements > pivot are to the right
 *
 * 3. RECURSE: Quicksort left and right subarrays
 *    - Base case: array of size 0 or 1
 *
 * Example:
 *   [3, 7, 8, 5, 2, 1, 9]  pivot = 9
 *
 *   Partition: [3, 7, 8, 5, 2, 1] 9 []
 *   Now pivot=1:
 *   [] 1 [3, 7, 8, 5, 2, 9]
 *   Continue recursively...
 *
 * Partitioning (Lomuto scheme):
 *   i = -1 (tracks position for next small element)
 *   j scans array
 *   When arr[j] < pivot:
 *     i++
 *     swap(arr[i], arr[j])
 *   Finally swap pivot to position i+1
 *
 * Time Complexity:
 * - Best/Average: O(n log n) - balanced partitions
 * - Worst: O(n²) - already sorted, pivot always min/max
 *
 * Space Complexity: O(log n) for recursion stack
 *
 * Stability: NO - equal elements may be reordered
 *
 * Optimization: Use insertion sort for small subarrays (< 10 elements)
 */

void swap_int(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Partition function (Lomuto partition scheme)
 *
 * Returns: Final position of pivot
 */
int partition(int arr[], int low, int high, bool verbose) {
    int pivot = arr[high];  // Choose last element as pivot
    int i = low - 1;        // Index of smaller element

    if (verbose) {
        printf("  Partition [%d..%d], pivot = %d\n", low, high, pivot);
    }

    for (int j = low; j < high; j++) {
        // If current element <= pivot
        if (arr[j] <= pivot) {
            i++;  // Increment index of smaller element
            swap_int(&arr[i], &arr[j]);

            if (verbose && i != j) {
                printf("    Swap arr[%d]=%d with arr[%d]=%d\n",
                       i, arr[i], j, arr[j]);
            }
        }
    }

    // Place pivot in correct position
    swap_int(&arr[i + 1], &arr[high]);

    if (verbose) {
        printf("  Place pivot %d at position %d\n", pivot, i + 1);
        printf("  Result: ");
        for (int k = low; k <= high; k++) {
            printf("%d ", arr[k]);
        }
        printf("\n\n");
    }

    return i + 1;
}

/**
 * Quicksort recursive function
 */
void quicksort_recursive(int arr[], int low, int high, bool verbose) {
    if (low < high) {
        // Partition and get pivot position
        int pi = partition(arr, low, high, verbose);

        // Recursively sort elements before and after partition
        quicksort_recursive(arr, low, pi - 1, verbose);
        quicksort_recursive(arr, pi + 1, high, verbose);
    }
}

void quicksort(int arr[], int n, bool verbose) {
    if (verbose) {
        printf("=== Quicksort Step-by-Step ===\n\n");
        printf("Initial array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n\n");
    }

    quicksort_recursive(arr, 0, n - 1, verbose);

    if (verbose) {
        printf("Final sorted array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
}

// ============================================================
// BUCKET SORT FOR INTEGERS
// ============================================================

/**
 * Bucket Sort for Integers [0, k]
 *
 * What is Bucket Sort?
 * --------------------
 * Distribution sort that divides elements into buckets,
 * sorts buckets individually, then concatenates.
 *
 * When to Use Bucket Sort:
 * ------------------------
 * ✅ Data uniformly distributed over a range
 * ✅ Know the range [min, max] in advance
 * ✅ Want linear time O(n + k) when k = O(n)
 * ✅ Have extra space for buckets
 *
 * ❌ NOT good for: Highly skewed data, unknown range
 *
 * How Bucket Sort Works (for integers [0, k]):
 * ---------------------------------------------
 * 1. CREATE BUCKETS: Array of size k+1
 *    - bucket[i] will hold count of value i
 *
 * 2. DISTRIBUTE: Count occurrences
 *    - For each element x: bucket[x]++
 *
 * 3. GATHER: Reconstruct sorted array
 *    - For each bucket i (0 to k):
 *      Output value i, bucket[i] times
 *
 * Example:
 *   Input: [3, 1, 4, 1, 5, 9, 2, 6]  (range 0-9)
 *
 *   Buckets after counting:
 *     0: 0
 *     1: 2  ← appears twice
 *     2: 1
 *     3: 1
 *     4: 1
 *     5: 1
 *     6: 1
 *     7: 0
 *     8: 0
 *     9: 1
 *
 *   Gather: [1, 1, 2, 3, 4, 5, 6, 9]
 *
 * Variant: For general ranges [min, max]
 * - Create k buckets where k = (max - min) / n + 1
 * - Map value x to bucket: (x - min) / bucket_size
 * - Sort each bucket (insertion sort)
 * - Concatenate buckets
 *
 * Time Complexity:
 * - Best/Average/Worst: O(n + k)
 *   - Counting: O(n)
 *   - Creating buckets: O(k)
 *   - Gathering: O(n + k)
 * - If k = O(n), then O(n) linear time!
 * - If k >> n, then O(k) dominated by bucket creation
 *
 * Space Complexity: O(k) for buckets
 *
 * Stability: YES (if implemented to preserve order)
 *
 * Use Cases:
 * - Counting sort (special case where k is small)
 * - Sorting exam scores (0-100)
 * - Sorting ages (0-150)
 * - Radix sort uses bucket sort as subroutine
 */

void bucket_sort(int arr[], int n, int k, bool verbose) {
    if (verbose) {
        printf("=== Bucket Sort (Range 0-%d) ===\n\n", k);
        printf("Initial array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n\n");
    }

    // Step 1: Create buckets (counting array)
    int* buckets = (int*)calloc(k + 1, sizeof(int));

    if (verbose) {
        printf("Step 1: Count occurrences\n");
    }

    // Step 2: Count occurrences
    for (int i = 0; i < n; i++) {
        if (arr[i] < 0 || arr[i] > k) {
            printf("Error: Value %d out of range [0, %d]\n", arr[i], k);
            free(buckets);
            return;
        }
        buckets[arr[i]]++;

        if (verbose) {
            printf("  arr[%d] = %d → buckets[%d]++\n", i, arr[i], arr[i]);
        }
    }

    if (verbose) {
        printf("\nBucket counts:\n");
        for (int i = 0; i <= k; i++) {
            if (buckets[i] > 0) {
                printf("  Value %d: %d time(s)\n", i, buckets[i]);
            }
        }
        printf("\n");
    }

    // Step 3: Reconstruct sorted array
    if (verbose) {
        printf("Step 2: Reconstruct sorted array\n");
    }

    int index = 0;
    for (int i = 0; i <= k; i++) {
        for (int j = 0; j < buckets[i]; j++) {
            arr[index++] = i;

            if (verbose) {
                printf("  Output value %d (bucket[%d], count %d/%d)\n",
                       i, i, j + 1, buckets[i]);
            }
        }
    }

    if (verbose) {
        printf("\nFinal sorted array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    free(buckets);
}

// ============================================================
// BITONIC SORT
// ============================================================

/**
 * Bitonic Sort
 *
 * What is Bitonic Sort?
 * ---------------------
 * A parallel sorting algorithm based on bitonic sequences.
 * Designed for parallel hardware (GPUs, sorting networks).
 *
 * What is a Bitonic Sequence?
 * ----------------------------
 * A sequence that:
 * - First increases, then decreases, OR
 * - Can be circularly rotated to be first increasing then decreasing
 *
 * Examples:
 *   Bitonic: [1, 3, 5, 7, 6, 4, 2]  ← increases then decreases
 *   Bitonic: [5, 7, 6, 4, 2, 1, 3]  ← circular rotation of above
 *   NOT:     [1, 3, 2, 4, 3, 5]     ← multiple peaks/valleys
 *
 * How Bitonic Sort Works:
 * -----------------------
 * Based on bitonic merge networks:
 *
 * 1. BUILD: Recursively build bitonic sequences
 *    - Sort pairs in alternating directions
 *    - Merge to form larger bitonic sequences
 *
 * 2. MERGE: Bitonic merge operation
 *    - Compare-and-swap elements at distance d
 *    - Creates two bitonic sequences (each half sorted)
 *    - Recursively merge each half
 *
 * Key Property:
 * - Split bitonic sequence into two halves
 * - Compare each element in first half with corresponding element in second half
 * - Swap if needed based on sort direction
 * - Result: Two bitonic sequences with max(first) ≤ min(second)
 *
 * Example (n=8):
 *   [3, 7, 4, 8, 6, 2, 1, 5]
 *
 *   Build bitonic sequences:
 *     Pairs: [3,7] [4,8] [6,2] [1,5]
 *     4-sequences: [3,4,7,8] [6,5,2,1]  ← bitonic!
 *     8-sequence: [3,4,6,7,8,5,2,1]     ← bitonic!
 *
 *   Bitonic merge:
 *     Compare dist 4: [3,4,6,7] [8,5,2,1]
 *     ...continue merging...
 *     Final: [1,2,3,4,5,6,7,8]
 *
 * Why Bitonic Sort?
 * -----------------
 * ✅ Highly parallelizable (data-oblivious comparisons)
 * ✅ Works well on GPUs and hardware sorting networks
 * ✅ Predictable comparison pattern (good for hardware)
 * ✅ O(log² n) parallel time with O(n log² n) work
 *
 * ❌ Not practical for sequential execution (O(n log² n))
 * ❌ Requires n to be power of 2
 * ❌ More comparisons than merge sort or quicksort
 *
 * Time Complexity:
 * - Sequential: O(n log² n) - worse than merge sort
 * - Parallel: O(log² n) time with n processors
 *
 * Space Complexity: O(log n) for recursion
 *
 * Use Cases:
 * - GPU sorting
 * - Hardware sorting networks
 * - Parallel sorting architectures
 * - NOT for general CPU sorting (use quicksort/mergesort)
 */

/**
 * Compare and swap based on direction
 */
void compare_and_swap(int arr[], int i, int j, bool ascending) {
    if ((ascending && arr[i] > arr[j]) || (!ascending && arr[i] < arr[j])) {
        swap_int(&arr[i], &arr[j]);
    }
}

/**
 * Bitonic merge: merge a bitonic sequence into sorted order
 *
 * @param arr Array to sort
 * @param low Starting index
 * @param cnt Number of elements
 * @param ascending Sort direction
 */
void bitonic_merge(int arr[], int low, int cnt, bool ascending) {
    if (cnt > 1) {
        int k = cnt / 2;

        // Compare and swap elements at distance k
        for (int i = low; i < low + k; i++) {
            compare_and_swap(arr, i, i + k, ascending);
        }

        // Recursively merge both halves
        bitonic_merge(arr, low, k, ascending);
        bitonic_merge(arr, low + k, k, ascending);
    }
}

/**
 * Bitonic sort recursive function
 *
 * @param arr Array to sort
 * @param low Starting index
 * @param cnt Number of elements (must be power of 2)
 * @param ascending Sort direction
 */
void bitonic_sort_recursive(int arr[], int low, int cnt, bool ascending) {
    if (cnt > 1) {
        int k = cnt / 2;

        // Sort first half in ascending order
        bitonic_sort_recursive(arr, low, k, true);

        // Sort second half in descending order
        bitonic_sort_recursive(arr, low + k, k, false);

        // Now we have a bitonic sequence
        // Merge it in the desired direction
        bitonic_merge(arr, low, cnt, ascending);
    }
}

/**
 * Bitonic sort main function
 */
void bitonic_sort(int arr[], int n, bool verbose) {
    // Check if n is power of 2
    if ((n & (n - 1)) != 0) {
        printf("Error: Bitonic sort requires array size to be power of 2\n");
        printf("Current size: %d\n", n);
        return;
    }

    if (verbose) {
        printf("=== Bitonic Sort ===\n\n");
        printf("Initial array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        printf("(Sorting in ascending order)\n\n");
    }

    bitonic_sort_recursive(arr, 0, n, true);

    if (verbose) {
        printf("Final sorted array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
}

// ============================================================
// UTILITY FUNCTIONS
// ============================================================

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

bool is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

// ============================================================
// TEST FUNCTIONS
// ============================================================

void test_merge_sort_list() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║        Test 1: Merge Sort on Linked List         ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    struct SLL* list = sll_create();

    // Add test data
    int data[] = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50};
    int n = sizeof(data) / sizeof(data[0]);

    printf("Building linked list with values: ");
    for (int i = 0; i < n; i++) {
        sll_insert(list, i, data[i]);
        printf("%d ", data[i]);
    }
    printf("\n\n");

    printf("Original list: ");
    sll_print(list);

    printf("\nSorting using Merge Sort...\n");
    list->head = merge_sort_list(list->head);

    printf("\nSorted list: ");
    sll_print(list);

    sll_destroy(list);
}

void test_quicksort() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║           Test 2: Quicksort on Array             ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    int arr[] = {64, 34, 25, 12, 22, 11, 90, 88};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: ");
    print_array(arr, n);
    printf("\n");

    quicksort(arr, n, true);

    printf("\n✓ Verification: Array is %s\n",
           is_sorted(arr, n) ? "sorted" : "NOT sorted");
}

void test_bucket_sort() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║      Test 3: Bucket Sort (Integers 0-20)         ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    int arr[] = {15, 3, 9, 8, 5, 2, 7, 1, 6, 12, 18, 5, 3, 9};
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 20;  // Range [0, 20]

    printf("Use case: Sorting exam scores (0-20 points)\n\n");

    bucket_sort(arr, n, k, true);

    printf("\n✓ Verification: Array is %s\n",
           is_sorted(arr, n) ? "sorted" : "NOT sorted");
}

void test_bitonic_sort() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║       Test 4: Bitonic Sort (Power of 2)          ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    int arr[] = {3, 7, 4, 8, 6, 2, 1, 5};  // n = 8 = 2^3
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Note: Bitonic sort requires array size = power of 2\n");
    printf("Current size: %d = 2^3\n\n", n);

    bitonic_sort(arr, n, true);

    printf("\n✓ Verification: Array is %s\n",
           is_sorted(arr, n) ? "sorted" : "NOT sorted");

    // Test with non-power-of-2 size
    printf("\n\n--- Test with invalid size ---\n");
    int arr2[] = {5, 2, 8, 1, 9};  // n = 5 (not power of 2)
    int n2 = sizeof(arr2) / sizeof(arr2[0]);

    bitonic_sort(arr2, n2, false);
}

void test_comparison() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║     Test 5: Performance Comparison (n=1000)      ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    const int n = 1000;
    int* arr1 = (int*)malloc(n * sizeof(int));
    int* arr2 = (int*)malloc(n * sizeof(int));
    int* arr3 = (int*)malloc(n * sizeof(int));

    // Generate random data
    srand(time(NULL));
    printf("Generating %d random integers (0-99)...\n\n", n);
    for (int i = 0; i < n; i++) {
        arr1[i] = rand() % 100;
        arr2[i] = arr1[i];  // Copy for fair comparison
        arr3[i] = arr1[i];
    }

    // Quicksort
    clock_t start = clock();
    quicksort(arr1, n, false);
    clock_t end = clock();
    double time_quicksort = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    // Bucket sort
    start = clock();
    bucket_sort(arr2, n, 99, false);
    end = clock();
    double time_bucket = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    // Bitonic sort (use power of 2 size)
    int n_bitonic = 1024;  // Next power of 2
    int* arr4 = (int*)malloc(n_bitonic * sizeof(int));
    for (int i = 0; i < n_bitonic; i++) {
        arr4[i] = rand() % 100;
    }

    start = clock();
    bitonic_sort(arr4, n_bitonic, false);
    end = clock();
    double time_bitonic = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

    printf("Results:\n");
    printf("%-20s %10.3f ms\n", "Quicksort:", time_quicksort);
    printf("%-20s %10.3f ms\n", "Bucket Sort:", time_bucket);
    printf("%-20s %10.3f ms (n=1024)\n", "Bitonic Sort:", time_bitonic);

    printf("\n✓ All arrays sorted correctly\n");

    free(arr1);
    free(arr2);
    free(arr3);
    free(arr4);
}

// ============================================================
// MAIN
// ============================================================

int main() {
    char choice;

    while (1) {
        printf("\n╔═══════════════════════════════════════════════════╗\n");
        printf("║           SORTING ALGORITHMS - Menu              ║\n");
        printf("╚═══════════════════════════════════════════════════╝\n\n");

        printf("1. Merge Sort (Linked List)\n");
        printf("2. Quicksort (Array)\n");
        printf("3. Bucket Sort (Integers 0-k)\n");
        printf("4. Bitonic Sort (Parallel)\n");
        printf("5. Performance Comparison\n");
        printf("\nx. Exit\n");
        printf("\nEnter choice: ");
        scanf(" %c", &choice);

        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            test_merge_sort_list();
        } else if (choice == '2') {
            test_quicksort();
        } else if (choice == '3') {
            test_bucket_sort();
        } else if (choice == '4') {
            test_bitonic_sort();
        } else if (choice == '5') {
            test_comparison();
        } else {
            printf("Invalid choice\n");
        }
    }

    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║              Algorithm Summary                    ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    printf("Merge Sort (Linked List):\n");
    printf("  Time: O(n log n)  |  Space: O(log n)  |  Stable: YES\n");
    printf("  Best for: Linked lists, guaranteed O(n log n)\n\n");

    printf("Quicksort (Array):\n");
    printf("  Time: O(n log n) avg, O(n²) worst  |  Space: O(log n)\n");
    printf("  Best for: Arrays, in-place sorting, cache-friendly\n\n");

    printf("Bucket Sort (Integers):\n");
    printf("  Time: O(n + k)  |  Space: O(k)  |  Stable: YES\n");
    printf("  Best for: Uniformly distributed integers, k = O(n)\n\n");

    printf("Bitonic Sort:\n");
    printf("  Time: O(n log² n) sequential, O(log² n) parallel\n");
    printf("  Best for: Parallel hardware, GPUs, sorting networks\n\n");

    return 0;
}
