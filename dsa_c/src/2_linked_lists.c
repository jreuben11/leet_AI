#include <stdio.h>
#include <stdlib.h>
#include "2_linked_lists.h"

// --- Singly Linked List (SLL) ---

struct SLL* sll_create() {
    struct SLL* list = (struct SLL*)malloc(sizeof(struct SLL));
    if (!list) {
        printf("Memory allocation error\n");
        exit(1);
    }
    list->head = NULL;
    return list;
}

void sll_destroy(struct SLL* list) {
    if (list == NULL) return;
    struct SLLNode* current = list->head;
    struct SLLNode* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

struct SLLNode* sll_createNode(long data) {
    struct SLLNode* newNode = (struct SLLNode*)malloc(sizeof(struct SLLNode));
    if (!newNode) {
        printf("Memory allocation error\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void sll_insert(struct SLL* list, int index, long new_data) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }

    if (index < 0) {
        printf("Invalid index\n");
        return;
    }

    if (index == 0) {
        struct SLLNode* new_node = sll_createNode(new_data);
        new_node->next = list->head;
        list->head = new_node;
        return;
    }

    struct SLLNode* temp = list->head;
    for (int i = 0; i < index - 1; i++) {
        if (temp == NULL) {
            printf("Index out of bounds\n");
            return;
        }
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Index out of bounds\n");
        return;
    }

    struct SLLNode* new_node = sll_createNode(new_data);
    new_node->next = temp->next;
    temp->next = new_node;
}

int sll_length(struct SLL* list) {
    if (list == NULL) return 0;
    int count = 0;
    struct SLLNode* head = list->head;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

void sll_delete(struct SLL* list, int position) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    if (list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    struct SLLNode* temp = list->head;

    if (position == 0) {
        list->head = temp->next;
        free(temp);
        return;
    }

    for (int i = 0; temp != NULL && i < position - 1; i++) {
        temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL) {
        printf("Position out of bounds\n");
        return;
    }

    struct SLLNode* next = temp->next->next;
    free(temp->next);
    temp->next = next;
}

void sll_print(struct SLL* list) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    struct SLLNode* node = list->head;
    while (node != NULL) {
        printf("%ld -> ", node->data);
        node = node->next;
    }
    printf("NULL\n");
}

// SLL helper for merge sort - split list into two halves
void sll_split(struct SLLNode* source, struct SLLNode** front, struct SLLNode** back) {
    struct SLLNode* fast;
    struct SLLNode* slow;
    slow = source;
    fast = source->next;
    
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    
    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

// SLL helper for merge sort - merge two sorted lists
struct SLLNode* sll_merge(struct SLLNode* a, struct SLLNode* b) {
    struct SLLNode* result = NULL;
    
    if (a == NULL) return b;
    if (b == NULL) return a;
    
    if (a->data <= b->data) {
        result = a;
        result->next = sll_merge(a->next, b);
    } else {
        result = b;
        result->next = sll_merge(a, b->next);
    }
    return result;
}

// SLL helper for merge sort - recursive sort
void sll_merge_sort(struct SLLNode** headRef) {
    struct SLLNode* head = *headRef;
    struct SLLNode* a;
    struct SLLNode* b;
    
    if (head == NULL || head->next == NULL) {
        return;
    }
    
    sll_split(head, &a, &b);
    sll_merge_sort(&a);
    sll_merge_sort(&b);
    *headRef = sll_merge(a, b);
}

void sll_sort(struct SLL* list) {
    if (list == NULL || list->head == NULL) {
        return;
    }
    sll_merge_sort(&list->head);
}

int sll_find(struct SLL* list, int value) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }
    
    struct SLLNode* current = list->head;
    int index = 0;
    
    while (current != NULL) {
        if (current->data == value) {
            return index;
        }
        current = current->next;
        index++;
    }
    
    return -1; // Not found
}

void sll_insert_sorted(struct SLL* list, long value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    
    struct SLLNode* new_node = sll_createNode(value);
    
    // If list is empty or value is smaller than head
    if (list->head == NULL || list->head->data >= value) {
        new_node->next = list->head;
        list->head = new_node;
        return;
    }
    
    // Find the position to insert
    struct SLLNode* current = list->head;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }
    
    new_node->next = current->next;
    current->next = new_node;
}

void sll_append_to_head(struct SLL* list, int value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    
    struct SLLNode* new_node = sll_createNode(value);
    new_node->next = list->head;
    list->head = new_node;
}

void sll_append_to_tail(struct SLL* list, int value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    
    struct SLLNode* new_node = sll_createNode(value);
    
    if (list->head == NULL) {
        list->head = new_node;
        return;
    }
    
    struct SLLNode* current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

// --- Main Menu Helper ---

void sll_menu() {
    struct SLL* list = sll_create();
    char choice;
    int val, index;

    while (1) {
        printf("\n--- Singly Linked List Menu ---\n");
        printf("1. Insert at Position\n");
        printf("2. Delete at Position\n");
        printf("3. Get Length\n");
        printf("4. Display\n");
        printf("5. Sort\n");
        printf("6. Find\n");
        printf("7. Insert Sorted\n");
        printf("8. Append to Head\n");
        printf("9. Append to Tail\n");
        printf("b. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'b') {
            break;
        } else if (choice == '1') {
            printf("Enter index: ");
            scanf("%d", &index);
            printf("Enter value to insert: ");
            scanf("%d", &val);
            sll_insert(list, index, val);
            sll_print(list);
        } else if (choice == '2') {
            printf("Enter index to delete: ");
            scanf("%d", &index);
            sll_delete(list, index);
            sll_print(list);
        } else if (choice == '3') {
            printf("Length: %d\n", sll_length(list));
        } else if (choice == '4') {
            printf("SLL: ");
            sll_print(list);
        } else if (choice == '5') {
            sll_sort(list);
            printf("List sorted.\n");
            sll_print(list);
        } else if (choice == '6') {
            printf("Enter value to find: ");
            scanf("%d", &val);
            int pos = sll_find(list, val);
            if (pos != -1) {
                printf("Value %d found at index %d\n", val, pos);
            } else {
                printf("Value %d not found\n", val);
            }
        } else if (choice == '7') {
            printf("Enter value to insert sorted: ");
            scanf("%d", &val);
            sll_insert_sorted(list, val);
            sll_print(list);
        } else if (choice == '8') {
            printf("Enter value to append to head: ");
            scanf("%d", &val);
            sll_append_to_head(list, val);
            sll_print(list);
        } else if (choice == '9') {
            printf("Enter value to append to tail: ");
            scanf("%d", &val);
            sll_append_to_tail(list, val);
            sll_print(list);
        }
    }
    sll_destroy(list);
}


// --- Doubly Linked List (DLL) ---

struct DLLNode {
    int data;
    struct DLLNode* next;
    struct DLLNode* prev;
};

struct DLL {
    struct DLLNode* head;
};

struct DLL* dll_create() {
    struct DLL* list = (struct DLL*)malloc(sizeof(struct DLL));
    if (!list) {
        printf("Memory allocation error\n");
        exit(1);
    }
    list->head = NULL;
    return list;
}

void dll_destroy(struct DLL* list) {
    if (list == NULL) return;
    struct DLLNode* current = list->head;
    struct DLLNode* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

struct DLLNode* dll_createNode(int data) {
    struct DLLNode* newNode = (struct DLLNode*)malloc(sizeof(struct DLLNode));
    if (!newNode) {
        printf("Memory allocation error\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void dll_insert(struct DLL* list, int index, int new_data) {
    if (list == NULL) return;
    if (index < 0) {
        printf("Invalid index\n");
        return;
    }

    struct DLLNode* new_node = dll_createNode(new_data);

    if (index == 0) {
        new_node->next = list->head;
        if (list->head != NULL) {
            list->head->prev = new_node;
        }
        list->head = new_node;
        return;
    }

    struct DLLNode* temp = list->head;
    for (int i = 0; i < index - 1; i++) {
        if (temp == NULL) {
            printf("Index out of bounds\n");
            free(new_node);
            return;
        }
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Index out of bounds\n");
        free(new_node);
        return;
    }

    new_node->next = temp->next;
    if (temp->next != NULL) {
        temp->next->prev = new_node;
    }
    temp->next = new_node;
    new_node->prev = temp;
}

int dll_length(struct DLL* list) {
    if (list == NULL) return 0;
    int count = 0;
    struct DLLNode* head = list->head;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

void dll_delete(struct DLL* list, int position) {
    if (list == NULL || list->head == NULL) {
        printf("List is empty or invalid\n");
        return;
    }

    struct DLLNode* temp = list->head;

    if (position == 0) {
        list->head = temp->next;
        if (list->head != NULL) {
            list->head->prev = NULL;
        }
        free(temp);
        return;
    }

    for (int i = 0; temp != NULL && i < position; i++) {
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Position out of bounds\n");
        return;
    }

    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    }
    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    }
    free(temp);
}

void dll_print(struct DLL* list) {
    if (list == NULL) return;
    struct DLLNode* node = list->head;
    printf("DLL: ");
    while (node != NULL) {
        printf("%d <-> ", node->data);
        node = node->next;
    }
    printf("NULL\n");
}

// DLL helper for merge sort - split list into two halves
void dll_split(struct DLLNode* source, struct DLLNode** front, struct DLLNode** back) {
    struct DLLNode* fast;
    struct DLLNode* slow;
    slow = source;
    fast = source->next;
    
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    
    *front = source;
    *back = slow->next;
    if (*back != NULL) {
        (*back)->prev = NULL;
    }
    slow->next = NULL;
}

// DLL helper for merge sort - merge two sorted lists
struct DLLNode* dll_merge(struct DLLNode* a, struct DLLNode* b) {
    if (a == NULL) return b;
    if (b == NULL) return a;
    
    if (a->data <= b->data) {
        a->next = dll_merge(a->next, b);
        if (a->next != NULL) {
            a->next->prev = a;
        }
        a->prev = NULL;
        return a;
    } else {
        b->next = dll_merge(a, b->next);
        if (b->next != NULL) {
            b->next->prev = b;
        }
        b->prev = NULL;
        return b;
    }
}

// DLL helper for merge sort - recursive sort
void dll_merge_sort(struct DLLNode** headRef) {
    struct DLLNode* head = *headRef;
    struct DLLNode* a;
    struct DLLNode* b;
    
    if (head == NULL || head->next == NULL) {
        return;
    }
    
    dll_split(head, &a, &b);
    dll_merge_sort(&a);
    dll_merge_sort(&b);
    *headRef = dll_merge(a, b);
}

void dll_sort(struct DLL* list) {
    if (list == NULL || list->head == NULL) {
        return;
    }
    dll_merge_sort(&list->head);
}

int dll_find(struct DLL* list, int value) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }
    
    struct DLLNode* current = list->head;
    int index = 0;
    
    while (current != NULL) {
        if (current->data == value) {
            return index;
        }
        current = current->next;
        index++;
    }
    
    return -1; // Not found
}

void dll_insert_sorted(struct DLL* list, int value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    
    struct DLLNode* new_node = dll_createNode(value);
    
    // If list is empty or value is smaller than head
    if (list->head == NULL || list->head->data >= value) {
        new_node->next = list->head;
        if (list->head != NULL) {
            list->head->prev = new_node;
        }
        list->head = new_node;
        return;
    }
    
    // Find the position to insert
    struct DLLNode* current = list->head;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }
    
    new_node->next = current->next;
    new_node->prev = current;
    if (current->next != NULL) {
        current->next->prev = new_node;
    }
    current->next = new_node;
}

void dll_append_to_head(struct DLL* list, int value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    
    struct DLLNode* new_node = dll_createNode(value);
    new_node->next = list->head;
    if (list->head != NULL) {
        list->head->prev = new_node;
    }
    list->head = new_node;
}

void dll_append_to_tail(struct DLL* list, int value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    
    struct DLLNode* new_node = dll_createNode(value);
    
    if (list->head == NULL) {
        list->head = new_node;
        return;
    }
    
    struct DLLNode* current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    current->next = new_node;
    new_node->prev = current;
}


void dll_menu() {
    struct DLL* list = dll_create();
    char choice;
    int val, index;

    while (1) {
        printf("\n--- Doubly Linked List Menu ---\n");
        printf("1. Insert at Position\n");
        printf("2. Delete at Position\n");
        printf("3. Get Length\n");
        printf("4. Display\n");
        printf("5. Sort\n");
        printf("6. Find\n");
        printf("7. Insert Sorted\n");
        printf("8. Append to Head\n");
        printf("9. Append to Tail\n");
        printf("b. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'b') {
            break;
        } else if (choice == '1') {
            printf("Enter index: ");
            scanf("%d", &index);
            printf("Enter value to insert: ");
            scanf("%d", &val);
            dll_insert(list, index, val);
            dll_print(list);
        } else if (choice == '2') {
            printf("Enter index to delete: ");
            scanf("%d", &index);
            dll_delete(list, index);
            dll_print(list);
        } else if (choice == '3') {
            printf("Length: %d\n", dll_length(list));
        } else if (choice == '4') {
            dll_print(list);
        } else if (choice == '5') {
            dll_sort(list);
            printf("List sorted.\n");
            dll_print(list);
        } else if (choice == '6') {
            printf("Enter value to find: ");
            scanf("%d", &val);
            int pos = dll_find(list, val);
            if (pos != -1) {
                printf("Value %d found at index %d\n", val, pos);
            } else {
                printf("Value %d not found\n", val);
            }
        } else if (choice == '7') {
            printf("Enter value to insert sorted: ");
            scanf("%d", &val);
            dll_insert_sorted(list, val);
            dll_print(list);
        } else if (choice == '8') {
            printf("Enter value to append to head: ");
            scanf("%d", &val);
            dll_append_to_head(list, val);
            dll_print(list);
        } else if (choice == '9') {
            printf("Enter value to append to tail: ");
            scanf("%d", &val);
            dll_append_to_tail(list, val);
            dll_print(list);
        }
    }
    dll_destroy(list);
}
    
// --- Circular Linked List (CLL) ---

struct CLLNode {
    int data;
    struct CLLNode* next;
};

struct CLL {
    struct CLLNode* head;
};

struct CLL* cll_create() {
    struct CLL* list = (struct CLL*)malloc(sizeof(struct CLL));
    if (!list) {
        printf("Memory allocation error\n");
        exit(1);
    }
    list->head = NULL;
    return list;
}

void cll_destroy(struct CLL* list) {
    if (list == NULL) return;
    if (list->head == NULL) {
        free(list);
        return;
    }

    struct CLLNode* current = list->head;
    struct CLLNode* next;
    
    // Break the circle for easy deletion or use do-while
    struct CLLNode* head = list->head;
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != head);

    free(list);
}

struct CLLNode* cll_createNode(int data) {
    struct CLLNode* newNode = (struct CLLNode*)malloc(sizeof(struct CLLNode));
    if (!newNode) {
        printf("Memory allocation error\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void cll_insert(struct CLL* list, int index, int new_data) {
    if (list == NULL) return;
    if (index < 0) {
        printf("Invalid index\n");
        return;
    }

    struct CLLNode* new_node = cll_createNode(new_data);

    if (index == 0) {
        if (list->head == NULL) {
            new_node->next = new_node;
            list->head = new_node;
        } else {
            struct CLLNode* temp = list->head;
            while (temp->next != list->head) {
                temp = temp->next;
            }
            new_node->next = list->head;
            temp->next = new_node;
            list->head = new_node;
        }
        return;
    }

    if (list->head == NULL) {
        printf("Index out of bounds\n");
        free(new_node);
        return;
    }

    struct CLLNode* temp = list->head;
    for (int i = 0; i < index - 1; i++) {
        temp = temp->next;
        if (temp == list->head) {
             printf("Index out of bounds\n");
             free(new_node);
             return;
        }
    }

    new_node->next = temp->next;
    temp->next = new_node;
}

int cll_length(struct CLL* list) {
    if (list == NULL || list->head == NULL) return 0;
    int count = 0;
    struct CLLNode* temp = list->head;
    do {
        count++;
        temp = temp->next;
    } while (temp != list->head);
    return count;
}

void cll_delete(struct CLL* list, int position) {
    if (list == NULL || list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    struct CLLNode* temp = list->head;

    if (position == 0) {
        if (temp->next == list->head) {
            // Only one node
            free(temp);
            list->head = NULL;
        } else {
            struct CLLNode* last = list->head;
            while (last->next != list->head) {
                last = last->next;
            }
            list->head = temp->next;
            last->next = list->head;
            free(temp);
        }
        return;
    }

    struct CLLNode* prev = NULL;
    for (int i = 0; i < position; i++) {
        prev = temp;
        temp = temp->next;
        if (temp == list->head) {
            printf("Position out of bounds\n");
            return;
        }
    }

    prev->next = temp->next;
    free(temp);
}

void cll_print(struct CLL* list) {
    if (list == NULL) return;
    if (list->head == NULL) {
        printf("NULL\n");
        return;
    }
    struct CLLNode* node = list->head;
    printf("CLL: ");
    do {
        printf("%d -> ", node->data);
        node = node->next;
    } while (node != list->head);
    printf("(head)\n");
}

// CLL helper for merge sort - split list into two halves (works on linear list)
void cll_split(struct CLLNode* source, struct CLLNode** front, struct CLLNode** back) {
    struct CLLNode* fast;
    struct CLLNode* slow;
    slow = source;
    fast = source->next;
    
    // Use NULL checks since we work with linear list after breaking the circle
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    
    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

// CLL helper for merge sort - merge two sorted linear lists
struct CLLNode* cll_merge(struct CLLNode* a, struct CLLNode* b) {
    if (a == NULL) return b;
    if (b == NULL) return a;
    
    struct CLLNode* result = NULL;
    
    if (a->data <= b->data) {
        result = a;
        result->next = cll_merge(a->next, b);
    } else {
        result = b;
        result->next = cll_merge(a, b->next);
    }
    return result;
}

// CLL helper for merge sort - recursive sort (works on linear list)
void cll_merge_sort_helper(struct CLLNode** headRef) {
    struct CLLNode* head = *headRef;
    struct CLLNode* a;
    struct CLLNode* b;
    
    if (head == NULL || head->next == NULL) {
        return;
    }
    
    cll_split(head, &a, &b);
    cll_merge_sort_helper(&a);
    cll_merge_sort_helper(&b);
    *headRef = cll_merge(a, b);
}

void cll_sort(struct CLL* list) {
    if (list == NULL || list->head == NULL) {
        return;
    }
    
    // For single node
    if (list->head->next == list->head) {
        return;
    }
    
    // Break the circle temporarily
    struct CLLNode* last = list->head;
    while (last->next != list->head) {
        last = last->next;
    }
    last->next = NULL;
    
    // Sort the now linear list
    cll_merge_sort_helper(&list->head);
    
    // Restore the circle
    struct CLLNode* temp = list->head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = list->head;
}

int cll_find(struct CLL* list, int value) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }
    
    struct CLLNode* current = list->head;
    int index = 0;
    
    do {
        if (current->data == value) {
            return index;
        }
        current = current->next;
        index++;
    } while (current != list->head);
    
    return -1; // Not found
}

void cll_insert_sorted(struct CLL* list, int value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    
    struct CLLNode* new_node = cll_createNode(value);
    
    // If list is empty
    if (list->head == NULL) {
        new_node->next = new_node;
        list->head = new_node;
        return;
    }
    
    // If value should be the new head
    if (value < list->head->data) {
        // Find last node
        struct CLLNode* last = list->head;
        while (last->next != list->head) {
            last = last->next;
        }
        new_node->next = list->head;
        last->next = new_node;
        list->head = new_node;
        return;
    }
    
    // Find the position to insert
    struct CLLNode* current = list->head;
    while (current->next != list->head && current->next->data < value) {
        current = current->next;
    }
    
    new_node->next = current->next;
    current->next = new_node;
}

void cll_append_to_head(struct CLL* list, int value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    
    struct CLLNode* new_node = cll_createNode(value);
    
    if (list->head == NULL) {
        new_node->next = new_node;
        list->head = new_node;
        return;
    }
    
    // Find last node
    struct CLLNode* last = list->head;
    while (last->next != list->head) {
        last = last->next;
    }
    
    new_node->next = list->head;
    last->next = new_node;
    list->head = new_node;
}

void cll_append_to_tail(struct CLL* list, int value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }
    
    struct CLLNode* new_node = cll_createNode(value);
    
    if (list->head == NULL) {
        new_node->next = new_node;
        list->head = new_node;
        return;
    }
    
    // Find last node
    struct CLLNode* last = list->head;
    while (last->next != list->head) {
        last = last->next;
    }
    
    last->next = new_node;
    new_node->next = list->head;
}


void cll_menu() {
    struct CLL* list = cll_create();
    char choice;
    int val, index;

    while (1) {
        printf("\n--- Circular Linked List Menu ---\n");
        printf("1. Insert at Position\n");
        printf("2. Delete at Position\n");
        printf("3. Get Length\n");
        printf("4. Display\n");
        printf("5. Sort\n");
        printf("6. Find\n");
        printf("7. Insert Sorted\n");
        printf("8. Append to Head\n");
        printf("9. Append to Tail\n");
        printf("b. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'b') {
            break;
        } else if (choice == '1') {
            printf("Enter index: ");
            scanf("%d", &index);
            printf("Enter value to insert: ");
            scanf("%d", &val);
            cll_insert(list, index, val);
            cll_print(list);
        } else if (choice == '2') {
            printf("Enter index to delete: ");
            scanf("%d", &index);
            cll_delete(list, index);
            cll_print(list);
        } else if (choice == '3') {
            printf("Length: %d\n", cll_length(list));
        } else if (choice == '4') {
            cll_print(list);
        } else if (choice == '5') {
            cll_sort(list);
            printf("List sorted.\n");
            cll_print(list);
        } else if (choice == '6') {
            printf("Enter value to find: ");
            scanf("%d", &val);
            int pos = cll_find(list, val);
            if (pos != -1) {
                printf("Value %d found at index %d\n", val, pos);
            } else {
                printf("Value %d not found\n", val);
            }
        } else if (choice == '7') {
            printf("Enter value to insert sorted: ");
            scanf("%d", &val);
            cll_insert_sorted(list, val);
            cll_print(list);
        } else if (choice == '8') {
            printf("Enter value to append to head: ");
            scanf("%d", &val);
            cll_append_to_head(list, val);
            cll_print(list);
        } else if (choice == '9') {
            printf("Enter value to append to tail: ");
            scanf("%d", &val);
            cll_append_to_tail(list, val);
            cll_print(list);
        }
    }
    cll_destroy(list);
}

#ifndef SKIP_MAIN
int main() {
    char choice;

    while (1) {
        printf("\n=== Main Menu ===\n");
        printf("1. Singly Linked List (SLL)\n");
        printf("2. Doubly Linked List (DLL)\n");
        printf("3. Circular Linked List (CLL)\n");
        printf("x. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            sll_menu();
        } else if (choice == '2') {
            dll_menu();
        } else if (choice == '3') {
            cll_menu();
        } else {
            printf("Not implemented yet or invalid choice.\n");
        }
    }
    return 0;
}
#endif
