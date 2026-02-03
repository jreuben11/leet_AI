#include <stdio.h>
#include <stdlib.h>
#include "2_linked_lists.h"

// Find the nth node from the end of the list using two-pointer technique (runner technique)
//
// Algorithm:
//   1. Initialize two pointers (slow and fast) at the head
//   2. Move fast pointer n nodes ahead
//   3. Move both pointers together until fast reaches NULL
//   4. When fast reaches NULL, slow will be at the nth node from end
//
// Why it works:
//   - The gap between slow and fast is always n nodes
//   - When fast reaches NULL (end), slow is exactly n nodes from end
//
// Time complexity: O(n) where n is the list length
// Space complexity: O(1) - only two pointers used
//
// Returns pointer to the node if found, NULL otherwise
// n = 1 means last node, n = 2 means second-to-last, etc.
struct SLLNode* search_from_end(struct SLL* list, int n) {
    if (list == NULL || list->head == NULL) {
        printf("List is empty or does not exist\n");
        return NULL;
    }

    if (n <= 0) {
        printf("Invalid position: n must be positive\n");
        return NULL;
    }

    // Two pointer technique:
    // 1. Move 'fast' pointer n nodes ahead
    // 2. Move both 'slow' and 'fast' together until 'fast' reaches end
    // 3. 'slow' will be at the nth node from the end

    struct SLLNode* slow = list->head;
    struct SLLNode* fast = list->head;

    // Move fast pointer n nodes ahead
    for (int i = 0; i < n; i++) {
        if (fast == NULL) {
            printf("List has fewer than %d nodes\n", n);
            return NULL;
        }
        fast = fast->next;
    }

    // Move both pointers until fast reaches the end
    while (fast != NULL) {
        slow = slow->next;
        fast = fast->next;
    }

    return slow;
}

// Find the nth node from the end and return its data
// Returns 1 if found (stores data in *result), 0 otherwise
int search_from_end_data(struct SLL* list, int n, int* result) {
    struct SLLNode* node = search_from_end(list, n);
    if (node != NULL) {
        *result = node->data;
        return 1;
    }
    return 0;
}

// Helper function to demonstrate the two-pointer technique visually
void demonstrate_search(struct SLL* list, int n) {
    printf("\n--- Demonstrating search for %dth node from end ---\n", n);

    if (list == NULL || list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    printf("List contents: ");
    sll_print(list);
    printf("List length: %d\n", sll_length(list));

    struct SLLNode* node = search_from_end(list, n);
    if (node != NULL) {
        printf("The %dth node from the end has data: %d\n", n, node->data);
    } else {
        printf("Could not find %dth node from end\n", n);
    }
}

// ========================================
// Floyd's Cycle Detection Algorithm
// ========================================

// Detect if the list has a cycle using Floyd's algorithm (tortoise and hare)
//
// Algorithm:
//   1. Initialize two pointers (slow and fast) at the head
//   2. Move slow pointer 1 step at a time
//   3. Move fast pointer 2 steps at a time
//   4. If they meet, there's a cycle
//   5. If fast reaches NULL, there's no cycle
//
// Why it works:
//   - If there's a cycle, fast will eventually catch up to slow inside the cycle
//   - Fast moves twice as fast, so it closes the gap by 1 position each iteration
//   - They must meet if a cycle exists
//
// Time complexity: O(n) where n is the number of nodes
// Space complexity: O(1) - only two pointers used
//
// Returns 1 if cycle detected, 0 otherwise
int detect_cycle(struct SLL* list) {
    if (list == NULL || list->head == NULL) {
        return 0;
    }

    struct SLLNode* slow = list->head;
    struct SLLNode* fast = list->head;

    // Move slow by 1, fast by 2
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;           // Move 1 step
        fast = fast->next->next;     // Move 2 steps

        // If they meet, there's a cycle
        if (slow == fast) {
            return 1;
        }
    }

    // Fast reached NULL, no cycle
    return 0;
}

// Find the starting node of the cycle (if one exists)
//
// Algorithm (after detecting cycle):
//   1. Keep one pointer at meeting point
//   2. Move another pointer to head
//   3. Move both pointers one step at a time
//   4. They will meet at the cycle start
//
// Returns pointer to cycle start, or NULL if no cycle
struct SLLNode* find_cycle_start(struct SLL* list) {
    if (list == NULL || list->head == NULL) {
        return NULL;
    }

    struct SLLNode* slow = list->head;
    struct SLLNode* fast = list->head;

    // First, detect if there's a cycle
    int has_cycle = 0;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            has_cycle = 1;
            break;
        }
    }

    if (!has_cycle) {
        return NULL;
    }

    // Move slow to head, keep fast at meeting point
    slow = list->head;

    // Move both one step at a time until they meet at cycle start
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }

    return slow;
}

// Get the length of the cycle
// Returns 0 if no cycle exists
int get_cycle_length(struct SLL* list) {
    if (list == NULL || list->head == NULL) {
        return 0;
    }

    struct SLLNode* slow = list->head;
    struct SLLNode* fast = list->head;

    // First, detect if there's a cycle
    int has_cycle = 0;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            has_cycle = 1;
            break;
        }
    }

    if (!has_cycle) {
        return 0;
    }

    // Count nodes in the cycle
    int length = 1;
    struct SLLNode* current = slow->next;
    while (current != slow) {
        length++;
        current = current->next;
    }

    return length;
}

// Helper function to create a cycle for testing
// Creates a cycle by connecting the tail to the node at 'position'
// position = 0 means cycle back to head, position = -1 means no cycle
void create_cycle_at(struct SLL* list, int position) {
    if (list == NULL || list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    if (position < 0) {
        printf("No cycle created (position < 0)\n");
        return;
    }

    struct SLLNode* tail = list->head;
    struct SLLNode* cycle_node = NULL;

    // Find the position node and tail
    int count = 0;
    while (tail->next != NULL) {
        if (count == position) {
            cycle_node = tail;
        }
        tail = tail->next;
        count++;
    }

    // Check if position is the last node
    if (count == position) {
        cycle_node = tail;
    }

    // Create cycle: tail->next points to cycle_node
    if (cycle_node != NULL) {
        tail->next = cycle_node;
        printf("Cycle created: tail->next points to node at position %d (data=%d)\n",
               position, cycle_node->data);
    } else {
        printf("Position %d out of bounds (list has %d nodes)\n", position, count + 1);
    }
}

// Remove cycle from the list (if it exists)
void remove_cycle(struct SLL* list) {
    if (list == NULL || list->head == NULL) {
        return;
    }

    struct SLLNode* slow = list->head;
    struct SLLNode* fast = list->head;

    // Detect cycle
    int has_cycle = 0;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            has_cycle = 1;
            break;
        }
    }

    if (!has_cycle) {
        printf("No cycle to remove\n");
        return;
    }

    // Find cycle start
    slow = list->head;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }

    // Now slow and fast are at cycle start
    // Find the node before cycle start
    while (fast->next != slow) {
        fast = fast->next;
    }

    // Break the cycle
    fast->next = NULL;
    printf("Cycle removed\n");
}

// Print list with cycle detection (stops after detecting cycle to avoid infinite loop)
void print_with_cycle_detection(struct SLL* list) {
    if (list == NULL || list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    if (!detect_cycle(list)) {
        printf("List (no cycle): ");
        sll_print(list);
        return;
    }

    printf("List (has cycle): ");
    struct SLLNode* current = list->head;
    struct SLLNode* slow = list->head;
    struct SLLNode* fast = list->head;

    // Print until we detect the cycle
    int count = 0;
    int max_print = 20; // Limit output to avoid infinite printing

    while (count < max_print) {
        printf("%d -> ", current->data);
        current = current->next;
        count++;

        // Check if we've completed the cycle
        if (fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                printf("... (cycle detected)\n");
                return;
            }
        }
    }
    printf("... (printing stopped)\n");
}

// ========================================
// Recursive List Reversal
// ========================================

// Recursive helper function to reverse the list
// Returns the new head of the reversed list
struct SLLNode* reverse_recursive_helper(struct SLLNode* current, struct SLLNode* prev) {
    // Base case: reached end of list
    if (current == NULL) {
        return prev;
    }

    // Save next node before changing the link
    struct SLLNode* next = current->next;

    // Reverse the current node's pointer
    current->next = prev;

    // Recursively reverse the rest of the list
    return reverse_recursive_helper(next, current);
}

// Reverse the linked list recursively
//
// Algorithm:
//   1. Use a helper function with two parameters: current and previous
//   2. Base case: when current is NULL, return previous (new head)
//   3. Recursive case:
//      - Save next node
//      - Reverse current->next to point to previous
//      - Recursively call with next and current
//
// Time complexity: O(n) where n is the number of nodes
// Space complexity: O(n) due to recursion stack
void reverse_list_recursive(struct SLL* list) {
    if (list == NULL || list->head == NULL) {
        printf("List is empty or does not exist\n");
        return;
    }

    list->head = reverse_recursive_helper(list->head, NULL);
}

// Alternative: Single-function recursive reverse (less clear but more concise)
struct SLLNode* reverse_recursive_single(struct SLLNode* head) {
    // Base cases
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // Recursively reverse the rest of the list
    struct SLLNode* new_head = reverse_recursive_single(head->next);

    // Make the next node point back to current
    head->next->next = head;

    // Current node becomes the tail
    head->next = NULL;

    return new_head;
}

// Test function to verify the two-pointer technique
void test_search_from_end() {
    printf("=== Testing Search From End (Two-Pointer Technique) ===\n\n");

    struct SLL* list = sll_create();

    // Create a list: 10 -> 20 -> 30 -> 40 -> 50
    printf("Creating list: 10 -> 20 -> 30 -> 40 -> 50\n");
    for (int i = 1; i <= 5; i++) {
        sll_insert(list, i - 1, i * 10);
    }
    sll_print(list);

    printf("\nTesting various positions from end:\n");
    printf("----------------------------------\n");

    // Test different positions
    int test_positions[] = {1, 2, 3, 4, 5, 6};
    int num_tests = 6;

    for (int i = 0; i < num_tests; i++) {
        int n = test_positions[i];
        int result;
        printf("Search %dth from end: ", n);
        if (search_from_end_data(list, n, &result)) {
            printf("Found data = %d", result);

            // Verify by counting from end
            int length = sll_length(list);
            int expected_index = length - n;
            printf(" (index %d from start)\n", expected_index);
        } else {
            printf("Not found\n");
        }
    }

    printf("\n--- How the Two-Pointer Technique Works ---\n");
    printf("To find the 2nd node from end in: 10 -> 20 -> 30 -> 40 -> 50\n\n");
    printf("Step 1: Move 'fast' pointer 2 steps ahead\n");
    printf("  slow: 10,  fast: 30\n\n");
    printf("Step 2: Move both pointers until 'fast' reaches NULL\n");
    printf("  slow: 10,  fast: 30\n");
    printf("  slow: 20,  fast: 40\n");
    printf("  slow: 30,  fast: 50\n");
    printf("  slow: 40,  fast: NULL (stop)\n\n");
    printf("Result: slow is at 40, which is 2nd from end!\n");

    sll_destroy(list);
}

// Test function for Floyd's cycle detection algorithm
void test_cycle_detection() {
    printf("=== Testing Floyd's Cycle Detection Algorithm ===\n\n");

    struct SLL* list = sll_create();

    // Create a list: 10 -> 20 -> 30 -> 40 -> 50
    printf("Creating list: 10 -> 20 -> 30 -> 40 -> 50\n");
    for (int i = 1; i <= 5; i++) {
        sll_insert(list, i - 1, i * 10);
    }
    sll_print(list);

    printf("\nTest 1: Detect cycle in list without cycle\n");
    printf("Result: %s\n", detect_cycle(list) ? "Has cycle" : "No cycle");

    printf("\nTest 2: Create cycle at position 2 (node with data=30)\n");
    create_cycle_at(list, 2);
    print_with_cycle_detection(list);

    printf("\nTest 3: Detect cycle\n");
    printf("Result: %s\n", detect_cycle(list) ? "Has cycle" : "No cycle");

    printf("\nTest 4: Find cycle start\n");
    struct SLLNode* start = find_cycle_start(list);
    if (start != NULL) {
        printf("Cycle starts at node with data: %d\n", start->data);
    }

    printf("\nTest 5: Get cycle length\n");
    int length = get_cycle_length(list);
    printf("Cycle length: %d nodes\n", length);

    printf("\nTest 6: Remove cycle\n");
    remove_cycle(list);
    sll_print(list);

    printf("\nTest 7: Verify cycle is removed\n");
    printf("Result: %s\n", detect_cycle(list) ? "Has cycle" : "No cycle");

    printf("\n--- How Floyd's Algorithm Works ---\n");
    printf("List: 10 -> 20 -> 30 -> 40 -> 50 -> (back to 30)\n\n");
    printf("Step 1: Initialize slow and fast at head\n");
    printf("  slow: 10,  fast: 10\n\n");
    printf("Step 2: Move slow by 1, fast by 2\n");
    printf("  slow: 20,  fast: 30\n");
    printf("  slow: 30,  fast: 50\n");
    printf("  slow: 40,  fast: 40 (via 30)\n");
    printf("  slow: 50,  fast: 50 (via 40->30)\n");
    printf("  They meet! Cycle detected.\n\n");
    printf("To find cycle start:\n");
    printf("  Move one pointer to head, move both by 1 step\n");
    printf("  They meet at the cycle start (30)\n");

    sll_destroy(list);
}

// Test function for recursive list reversal
void test_reverse_recursive() {
    printf("=== Testing Recursive List Reversal ===\n\n");

    struct SLL* list = sll_create();

    // Create a list: 10 -> 20 -> 30 -> 40 -> 50
    printf("Creating list: 10 -> 20 -> 30 -> 40 -> 50\n");
    for (int i = 1; i <= 5; i++) {
        sll_insert(list, i - 1, i * 10);
    }
    printf("Original list: ");
    sll_print(list);

    printf("\nReversing list recursively...\n");
    reverse_list_recursive(list);
    printf("Reversed list: ");
    sll_print(list);

    printf("\nReversing again (should return to original)...\n");
    reverse_list_recursive(list);
    printf("List after second reverse: ");
    sll_print(list);

    printf("\n--- How Recursive Reversal Works ---\n");
    printf("Original: 10 -> 20 -> 30 -> 40 -> 50 -> NULL\n\n");
    printf("Recursion steps:\n");
    printf("1. reverse(10, NULL)  -> saves next=20, sets 10->NULL\n");
    printf("2. reverse(20, 10)    -> saves next=30, sets 20->10\n");
    printf("3. reverse(30, 20)    -> saves next=40, sets 30->20\n");
    printf("4. reverse(40, 30)    -> saves next=50, sets 40->30\n");
    printf("5. reverse(50, 40)    -> saves next=NULL, sets 50->40\n");
    printf("6. reverse(NULL, 50)  -> base case, return 50 (new head)\n\n");
    printf("Result: 50 -> 40 -> 30 -> 20 -> 10 -> NULL\n");

    // Test with single element
    printf("\nTest with single element:\n");
    sll_destroy(list);
    list = sll_create();
    sll_insert(list, 0, 100);
    printf("Original: ");
    sll_print(list);
    reverse_list_recursive(list);
    printf("Reversed: ");
    sll_print(list);

    // Test with empty list
    printf("\nTest with empty list:\n");
    sll_destroy(list);
    list = sll_create();
    printf("Original: ");
    sll_print(list);
    reverse_list_recursive(list);

    sll_destroy(list);
}

int main() {
    char choice;

    while (1) {
        printf("\n=== List Search Main Menu ===\n");
        printf("1. Search from end (two-pointer)\n");
        printf("2. Cycle detection (Floyd's algorithm)\n");
        printf("3. Reverse list (recursive)\n");
        printf("x. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            test_search_from_end();
        } else if (choice == '2') {
            test_cycle_detection();
        } else if (choice == '3') {
            test_reverse_recursive();
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
