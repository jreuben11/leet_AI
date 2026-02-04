#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

// --- Singly Linked List (SLL) ---

struct SLLNode {
    long data;
    struct SLLNode* next;
};

struct SLL {
    struct SLLNode* head;
};

// SLL function prototypes
struct SLL* sll_create();
void sll_destroy(struct SLL* list);
struct SLLNode* sll_createNode(long data);
void sll_insert(struct SLL* list, int index, long new_data);
int sll_length(struct SLL* list);
void sll_delete(struct SLL* list, int position);
void sll_print(struct SLL* list);

#endif
