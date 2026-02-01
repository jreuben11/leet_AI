#include <stdio.h>
#include <stdlib.h>

// --- Singly Linked List (SLL) ---

struct SLLNode {
    int data;
    struct SLLNode* next;
};

struct SLL {
    struct SLLNode* head;
};

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

struct SLLNode* sll_createNode(int data) {
    struct SLLNode* newNode = (struct SLLNode*)malloc(sizeof(struct SLLNode));
    if (!newNode) {
        printf("Memory allocation error\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void sll_insert(struct SLL* list, int index, int new_data) {
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
        printf("%d -> ", node->data);
        node = node->next;
    }
    printf("NULL\n");
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
        }
    }
    cll_destroy(list);
}

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
