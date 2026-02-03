#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Maximum number of levels in the skip list
#define MAX_LEVEL 6
// Probability factor for level generation (0.5 = 50% chance of going up a level)
#define P_FACTOR 0.5

// ========================================
// Key-Value Skip List (Map/Dictionary)
// ========================================

// Key-value skip list node structure
struct KVSkipNode {
    int key;                        // Key for ordering
    int value;                      // Associated value
    struct KVSkipNode** forward;    // Array of forward pointers, one for each level
};

// Key-value skip list structure
struct KVSkipList {
    int level;                      // Current maximum level in the list
    struct KVSkipNode* header;      // Header node (sentinel)
};

// Generate a random level for a new node using probabilistic approach
// Returns a level between 1 and MAX_LEVEL
int random_level() {
    int lvl = 1;
    // Flip a coin (P_FACTOR probability) to decide whether to go up a level
    while (((double)rand() / RAND_MAX) < P_FACTOR && lvl < MAX_LEVEL) {
        lvl++;
    }
    return lvl;
}

// Create a new KV skip list node with the given key, value, and level
// Allocates memory for the node and its forward pointer array
struct KVSkipNode* kvskip_createNode(int key, int value, int level) {
    struct KVSkipNode* node = (struct KVSkipNode*)malloc(sizeof(struct KVSkipNode));
    if (!node) {
        printf("Memory allocation error\n");
        exit(1);
    }
    node->key = key;
    node->value = value;

    // Allocate array of forward pointers (level + 1 pointers, from 0 to level)
    node->forward = (struct KVSkipNode**)malloc(sizeof(struct KVSkipNode*) * (level + 1));
    if (!node->forward) {
        printf("Memory allocation error\n");
        free(node);
        exit(1);
    }

    // Initialize all forward pointers to NULL
    for (int i = 0; i <= level; i++) {
        node->forward[i] = NULL;
    }
    return node;
}

// Create and initialize a new KV skip list
// Returns a pointer to the newly created skip list
struct KVSkipList* kvskip_create() {
    struct KVSkipList* list = (struct KVSkipList*)malloc(sizeof(struct KVSkipList));
    if (!list) {
        printf("Memory allocation error\n");
        exit(1);
    }
    list->level = 0;  // Start with level 0
    // Create header node with dummy key/value and MAX_LEVEL forward pointers
    list->header = kvskip_createNode(-1, -1, MAX_LEVEL);
    return list;
}

// Insert a key-value pair into the KV skip list
// If the key already exists, update its value
void kvskip_insert(struct KVSkipList* list, int key, int value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }

    // Array to store nodes that need to be updated at each level
    struct KVSkipNode* update[MAX_LEVEL + 1];
    struct KVSkipNode* current = list->header;

    // Find the position to insert by traversing from top level to bottom
    for (int i = list->level; i >= 0; i--) {
        // Move forward at current level while next node's key is less than search key
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        // Store the node where we need to update the pointer at this level
        update[i] = current;
    }

    // Move to the next node at level 0 (actual position where key should be)
    current = current->forward[0];

    // If key already exists, update the value
    if (current != NULL && current->key == key) {
        current->value = value;
        return;
    }

    // Generate a random level for the new node
    int new_level = random_level();

    // If new level is higher than current max level, update the list level
    if (new_level > list->level) {
        for (int i = list->level + 1; i <= new_level; i++) {
            update[i] = list->header;
        }
        list->level = new_level;
    }

    // Create new node and insert it by updating pointers at all levels
    struct KVSkipNode* new_node = kvskip_createNode(key, value, new_level);

    for (int i = 0; i <= new_level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
}

// Search for a key in the KV skip list
// If found, stores the value in *value (if value is not NULL) and returns 1
// If not found, returns 0
int kvskip_search(struct KVSkipList* list, int key, int* value) {
    if (list == NULL) {
        printf("List does not exist\n");
        return 0;
    }

    struct KVSkipNode* current = list->header;

    // Traverse from top level to bottom, using express lanes
    for (int i = list->level; i >= 0; i--) {
        // Move forward at current level while next node's key is less than search key
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }

    // Move to the next node at level 0 (actual position)
    current = current->forward[0];

    // Check if we found the key
    if (current != NULL && current->key == key) {
        if (value != NULL) {
            *value = current->value;
        }
        return 1;  // Found
    }

    return 0;  // Not found
}

// Delete a node with the given key from the KV skip list
// Prints an error message if the key is not found
void kvskip_delete(struct KVSkipList* list, int key) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }

    // Array to store nodes that need to be updated at each level
    struct KVSkipNode* update[MAX_LEVEL + 1];
    struct KVSkipNode* current = list->header;

    // Find the node to delete by traversing from top level to bottom
    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    // Move to the actual node
    current = current->forward[0];

    // Check if the node exists
    if (current == NULL || current->key != key) {
        printf("Key %d not found\n", key);
        return;
    }

    // Update forward pointers to bypass the node being deleted
    for (int i = 0; i <= list->level; i++) {
        if (update[i]->forward[i] != current) {
            break;
        }
        update[i]->forward[i] = current->forward[i];
    }

    // Free the node's memory
    free(current->forward);
    free(current);

    // Reduce the level if the highest level is now empty
    while (list->level > 0 && list->header->forward[list->level] == NULL) {
        list->level--;
    }
}

// Display the KV skip list structure showing all levels
// Higher levels show fewer nodes (express lanes)
void kvskip_display(struct KVSkipList* list) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }

    printf("\n----- KV Skip List Structure -----\n");
    // Display from highest level to lowest
    for (int i = list->level; i >= 0; i--) {
        struct KVSkipNode* node = list->header->forward[i];
        printf("Level %d: ", i);
        while (node != NULL) {
            printf("(%d:%d) ", node->key, node->value);
            node = node->forward[i];
        }
        printf("\n");
    }
    printf("----------------------------------\n");
}

// Destroy the KV skip list and free all allocated memory
void kvskip_destroy(struct KVSkipList* list) {
    if (list == NULL) return;

    struct KVSkipNode* current = list->header;
    struct KVSkipNode* next;

    // Traverse at level 0 and free each node
    while (current != NULL) {
        next = current->forward[0];
        free(current->forward);  // Free the forward pointer array
        free(current);           // Free the node itself
        current = next;
    }

    free(list);  // Free the list structure
}

// Check if the KV skip list is empty
// Returns 1 if empty, 0 otherwise
int kvskip_isEmpty(struct KVSkipList* list) {
    if (list == NULL) return 1;
    return list->header->forward[0] == NULL;
}

// Print all elements in the KV skip list in sorted order (level 0 traversal)
void kvskip_print(struct KVSkipList* list) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }

    if (kvskip_isEmpty(list)) {
        printf("KV Skip list is empty\n");
        return;
    }

    printf("KV Skip List: ");
    struct KVSkipNode* current = list->header->forward[0];
    while (current != NULL) {
        printf("(%d:%d) ", current->key, current->value);
        current = current->forward[0];
    }
    printf("\n");
}

// ========================================
// Simple Skip List (Sorted List)
// ========================================

// Simple skip list node structure (only data, no separate key)
struct SkipNode {
    int data;                       // Data value (used for both ordering and storage)
    struct SkipNode** forward;      // Array of forward pointers, one for each level
};

// Simple skip list structure
struct SkipList {
    int level;                      // Current maximum level in the list
    struct SkipNode* header;        // Header node (sentinel)
};

// Create a new simple skip list node with the given data and level
// Allocates memory for the node and its forward pointer array
struct SkipNode* skip_createNode(int data, int level) {
    struct SkipNode* node = (struct SkipNode*)malloc(sizeof(struct SkipNode));
    if (!node) {
        printf("Memory allocation error\n");
        exit(1);
    }
    node->data = data;

    // Allocate array of forward pointers (level + 1 pointers, from 0 to level)
    node->forward = (struct SkipNode**)malloc(sizeof(struct SkipNode*) * (level + 1));
    if (!node->forward) {
        printf("Memory allocation error\n");
        free(node);
        exit(1);
    }

    // Initialize all forward pointers to NULL
    for (int i = 0; i <= level; i++) {
        node->forward[i] = NULL;
    }
    return node;
}

// Create and initialize a new simple skip list
// Returns a pointer to the newly created skip list
struct SkipList* skip_create() {
    struct SkipList* list = (struct SkipList*)malloc(sizeof(struct SkipList));
    if (!list) {
        printf("Memory allocation error\n");
        exit(1);
    }
    list->level = 0;  // Start with level 0
    // Create header node with dummy data and MAX_LEVEL forward pointers
    list->header = skip_createNode(-1, MAX_LEVEL);
    return list;
}

// Insert data into the simple skip list (maintains sorted order)
// Allows duplicates
void skip_insert(struct SkipList* list, int data) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }

    // Array to store nodes that need to be updated at each level
    struct SkipNode* update[MAX_LEVEL + 1];
    struct SkipNode* current = list->header;

    // Find the position to insert by traversing from top level to bottom
    for (int i = list->level; i >= 0; i--) {
        // Move forward at current level while next node's data is less than search data
        while (current->forward[i] != NULL && current->forward[i]->data < data) {
            current = current->forward[i];
        }
        // Store the node where we need to update the pointer at this level
        update[i] = current;
    }

    // Generate a random level for the new node
    int new_level = random_level();

    // If new level is higher than current max level, update the list level
    if (new_level > list->level) {
        for (int i = list->level + 1; i <= new_level; i++) {
            update[i] = list->header;
        }
        list->level = new_level;
    }

    // Create new node and insert it by updating pointers at all levels
    struct SkipNode* new_node = skip_createNode(data, new_level);

    for (int i = 0; i <= new_level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
}

// Search for data in the simple skip list
// Returns 1 if found, 0 if not found
int skip_search(struct SkipList* list, int data) {
    if (list == NULL) {
        printf("List does not exist\n");
        return 0;
    }

    struct SkipNode* current = list->header;

    // Traverse from top level to bottom, using express lanes
    for (int i = list->level; i >= 0; i--) {
        // Move forward at current level while next node's data is less than search data
        while (current->forward[i] != NULL && current->forward[i]->data < data) {
            current = current->forward[i];
        }
    }

    // Move to the next node at level 0 (actual position)
    current = current->forward[0];

    // Check if we found the data
    if (current != NULL && current->data == data) {
        return 1;  // Found
    }

    return 0;  // Not found
}

// Delete the first occurrence of data from the simple skip list
// Prints an error message if the data is not found
void skip_delete(struct SkipList* list, int data) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }

    // Array to store nodes that need to be updated at each level
    struct SkipNode* update[MAX_LEVEL + 1];
    struct SkipNode* current = list->header;

    // Find the node to delete by traversing from top level to bottom
    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->data < data) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    // Move to the actual node
    current = current->forward[0];

    // Check if the node exists
    if (current == NULL || current->data != data) {
        printf("Data %d not found\n", data);
        return;
    }

    // Update forward pointers to bypass the node being deleted
    for (int i = 0; i <= list->level; i++) {
        if (update[i]->forward[i] != current) {
            break;
        }
        update[i]->forward[i] = current->forward[i];
    }

    // Free the node's memory
    free(current->forward);
    free(current);

    // Reduce the level if the highest level is now empty
    while (list->level > 0 && list->header->forward[list->level] == NULL) {
        list->level--;
    }
}

// Display the simple skip list structure showing all levels
// Higher levels show fewer nodes (express lanes)
void skip_display(struct SkipList* list) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }

    printf("\n----- Skip List Structure -----\n");
    // Display from highest level to lowest
    for (int i = list->level; i >= 0; i--) {
        struct SkipNode* node = list->header->forward[i];
        printf("Level %d: ", i);
        while (node != NULL) {
            printf("%d ", node->data);
            node = node->forward[i];
        }
        printf("\n");
    }
    printf("-------------------------------\n");
}

// Destroy the simple skip list and free all allocated memory
void skip_destroy(struct SkipList* list) {
    if (list == NULL) return;

    struct SkipNode* current = list->header;
    struct SkipNode* next;

    // Traverse at level 0 and free each node
    while (current != NULL) {
        next = current->forward[0];
        free(current->forward);  // Free the forward pointer array
        free(current);           // Free the node itself
        current = next;
    }

    free(list);  // Free the list structure
}

// Check if the simple skip list is empty
// Returns 1 if empty, 0 otherwise
int skip_isEmpty(struct SkipList* list) {
    if (list == NULL) return 1;
    return list->header->forward[0] == NULL;
}

// Print all elements in the simple skip list in sorted order (level 0 traversal)
void skip_print(struct SkipList* list) {
    if (list == NULL) {
        printf("List does not exist\n");
        return;
    }

    if (skip_isEmpty(list)) {
        printf("Skip list is empty\n");
        return;
    }

    printf("Skip List: ");
    struct SkipNode* current = list->header->forward[0];
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->forward[0];
    }
    printf("\n");
}

// ========================================
// Test/Menu Functions
// ========================================

// Interactive menu for Key-Value Skip List
void kvskip_menu() {
    struct KVSkipList* list = kvskip_create();
    char choice;
    int key, value;

    while (1) {
        printf("\n=== Key-Value Skip List Menu ===\n");
        printf("1. Insert key-value pair\n");
        printf("2. Search by key\n");
        printf("3. Delete by key\n");
        printf("4. Print list\n");
        printf("5. Display structure\n");
        printf("6. Check if empty\n");
        printf("x. Back to main menu\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            printf("Enter key: ");
            scanf("%d", &key);
            printf("Enter value: ");
            scanf("%d", &value);
            kvskip_insert(list, key, value);
            printf("Inserted (%d:%d)\n", key, value);
        } else if (choice == '2') {
            printf("Enter key to search: ");
            scanf("%d", &key);
            if (kvskip_search(list, key, &value)) {
                printf("Found - Key: %d, Value: %d\n", key, value);
            } else {
                printf("Key %d not found\n", key);
            }
        } else if (choice == '3') {
            printf("Enter key to delete: ");
            scanf("%d", &key);
            kvskip_delete(list, key);
        } else if (choice == '4') {
            kvskip_print(list);
        } else if (choice == '5') {
            kvskip_display(list);
        } else if (choice == '6') {
            printf("Is empty: %s\n", kvskip_isEmpty(list) ? "Yes" : "No");
        } else {
            printf("Invalid choice\n");
        }
    }

    kvskip_destroy(list);
}

// Interactive menu for Simple Skip List
void skip_menu() {
    struct SkipList* list = skip_create();
    char choice;
    int data;

    while (1) {
        printf("\n=== Simple Skip List Menu ===\n");
        printf("1. Insert data\n");
        printf("2. Search data\n");
        printf("3. Delete data\n");
        printf("4. Print list\n");
        printf("5. Display structure\n");
        printf("6. Check if empty\n");
        printf("x. Back to main menu\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            printf("Enter data to insert: ");
            scanf("%d", &data);
            skip_insert(list, data);
            printf("Inserted %d\n", data);
        } else if (choice == '2') {
            printf("Enter data to search: ");
            scanf("%d", &data);
            if (skip_search(list, data)) {
                printf("Data %d found\n", data);
            } else {
                printf("Data %d not found\n", data);
            }
        } else if (choice == '3') {
            printf("Enter data to delete: ");
            scanf("%d", &data);
            skip_delete(list, data);
        } else if (choice == '4') {
            skip_print(list);
        } else if (choice == '5') {
            skip_display(list);
        } else if (choice == '6') {
            printf("Is empty: %s\n", skip_isEmpty(list) ? "Yes" : "No");
        } else {
            printf("Invalid choice\n");
        }
    }

    skip_destroy(list);
}

// Main menu
int main() {
    srand(time(NULL));
    char choice;

    while (1) {
        printf("\n=== Skip List Main Menu ===\n");
        printf("1. Key-Value Skip List (Map/Dictionary)\n");
        printf("2. Simple Skip List (Sorted List)\n");
        printf("x. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            kvskip_menu();
        } else if (choice == '2') {
            skip_menu();
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
