#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "2_linked_lists.h"
#include "stacks_queues.h"

// ============================================================
// 1. STACK USING DYNAMIC ARRAY
// ============================================================

typedef struct {
    int* data;       // Dynamic array to store elements
    int top;         // Index of top element (-1 if empty)
    int capacity;    // Current capacity of the array
} ArrayStack;

// Create a new array-based stack with initial capacity
ArrayStack* array_stack_create(int initial_capacity) {
    ArrayStack* stack = (ArrayStack*)malloc(sizeof(ArrayStack));
    if (!stack) {
        printf("Memory allocation error\n");
        exit(1);
    }
    stack->data = (int*)malloc(initial_capacity * sizeof(int));
    if (!stack->data) {
        printf("Memory allocation error\n");
        free(stack);
        exit(1);
    }
    stack->top = -1;
    stack->capacity = initial_capacity;
    return stack;
}

// Check if array stack is empty
bool array_stack_is_empty(ArrayStack* stack) {
    return stack->top == -1;
}

// Check if array stack is full
bool array_stack_is_full(ArrayStack* stack) {
    return stack->top == stack->capacity - 1;
}

// Resize the array stack (double the capacity)
// Time Complexity: O(n) - need to copy all elements
void array_stack_resize(ArrayStack* stack) {
    int new_capacity = stack->capacity * 2;
    int* new_data = (int*)realloc(stack->data, new_capacity * sizeof(int));
    if (!new_data) {
        printf("Memory allocation error during resize\n");
        exit(1);
    }
    stack->data = new_data;
    stack->capacity = new_capacity;
    printf("Stack resized from %d to %d\n", stack->capacity / 2, stack->capacity);
}

// Push element onto array stack
// Time Complexity: O(1) amortized (O(n) when resize needed)
// Space Complexity: O(1)
void array_stack_push(ArrayStack* stack, int value) {
    if (array_stack_is_full(stack)) {
        array_stack_resize(stack);
    }
    stack->data[++stack->top] = value;
}

// Pop element from array stack
// Time Complexity: O(1)
// Space Complexity: O(1)
int array_stack_pop(ArrayStack* stack) {
    if (array_stack_is_empty(stack)) {
        printf("Stack underflow! Cannot pop from empty stack.\n");
        return -1;
    }
    return stack->data[stack->top--];
}

// Peek at top element without removing it
// Time Complexity: O(1)
int array_stack_peek(ArrayStack* stack) {
    if (array_stack_is_empty(stack)) {
        printf("Stack is empty!\n");
        return -1;
    }
    return stack->data[stack->top];
}

// Get current size of stack
int array_stack_size(ArrayStack* stack) {
    return stack->top + 1;
}

// Print array stack
void array_stack_print(ArrayStack* stack) {
    if (array_stack_is_empty(stack)) {
        printf("Stack is empty\n");
        return;
    }
    printf("Stack (top to bottom): ");
    for (int i = stack->top; i >= 0; i--) {
        printf("%d ", stack->data[i]);
    }
    printf("\n");
}

// Destroy array stack
void array_stack_destroy(ArrayStack* stack) {
    if (stack) {
        free(stack->data);
        free(stack);
    }
}

// ============================================================
// 2. STACK USING LINKED LIST
// ============================================================

// Create a new linked list-based stack
ListStack* list_stack_create() {
    ListStack* stack = (ListStack*)malloc(sizeof(ListStack));
    if (!stack) {
        printf("Memory allocation error\n");
        exit(1);
    }
    stack->list = sll_create();
    return stack;
}

// Check if list stack is empty
bool list_stack_is_empty(ListStack* stack) {
    return stack->list->head == NULL;
}

// Push element onto list stack
// Time Complexity: O(1) - insert at head
// Space Complexity: O(1)
void list_stack_push(ListStack* stack, long value) {
    sll_insert(stack->list, 0, value);  // Insert at head (index 0)
}

// Pop element from list stack
// Time Complexity: O(1) - delete from head
// Space Complexity: O(1)
long list_stack_pop(ListStack* stack) {
    if (list_stack_is_empty(stack)) {
        printf("Stack underflow! Cannot pop from empty stack.\n");
        return -1;
    }
    long value = stack->list->head->data;
    sll_delete(stack->list, 0);  // Delete from head
    return value;
}

// Peek at top element without removing it
// Time Complexity: O(1)
long list_stack_peek(ListStack* stack) {
    if (list_stack_is_empty(stack)) {
        printf("Stack is empty!\n");
        return -1;
    }
    return stack->list->head->data;
}

// Get current size of stack
// Time Complexity: O(n)
int list_stack_size(ListStack* stack) {
    return sll_length(stack->list);
}

// Print list stack
void list_stack_print(ListStack* stack) {
    if (list_stack_is_empty(stack)) {
        printf("Stack is empty\n");
        return;
    }
    printf("Stack (top to bottom): ");
    struct SLLNode* current = stack->list->head;
    while (current != NULL) {
        printf("%ld ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Destroy list stack
void list_stack_destroy(ListStack* stack) {
    if (stack) {
        sll_destroy(stack->list);
        free(stack);
    }
}

// ============================================================
// 3. QUEUE USING LINKED LIST
// ============================================================

// Create a new linked list-based queue
ListQueue* list_queue_create() {
    ListQueue* queue = (ListQueue*)malloc(sizeof(ListQueue));
    if (!queue) {
        printf("Memory allocation error\n");
        exit(1);
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

// Check if list queue is empty
bool list_queue_is_empty(ListQueue* queue) {
    return queue->front == NULL;
}

// Enqueue element to the queue
// Time Complexity: O(1) - add at rear
// Space Complexity: O(1)
void list_queue_enqueue(ListQueue* queue, long value) {
    struct SLLNode* newNode = sll_createNode(value);

    // If queue is empty, both front and rear point to new node
    if (list_queue_is_empty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        // Add new node at rear and update rear pointer
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Dequeue element from the queue
// Time Complexity: O(1) - remove from front
// Space Complexity: O(1)
long list_queue_dequeue(ListQueue* queue) {
    if (list_queue_is_empty(queue)) {
        printf("Queue underflow! Cannot dequeue from empty queue.\n");
        return -1;
    }

    struct SLLNode* temp = queue->front;
    long value = temp->data;
    queue->front = queue->front->next;

    // If queue becomes empty, update rear to NULL
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return value;
}

// Peek at front element without removing it
// Time Complexity: O(1)
long list_queue_peek(ListQueue* queue) {
    if (list_queue_is_empty(queue)) {
        printf("Queue is empty!\n");
        return -1;
    }
    return queue->front->data;
}

// Get current size of queue
// Time Complexity: O(n) - must traverse entire queue
int list_queue_size(ListQueue* queue) {
    int count = 0;
    struct SLLNode* current = queue->front;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// Print list queue
void list_queue_print(ListQueue* queue) {
    if (list_queue_is_empty(queue)) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue (front to rear): ");
    struct SLLNode* current = queue->front;
    while (current != NULL) {
        printf("%ld ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Destroy list queue
void list_queue_destroy(ListQueue* queue) {
    if (queue) {
        // Free all nodes
        struct SLLNode* current = queue->front;
        while (current != NULL) {
            struct SLLNode* next = current->next;
            free(current);
            current = next;
        }
        free(queue);
    }
}

// ============================================================
// 4. QUEUE USING TWO STACKS
// ============================================================

typedef struct {
    ListStack* stack1;  // For enqueue operations
    ListStack* stack2;  // For dequeue operations
} StackQueue;

// Create a new queue using two stacks
StackQueue* stack_queue_create() {
    StackQueue* queue = (StackQueue*)malloc(sizeof(StackQueue));
    if (!queue) {
        printf("Memory allocation error\n");
        exit(1);
    }
    queue->stack1 = list_stack_create();
    queue->stack2 = list_stack_create();
    return queue;
}

// Check if queue is empty
bool stack_queue_is_empty(StackQueue* queue) {
    return list_stack_is_empty(queue->stack1) && list_stack_is_empty(queue->stack2);
}

// Enqueue element to the queue
// Time Complexity: O(1)
void stack_queue_enqueue(StackQueue* queue, int value) {
    list_stack_push(queue->stack1, value);
}

// Dequeue element from the queue
// Time Complexity: O(1) amortized, O(n) worst case
// Mechanism: If stack2 is empty, transfer all elements from stack1 to stack2
// This reverses the order, making the oldest element accessible at top of stack2
int stack_queue_dequeue(StackQueue* queue) {
    if (stack_queue_is_empty(queue)) {
        printf("Queue is empty!\n");
        return -1;
    }

    // If stack2 is empty, transfer all elements from stack1
    if (list_stack_is_empty(queue->stack2)) {
        while (!list_stack_is_empty(queue->stack1)) {
            int value = list_stack_pop(queue->stack1);
            list_stack_push(queue->stack2, value);
        }
    }

    return list_stack_pop(queue->stack2);
}

// Peek at front element
int stack_queue_peek(StackQueue* queue) {
    if (stack_queue_is_empty(queue)) {
        printf("Queue is empty!\n");
        return -1;
    }

    if (list_stack_is_empty(queue->stack2)) {
        while (!list_stack_is_empty(queue->stack1)) {
            int value = list_stack_pop(queue->stack1);
            list_stack_push(queue->stack2, value);
        }
    }

    return list_stack_peek(queue->stack2);
}

// Destroy queue
void stack_queue_destroy(StackQueue* queue) {
    if (queue) {
        list_stack_destroy(queue->stack1);
        list_stack_destroy(queue->stack2);
        free(queue);
    }
}

// ============================================================
// 5. STACK USING TWO QUEUES
// ============================================================

typedef struct {
    StackQueue* queue1;
    StackQueue* queue2;
} QueueStack;

// Create a new stack using two queues
QueueStack* queue_stack_create() {
    QueueStack* stack = (QueueStack*)malloc(sizeof(QueueStack));
    if (!stack) {
        printf("Memory allocation error\n");
        exit(1);
    }
    stack->queue1 = stack_queue_create();
    stack->queue2 = stack_queue_create();
    return stack;
}

// Check if stack is empty
bool queue_stack_is_empty(QueueStack* stack) {
    return stack_queue_is_empty(stack->queue1);
}

// Push element onto stack
// Time Complexity: O(n) - need to transfer all elements
// Mechanism: Push to queue2, transfer all from queue1 to queue2, swap queues
void queue_stack_push(QueueStack* stack, int value) {
    // Enqueue new element to queue2
    stack_queue_enqueue(stack->queue2, value);

    // Transfer all elements from queue1 to queue2
    while (!stack_queue_is_empty(stack->queue1)) {
        int val = stack_queue_dequeue(stack->queue1);
        stack_queue_enqueue(stack->queue2, val);
    }

    // Swap queue1 and queue2
    StackQueue* temp = stack->queue1;
    stack->queue1 = stack->queue2;
    stack->queue2 = temp;
}

// Pop element from stack
// Time Complexity: O(1)
int queue_stack_pop(QueueStack* stack) {
    if (queue_stack_is_empty(stack)) {
        printf("Stack is empty!\n");
        return -1;
    }
    return stack_queue_dequeue(stack->queue1);
}

// Peek at top element
int queue_stack_peek(QueueStack* stack) {
    if (queue_stack_is_empty(stack)) {
        printf("Stack is empty!\n");
        return -1;
    }
    return stack_queue_peek(stack->queue1);
}

// Destroy stack
void queue_stack_destroy(QueueStack* stack) {
    if (stack) {
        stack_queue_destroy(stack->queue1);
        stack_queue_destroy(stack->queue2);
        free(stack);
    }
}

// ============================================================
// STACK ALGORITHM FUNCTIONS
// ============================================================

// 1. Check if symbols are balanced
// Supports: (), [], {}
// Time Complexity: O(n) where n is length of expression
// Space Complexity: O(n) for stack
bool check_balanced_symbols(const char* expression) {
    ListStack* stack = list_stack_create();

    for (int i = 0; expression[i] != '\0'; i++) {
        char ch = expression[i];

        // Push opening symbols
        if (ch == '(' || ch == '[' || ch == '{') {
            list_stack_push(stack, ch);
        }
        // Check closing symbols
        else if (ch == ')' || ch == ']' || ch == '}') {
            if (list_stack_is_empty(stack)) {
                list_stack_destroy(stack);
                return false;  // No matching opening symbol
            }

            char top = list_stack_pop(stack);

            // Check if opening and closing symbols match
            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{')) {
                list_stack_destroy(stack);
                return false;  // Mismatched symbols
            }
        }
    }

    // Stack should be empty if balanced
    bool is_balanced = list_stack_is_empty(stack);
    list_stack_destroy(stack);
    return is_balanced;
}

// 2. Infix to Postfix Conversion
// Converts expressions like "A+B*C" to "ABC*+"
// Time Complexity: O(n)
// Space Complexity: O(n)

// Helper: Get precedence of operator
int get_precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Helper: Check if character is operator
bool is_operator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

// Convert infix to postfix
// Result is stored in 'postfix' parameter
void infix_to_postfix(const char* infix, char* postfix) {
    ListStack* stack = list_stack_create();
    int j = 0;  // Index for postfix string

    for (int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];

        // Skip whitespace
        if (ch == ' ') continue;

        // If operand (letter or digit), add to output
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) {
            postfix[j++] = ch;
        }
        // If '(', push to stack
        else if (ch == '(') {
            list_stack_push(stack, ch);
        }
        // If ')', pop until '('
        else if (ch == ')') {
            while (!list_stack_is_empty(stack) && list_stack_peek(stack) != '(') {
                postfix[j++] = list_stack_pop(stack);
            }
            if (!list_stack_is_empty(stack)) {
                list_stack_pop(stack);  // Remove '('
            }
        }
        // If operator
        else if (is_operator(ch)) {
            // Pop operators with higher or equal precedence
            while (!list_stack_is_empty(stack) &&
                   list_stack_peek(stack) != '(' &&
                   get_precedence(list_stack_peek(stack)) >= get_precedence(ch)) {
                postfix[j++] = list_stack_pop(stack);
            }
            list_stack_push(stack, ch);
        }
    }

    // Pop remaining operators
    while (!list_stack_is_empty(stack)) {
        postfix[j++] = list_stack_pop(stack);
    }

    postfix[j] = '\0';  // Null-terminate string
    list_stack_destroy(stack);
}

// 3. Recursive Reverse Stack
// Uses only stack operations (push, pop, isEmpty, peek)
// Time Complexity: O(n^2)
// Space Complexity: O(n) for recursion stack

// Helper: Insert element at bottom of stack
void insert_at_bottom(ListStack* stack, int value) {
    if (list_stack_is_empty(stack)) {
        list_stack_push(stack, value);
        return;
    }

    // Hold all items in call stack until we reach bottom
    int top = list_stack_pop(stack);
    insert_at_bottom(stack, value);

    // Put back the held items
    list_stack_push(stack, top);
}

// Reverse stack recursively
void reverse_stack_recursive(ListStack* stack) {
    if (list_stack_is_empty(stack)) {
        return;
    }

    // Hold top element
    int top = list_stack_pop(stack);

    // Reverse remaining stack
    reverse_stack_recursive(stack);

    // Insert held element at bottom
    insert_at_bottom(stack, top);
}

// ============================================================
// TEST FUNCTIONS
// ============================================================

void test_array_stack() {
    printf("\n=== Testing Array-based Stack ===\n");
    ArrayStack* stack = array_stack_create(2);  // Small initial capacity to test resize

    printf("\nPushing elements: 10, 20, 30, 40, 50\n");
    array_stack_push(stack, 10);
    array_stack_push(stack, 20);
    array_stack_push(stack, 30);
    array_stack_push(stack, 40);
    array_stack_push(stack, 50);

    array_stack_print(stack);
    printf("Size: %d, Capacity: %d\n", array_stack_size(stack), stack->capacity);

    printf("\nPeek: %d\n", array_stack_peek(stack));

    printf("\nPopping 2 elements:\n");
    printf("Popped: %d\n", array_stack_pop(stack));
    printf("Popped: %d\n", array_stack_pop(stack));

    array_stack_print(stack);
    printf("Size: %d\n", array_stack_size(stack));

    printf("\nPushing 60 and 70\n");
    array_stack_push(stack, 60);
    array_stack_push(stack, 70);
    array_stack_print(stack);

    array_stack_destroy(stack);
    printf("\nArray stack destroyed\n");
}

void test_list_stack() {
    printf("\n=== Testing Linked List-based Stack ===\n");
    ListStack* stack = list_stack_create();

    printf("\nPushing elements: 10, 20, 30, 40, 50\n");
    list_stack_push(stack, 10);
    list_stack_push(stack, 20);
    list_stack_push(stack, 30);
    list_stack_push(stack, 40);
    list_stack_push(stack, 50);

    list_stack_print(stack);
    printf("Size: %d\n", list_stack_size(stack));

    printf("\nPeek: %ld\n", list_stack_peek(stack));

    printf("\nPopping 2 elements:\n");
    printf("Popped: %ld\n", list_stack_pop(stack));
    printf("Popped: %ld\n", list_stack_pop(stack));

    list_stack_print(stack);
    printf("Size: %d\n", list_stack_size(stack));

    printf("\nPushing 60 and 70\n");
    list_stack_push(stack, 60);
    list_stack_push(stack, 70);
    list_stack_print(stack);

    list_stack_destroy(stack);
    printf("\nList stack destroyed\n");
}

void test_list_queue() {
    printf("\n=== Testing Linked List-based Queue ===\n");
    ListQueue* queue = list_queue_create();

    printf("\nEnqueuing elements: 10, 20, 30, 40, 50\n");
    list_queue_enqueue(queue, 10);
    list_queue_enqueue(queue, 20);
    list_queue_enqueue(queue, 30);
    list_queue_enqueue(queue, 40);
    list_queue_enqueue(queue, 50);

    list_queue_print(queue);
    printf("Size: %d\n", list_queue_size(queue));

    printf("\nPeek front: %ld\n", list_queue_peek(queue));

    printf("\nDequeuing 2 elements:\n");
    printf("Dequeued: %ld\n", list_queue_dequeue(queue));
    printf("Dequeued: %ld\n", list_queue_dequeue(queue));

    list_queue_print(queue);
    printf("Size: %d\n", list_queue_size(queue));

    printf("\nEnqueuing 60 and 70\n");
    list_queue_enqueue(queue, 60);
    list_queue_enqueue(queue, 70);
    list_queue_print(queue);

    printf("\nDequeuing all remaining elements:\n");
    while (!list_queue_is_empty(queue)) {
        printf("Dequeued: %ld\n", list_queue_dequeue(queue));
    }

    list_queue_destroy(queue);
    printf("\nList queue destroyed\n");
}

void test_balanced_symbols() {
    printf("\n=== Testing Balanced Symbols ===\n");

    const char* test_cases[] = {
        "((A+B)+[C-D])",
        "{[()]}",
        "((A+B)+[C-D]}",
        "{[(])}",
        "((A+B)",
        "(A+B))",
        "",
        "()",
        "A+B"
    };
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_cases; i++) {
        bool balanced = check_balanced_symbols(test_cases[i]);
        printf("Expression: %-20s => %s\n", test_cases[i],
               balanced ? "BALANCED" : "NOT BALANCED");
    }
}

void test_stack_queue() {
    printf("\n=== Testing Queue using Two Stacks ===\n");
    StackQueue* queue = stack_queue_create();

    printf("\nEnqueuing: 10, 20, 30, 40, 50\n");
    stack_queue_enqueue(queue, 10);
    stack_queue_enqueue(queue, 20);
    stack_queue_enqueue(queue, 30);
    stack_queue_enqueue(queue, 40);
    stack_queue_enqueue(queue, 50);

    printf("\nPeek front: %d\n", stack_queue_peek(queue));

    printf("\nDequeuing 3 elements:\n");
    printf("Dequeued: %d\n", stack_queue_dequeue(queue));
    printf("Dequeued: %d\n", stack_queue_dequeue(queue));
    printf("Dequeued: %d\n", stack_queue_dequeue(queue));

    printf("\nEnqueuing: 60, 70\n");
    stack_queue_enqueue(queue, 60);
    stack_queue_enqueue(queue, 70);

    printf("\nDequeuing remaining elements:\n");
    while (!stack_queue_is_empty(queue)) {
        printf("Dequeued: %d\n", stack_queue_dequeue(queue));
    }

    stack_queue_destroy(queue);
    printf("\nQueue destroyed\n");
}

void test_queue_stack() {
    printf("\n=== Testing Stack using Two Queues ===\n");
    QueueStack* stack = queue_stack_create();

    printf("\nPushing: 10, 20, 30, 40, 50\n");
    queue_stack_push(stack, 10);
    queue_stack_push(stack, 20);
    queue_stack_push(stack, 30);
    queue_stack_push(stack, 40);
    queue_stack_push(stack, 50);

    printf("\nPeek top: %d\n", queue_stack_peek(stack));

    printf("\nPopping 2 elements:\n");
    printf("Popped: %d\n", queue_stack_pop(stack));
    printf("Popped: %d\n", queue_stack_pop(stack));

    printf("\nPushing: 60, 70\n");
    queue_stack_push(stack, 60);
    queue_stack_push(stack, 70);

    printf("\nPopping remaining elements:\n");
    while (!queue_stack_is_empty(stack)) {
        printf("Popped: %d\n", queue_stack_pop(stack));
    }

    queue_stack_destroy(stack);
    printf("\nStack destroyed\n");
}

void test_infix_to_postfix() {
    printf("\n=== Testing Infix to Postfix Conversion ===\n");

    const char* test_cases[] = {
        "A+B",
        "A+B*C",
        "(A+B)*C",
        "A+B*C-D",
        "A*(B+C)/D",
        "((A+B)+[C-D])",
        "A+B-C*D/E",
        "(A+B)*(C-D)"
    };
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    char postfix[100];
    for (int i = 0; i < num_cases; i++) {
        infix_to_postfix(test_cases[i], postfix);
        printf("Infix:    %-20s => Postfix: %s\n", test_cases[i], postfix);
    }
}

void test_reverse_stack() {
    printf("\n=== Testing Recursive Stack Reversal ===\n");
    ListStack* stack = list_stack_create();

    printf("\nOriginal stack (pushing 10, 20, 30, 40, 50):\n");
    list_stack_push(stack, 10);
    list_stack_push(stack, 20);
    list_stack_push(stack, 30);
    list_stack_push(stack, 40);
    list_stack_push(stack, 50);
    list_stack_print(stack);

    printf("\nReversing stack recursively...\n");
    reverse_stack_recursive(stack);

    printf("\nReversed stack:\n");
    list_stack_print(stack);

    list_stack_destroy(stack);
    printf("\nStack destroyed\n");
}

// ============================================================
// MAIN
// ============================================================

#ifndef SKIP_MAIN
int main() {
    char choice;

    while (1) {
        printf("\n=== Stacks and Queues Menu ===\n");
        printf("1. Test Array-based Stack\n");
        printf("2. Test Linked List-based Stack\n");
        printf("3. Test Linked List-based Queue\n");
        printf("4. Test Balanced Symbols\n");
        printf("5. Test Queue using Two Stacks\n");
        printf("6. Test Stack using Two Queues\n");
        printf("7. Test Infix to Postfix Conversion\n");
        printf("8. Test Recursive Stack Reversal\n");
        printf("x. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            test_array_stack();
        } else if (choice == '2') {
            test_list_stack();
        } else if (choice == '3') {
            test_list_queue();
        } else if (choice == '4') {
            test_balanced_symbols();
        } else if (choice == '5') {
            test_stack_queue();
        } else if (choice == '6') {
            test_queue_stack();
        } else if (choice == '7') {
            test_infix_to_postfix();
        } else if (choice == '8') {
            test_reverse_stack();
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
#endif  // SKIP_MAIN
