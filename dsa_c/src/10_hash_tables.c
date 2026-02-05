/**
 * HASH TABLES - Collision Resolution via Chaining
 *
 * What is a Hash Table?
 * --------------------
 * A hash table is a data structure that maps keys to values using a hash function.
 * It provides O(1) average-case lookup, insert, and delete operations.
 *
 * How it works:
 * 1. Hash function converts key → index in array
 * 2. Store key-value pair at that index
 * 3. Handle collisions when different keys hash to same index
 *
 * Key Components:
 * - Hash Function: Converts key → array index
 * - Buckets: Array slots that store data
 * - Collision Resolution: Strategy when multiple keys hash to same bucket
 *
 * This implementation uses CHAINING for collision resolution:
 * - Each bucket is a linked list
 * - Multiple keys at same index → stored in same list
 * - Simple, flexible, handles any number of collisions
 *
 * Time Complexity:
 * - Average case: O(1) for lookup, insert, delete
 * - Worst case: O(n) if all keys hash to same bucket (becomes linked list)
 * - Load factor (n/m) determines performance: keep < 0.75 for good performance
 *
 * Space Complexity: O(n + m) where n = entries, m = table size
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ============================================================
// HASH TABLE STRUCTURE
// ============================================================

/**
 * Hash Table Entry (Key-Value Pair)
 *
 * Stores both key and value so we can:
 * 1. Distinguish between different keys that hash to same bucket
 * 2. Return the value associated with a specific key
 *
 * Uses a linked list for chaining (collision resolution)
 */
typedef struct HashEntry {
    char* key;                   // String key
    int value;                   // Integer value
    struct HashEntry* next;      // Next entry in chain (for collisions)
} HashEntry;

/**
 * Hash Table Structure
 *
 * Components:
 * - buckets: Array of linked list heads (one per bucket)
 * - size: Number of buckets in the table
 * - count: Number of key-value pairs stored
 * - collisions: Track how many collisions occurred
 */
typedef struct {
    HashEntry** buckets;  // Array of bucket (linked list) pointers
    int size;             // Number of buckets
    int count;            // Number of entries
    int collisions;       // Collision counter for analysis
} HashTable;

// ============================================================
// HASH FUNCTIONS
// ============================================================

/**
 * Hash Function 1: Simple Additive Hash (POOR)
 *
 * Method: Sum all character ASCII values, mod by table size
 *
 * Why it's poor:
 * - Anagrams hash to same value ("abc" == "bca" == "cab")
 * - Poor distribution for real-world data
 * - Clusters keys that have similar character sums
 *
 * Example:
 * "cat" = 'c' + 'a' + 't' = 99 + 97 + 116 = 312
 * "tac" = 't' + 'a' + 'c' = 116 + 97 + 99 = 312  (COLLISION!)
 *
 * Time: O(n) where n = key length
 */
unsigned int hash_additive(const char* key, int table_size) {
    unsigned int hash = 0;

    for (int i = 0; key[i] != '\0'; i++) {
        hash += (unsigned char)key[i];
    }

    return hash % table_size;
}

/**
 * Hash Function 2: Multiplicative Hash (BETTER)
 *
 * Method: Multiply running hash by prime, add character
 * hash = (hash * PRIME) + char
 *
 * Why it's better:
 * - Position matters: "abc" ≠ "bca" ≠ "cab"
 * - Prime multiplier improves distribution
 * - Commonly used in practice
 *
 * The magic of 31:
 * - Prime number (reduces patterns)
 * - Small enough to avoid overflow quickly
 * - Can be optimized to (hash << 5) - hash
 *
 * Time: O(n) where n = key length
 */
unsigned int hash_multiplicative(const char* key, int table_size) {
    unsigned int hash = 0;
    const unsigned int PRIME = 31;  // Could also use 37, 41, etc.

    for (int i = 0; key[i] != '\0'; i++) {
        hash = hash * PRIME + (unsigned char)key[i];
    }

    return hash % table_size;
}

/**
 * Hash Function 3: DJB2 Hash (EXCELLENT)
 *
 * Method: hash = hash * 33 + char (starting from 5381)
 *
 * Created by Dan Bernstein, widely used in practice.
 *
 * Why it's excellent:
 * - Magic constant 33 = 32 + 1 = (hash << 5) + hash (fast bit shift)
 * - Starting value 5381 chosen empirically for good distribution
 * - Very good distribution for strings
 * - Fast and simple
 *
 * Time: O(n) where n = key length
 */
unsigned int hash_djb2(const char* key, int table_size) {
    unsigned int hash = 5381;  // Magic starting value

    for (int i = 0; key[i] != '\0'; i++) {
        // hash = hash * 33 + char
        // Optimized: hash = ((hash << 5) + hash) + char
        hash = ((hash << 5) + hash) + (unsigned char)key[i];
    }

    return hash % table_size;
}

/**
 * Hash Function 4: FNV-1a Hash (EXCELLENT)
 *
 * Fowler-Noll-Vo hash, designed for hash tables.
 *
 * Method: XOR then multiply (opposite of FNV-1)
 * hash = (hash XOR char) * FNV_PRIME
 *
 * Why it's excellent:
 * - Very good avalanche effect (small changes → big hash changes)
 * - Good distribution
 * - Industry standard
 *
 * Time: O(n) where n = key length
 */
unsigned int hash_fnv1a(const char* key, int table_size) {
    unsigned int hash = 2166136261u;  // FNV offset basis (32-bit)
    const unsigned int FNV_PRIME = 16777619u;

    for (int i = 0; key[i] != '\0'; i++) {
        hash ^= (unsigned char)key[i];  // XOR with byte
        hash *= FNV_PRIME;               // Multiply by prime
    }

    return hash % table_size;
}

// Function pointer type for hash functions
typedef unsigned int (*HashFunction)(const char*, int);

// Global hash function selector
HashFunction current_hash_func = hash_djb2;  // Default to DJB2
const char* hash_func_names[] = {"Additive", "Multiplicative", "DJB2", "FNV-1a"};
int current_hash_index = 2;  // Default to DJB2

// ============================================================
// HASH TABLE OPERATIONS
// ============================================================

/**
 * Create a new hash table
 *
 * @param size Number of buckets (should be prime for better distribution)
 * @return Pointer to new hash table
 */
HashTable* hash_table_create(int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));

    table->size = size;
    table->count = 0;
    table->collisions = 0;

    // Allocate array of bucket pointers
    table->buckets = (HashEntry**)calloc(size, sizeof(HashEntry*));

    // Initially all buckets are NULL (empty)
    for (int i = 0; i < size; i++) {
        table->buckets[i] = NULL;
    }

    return table;
}

/**
 * Insert key-value pair into hash table
 *
 * Process:
 * 1. Hash key to get bucket index
 * 2. Check if key already exists (update value)
 * 3. If new key, prepend to bucket's linked list
 * 4. Track collisions for analysis
 *
 * Time: O(1) average, O(n) worst case (if all in one bucket)
 */
void hash_table_insert(HashTable* table, const char* key, int value) {
    // 1. Compute hash to find bucket
    unsigned int index = current_hash_func(key, table->size);

    // 2. Check if key already exists in this bucket
    HashEntry* current = table->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // Key exists, update value
            printf("Key '%s' already exists. Updated value: %d → %d\n",
                   key, current->value, value);
            current->value = value;
            return;
        }
        current = current->next;
    }

    // 3. Key doesn't exist, create new entry
    HashEntry* new_entry = (HashEntry*)malloc(sizeof(HashEntry));
    new_entry->key = strdup(key);  // Make copy of key string
    new_entry->value = value;

    // 4. Prepend to bucket's linked list (O(1) insertion)
    new_entry->next = table->buckets[index];

    // If bucket wasn't empty, this is a collision
    if (table->buckets[index] != NULL) {
        table->collisions++;
        printf("→ Collision at bucket %u! (using chaining)\n", index);
    }

    table->buckets[index] = new_entry;
    table->count++;
    printf("Inserted: '%s' → %d (bucket %u)\n", key, value, index);
}

/**
 * Search for key in hash table
 *
 * Process:
 * 1. Hash key to find bucket
 * 2. Search linked list at that bucket
 *
 * @return Pointer to value if found, NULL otherwise
 *
 * Time: O(1) average, O(n) worst case
 */
int* hash_table_search(HashTable* table, const char* key) {
    unsigned int index = current_hash_func(key, table->size);

    HashEntry* current = table->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return &(current->value);
        }
        current = current->next;
    }

    return NULL;  // Key not found
}

/**
 * Delete key from hash table
 *
 * Process:
 * 1. Hash key to find bucket
 * 2. Search and remove from linked list
 *
 * Time: O(1) average, O(n) worst case
 */
bool hash_table_delete(HashTable* table, const char* key) {
    unsigned int index = current_hash_func(key, table->size);

    HashEntry* current = table->buckets[index];
    HashEntry* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // Found the key, remove it
            if (prev == NULL) {
                // Removing first entry in bucket
                table->buckets[index] = current->next;
            } else {
                // Removing from middle/end
                prev->next = current->next;
            }

            free(current->key);
            free(current);
            table->count--;
            return true;
        }

        prev = current;
        current = current->next;
    }

    return false;  // Key not found
}

/**
 * Display hash table contents
 */
void hash_table_display(HashTable* table) {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║              Hash Table Contents                  ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
    printf("Hash Function: %s\n", hash_func_names[current_hash_index]);
    printf("Size: %d buckets | Entries: %d | Collisions: %d\n",
           table->size, table->count, table->collisions);
    printf("Load Factor: %.2f\n\n", (double)table->count / table->size);

    for (int i = 0; i < table->size; i++) {
        printf("Bucket %2d: ", i);

        HashEntry* current = table->buckets[i];
        if (current == NULL) {
            printf("(empty)\n");
        } else {
            while (current != NULL) {
                printf("[%s=%d]", current->key, current->value);
                if (current->next != NULL) {
                    printf(" → ");
                }
                current = current->next;
            }
            printf("\n");
        }
    }
}

/**
 * Display collision statistics
 */
void hash_table_stats(HashTable* table) {
    int empty_buckets = 0;
    int max_chain_length = 0;
    int total_chain_length = 0;

    for (int i = 0; i < table->size; i++) {
        int chain_length = 0;
        HashEntry* current = table->buckets[i];

        if (current == NULL) {
            empty_buckets++;
        } else {
            while (current != NULL) {
                chain_length++;
                current = current->next;
            }
            total_chain_length += chain_length;
            if (chain_length > max_chain_length) {
                max_chain_length = chain_length;
            }
        }
    }

    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║           Hash Table Statistics                   ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
    printf("Hash Function:       %s\n", hash_func_names[current_hash_index]);
    printf("Total buckets:       %d\n", table->size);
    printf("Empty buckets:       %d (%.1f%%)\n",
           empty_buckets, 100.0 * empty_buckets / table->size);
    printf("Used buckets:        %d (%.1f%%)\n",
           table->size - empty_buckets,
           100.0 * (table->size - empty_buckets) / table->size);
    printf("Total entries:       %d\n", table->count);
    printf("Total collisions:    %d\n", table->collisions);
    printf("Load factor:         %.2f\n", (double)table->count / table->size);
    printf("Max chain length:    %d\n", max_chain_length);
    if (table->size - empty_buckets > 0) {
        printf("Avg chain length:    %.2f\n",
               (double)total_chain_length / (table->size - empty_buckets));
    }
}

/**
 * Destroy hash table and free all memory
 */
void hash_table_destroy(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        HashEntry* current = table->buckets[i];
        while (current != NULL) {
            HashEntry* next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }

    free(table->buckets);
    free(table);
}

// ============================================================
// DEMO FUNCTIONS
// ============================================================

void demo_hash_comparison() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║       Hash Function Comparison (Anagrams)        ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    const char* anagrams[] = {"listen", "silent", "enlist"};
    int table_size = 10;

    printf("Testing anagrams (same letters, different order):\n");
    printf("Words: listen, silent, enlist\n\n");

    printf("%-15s %-12s %-12s %-12s %-12s\n",
           "Word", "Additive", "Multiply", "DJB2", "FNV-1a");
    printf("%-15s %-12s %-12s %-12s %-12s\n",
           "----", "--------", "--------", "----", "------");

    for (int i = 0; i < 3; i++) {
        printf("%-15s %-12u %-12u %-12u %-12u\n",
               anagrams[i],
               hash_additive(anagrams[i], table_size),
               hash_multiplicative(anagrams[i], table_size),
               hash_djb2(anagrams[i], table_size),
               hash_fnv1a(anagrams[i], table_size));
    }

    printf("\n💡 Key Observation:\n");
    printf("   Additive: All anagrams hash to SAME value → BAD!\n");
    printf("   Others: Different values for each anagram → GOOD!\n");
    printf("\nPress Enter to continue...");
    getchar();
}

void demo_collision_poor_hash() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║     Collision Demo: Poor Hash (Additive)         ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    // Temporarily switch to poor hash
    HashFunction saved = current_hash_func;
    int saved_index = current_hash_index;
    current_hash_func = hash_additive;
    current_hash_index = 0;

    HashTable* table = hash_table_create(5);
    const char* words[] = {"cat", "act", "tac", "dog", "god", "hello"};

    printf("Inserting anagrams with ADDITIVE hash:\n\n");
    for (int i = 0; i < 6; i++) {
        hash_table_insert(table, words[i], i + 1);
    }

    hash_table_display(table);
    hash_table_stats(table);

    hash_table_destroy(table);

    // Restore hash function
    current_hash_func = saved;
    current_hash_index = saved_index;

    printf("\nPress Enter to continue...");
    getchar();
}

void demo_good_distribution() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║      Good Distribution Demo: DJB2 Hash           ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    // Ensure using good hash
    current_hash_func = hash_djb2;
    current_hash_index = 2;

    HashTable* table = hash_table_create(11);  // Prime number
    const char* words[] = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "honeydew", "kiwi", "lemon", "mango"
    };

    printf("Inserting 11 fruits with DJB2 hash:\n\n");
    for (int i = 0; i < 11; i++) {
        hash_table_insert(table, words[i], i + 1);
    }

    hash_table_display(table);
    hash_table_stats(table);

    hash_table_destroy(table);

    printf("\nPress Enter to continue...");
    getchar();
}

// ============================================================
// INTERACTIVE MENU
// ============================================================

void interactive_menu() {
    HashTable* table = hash_table_create(7);  // Start with size 7 (prime)
    char choice;
    char key[100];
    int value;

    while (1) {
        printf("\n╔═══════════════════════════════════════════════════╗\n");
        printf("║          Hash Table Interactive Menu             ║\n");
        printf("╚═══════════════════════════════════════════════════╝\n");
        printf("Current Hash: %s | Entries: %d | Collisions: %d\n\n",
               hash_func_names[current_hash_index], table->count, table->collisions);

        printf("1. Insert key-value pair\n");
        printf("2. Search for key\n");
        printf("3. Delete key\n");
        printf("4. Display table\n");
        printf("5. Show statistics\n");
        printf("6. Change hash function\n");
        printf("7. Clear table\n");
        printf("b. Back to main menu\n");
        printf("\nEnter choice: ");
        scanf(" %c", &choice);

        if (choice == 'b') {
            break;
        } else if (choice == '1') {
            printf("Enter key: ");
            scanf("%s", key);
            printf("Enter value: ");
            scanf("%d", &value);
            hash_table_insert(table, key, value);

        } else if (choice == '2') {
            printf("Enter key to search: ");
            scanf("%s", key);
            int* result = hash_table_search(table, key);
            if (result) {
                printf("✓ Found: '%s' → %d\n", key, *result);
            } else {
                printf("✗ Key '%s' not found\n", key);
            }

        } else if (choice == '3') {
            printf("Enter key to delete: ");
            scanf("%s", key);
            if (hash_table_delete(table, key)) {
                printf("✓ Deleted key '%s'\n", key);
            } else {
                printf("✗ Key '%s' not found\n", key);
            }

        } else if (choice == '4') {
            hash_table_display(table);

        } else if (choice == '5') {
            hash_table_stats(table);

        } else if (choice == '6') {
            printf("\nSelect hash function:\n");
            printf("1. Additive (poor)\n");
            printf("2. Multiplicative (better)\n");
            printf("3. DJB2 (excellent)\n");
            printf("4. FNV-1a (excellent)\n");
            printf("Choice: ");
            scanf(" %c", &choice);

            HashFunction funcs[] = {hash_additive, hash_multiplicative, hash_djb2, hash_fnv1a};
            if (choice >= '1' && choice <= '4') {
                current_hash_index = choice - '1';
                current_hash_func = funcs[current_hash_index];
                printf("✓ Switched to %s hash\n", hash_func_names[current_hash_index]);
                printf("⚠ Warning: Existing entries will hash to different locations!\n");
            }

        } else if (choice == '7') {
            hash_table_destroy(table);
            table = hash_table_create(7);
            printf("✓ Table cleared\n");

        } else {
            printf("Invalid choice\n");
        }
    }

    hash_table_destroy(table);
}

// ============================================================
// MAIN
// ============================================================

int main() {
    char choice;

    while (1) {
        printf("\n╔═══════════════════════════════════════════════════╗\n");
        printf("║          HASH TABLES - Main Menu                  ║\n");
        printf("╚═══════════════════════════════════════════════════╝\n");
        printf("\n");
        printf("Demonstrations:\n");
        printf("1. Hash Function Comparison (Anagrams)\n");
        printf("2. Collision Demo (Poor Hash)\n");
        printf("3. Good Distribution Demo (DJB2)\n");
        printf("\n");
        printf("Interactive:\n");
        printf("4. Interactive Hash Table\n");
        printf("\n");
        printf("x. Exit\n");
        printf("\nEnter choice: ");
        scanf(" %c", &choice);

        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            demo_hash_comparison();
        } else if (choice == '2') {
            demo_collision_poor_hash();
        } else if (choice == '3') {
            demo_good_distribution();
        } else if (choice == '4') {
            interactive_menu();
        } else {
            printf("Invalid choice\n");
        }
    }

    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║                 Key Takeaways                     ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    printf("1. Hash Function Quality:\n");
    printf("   • Poor (additive): Anagrams collide\n");
    printf("   • Good (DJB2, FNV): Better distribution\n\n");

    printf("2. Collision Resolution (Chaining):\n");
    printf("   • Each bucket is a linked list\n");
    printf("   • Multiple keys share same bucket\n");
    printf("   • Simple, handles unlimited collisions\n\n");

    printf("3. Load Factor Impact:\n");
    printf("   • < 0.5: Fast, wastes space\n");
    printf("   • ~0.75: Good balance\n");
    printf("   • > 1.0: Slower, many collisions\n\n");

    printf("4. Performance:\n");
    printf("   • Average: O(1) all operations\n");
    printf("   • Worst: O(n) if all in one bucket\n\n");

    return 0;
}
