/*
 * Stacks and Queues Header
 *
 * Provides stack and queue data structures implemented using linked lists.
 * Used for graph/tree traversal algorithms (DFS, BFS).
 */

#ifndef STACKS_QUEUES_H
#define STACKS_QUEUES_H

#include <stdbool.h>
#include "2_linked_lists.h"

// ============================================================
// LINKED LIST-BASED STACK
// ============================================================

typedef struct {
    struct SLL* list;  // Using SLL from 2_linked_lists.h
} ListStack;

// Stack operations
ListStack* list_stack_create();
void list_stack_destroy(ListStack* stack);
bool list_stack_is_empty(ListStack* stack);
void list_stack_push(ListStack* stack, long value);
long list_stack_pop(ListStack* stack);
long list_stack_peek(ListStack* stack);
int list_stack_size(ListStack* stack);
void list_stack_print(ListStack* stack);

// ============================================================
// LINKED LIST-BASED QUEUE
// ============================================================

typedef struct {
    struct SLLNode* front;  // Points to first element (for dequeue)
    struct SLLNode* rear;   // Points to last element (for enqueue)
} ListQueue;

// Queue operations
ListQueue* list_queue_create();
void list_queue_destroy(ListQueue* queue);
bool list_queue_is_empty(ListQueue* queue);
void list_queue_enqueue(ListQueue* queue, long value);
long list_queue_dequeue(ListQueue* queue);
long list_queue_peek(ListQueue* queue);
int list_queue_size(ListQueue* queue);
void list_queue_print(ListQueue* queue);

#endif // STACKS_QUEUES_H
