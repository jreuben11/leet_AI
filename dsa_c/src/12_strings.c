/**
 * STRING ALGORITHMS - Pattern Matching and Data Structures
 *
 * This file demonstrates five important string algorithms/structures:
 * 1. Trie (Prefix Tree) - Efficient string storage and prefix search
 * 2. Rabin-Karp Algorithm - Fast string matching using rolling hash
 * 3. Knuth-Morris-Pratt (KMP) - Efficient pattern matching without backtracking
 * 4. Boyer-Moore Algorithm - Advanced pattern matching with right-to-left scanning
 * 5. Levenshtein Distance - Measuring string similarity via edit distance
 *
 * Each has different use cases and performance characteristics.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// ============================================================
// TRIE (PREFIX TREE)
// ============================================================

/**
 * Trie (Prefix Tree) Data Structure
 *
 * What is a Trie?
 * ---------------
 * A tree-based data structure for storing strings where:
 * - Each path from root to a node represents a prefix
 * - Each edge is labeled with a character
 * - Nodes can be marked as "end of word"
 *
 * Example Trie for ["cat", "car", "card", "care", "dog"]:
 *
 *           (root)
 *          /      \
 *         c        d
 *         |        |
 *         a        o
 *        / \       |
 *       t   r      g*
 *      *   /|\
 *         d e
 *        *| |*
 *         |
 *         *
 *
 * (* = end of word)
 *
 * Key Operations:
 * ---------------
 * INSERT(word):
 *   - Start at root
 *   - For each character:
 *     * If child exists for char, go to child
 *     * Else create new child node
 *   - Mark last node as end of word
 *   - Time: O(m) where m = word length
 *
 * SEARCH(word):
 *   - Follow path for each character
 *   - Check if final node is marked as end of word
 *   - Time: O(m)
 *
 * PREFIX_SEARCH(prefix):
 *   - Follow path for prefix
 *   - If path exists, prefix is in trie
 *   - Time: O(m)
 *
 * When to Use Trie:
 * -----------------
 * ✅ Autocomplete (find all words with prefix)
 * ✅ Spell checking (dictionary lookup)
 * ✅ IP routing (longest prefix matching)
 * ✅ T9 predictive text
 * ✅ Word games (Scrabble, Boggle)
 *
 * ❌ NOT good for: Single string searches (use hash table instead)
 *
 * Space Complexity: O(ALPHABET_SIZE * N * M)
 * - N = number of words
 * - M = average word length
 * - Can be memory-intensive for large alphabets
 *
 * Optimization: Use hash table instead of array for children
 *   (reduces space when alphabet is large or sparse)
 */

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool is_end_of_word;
    int word_count;  // Number of words ending at this node
} TrieNode;

typedef struct {
    TrieNode* root;
    int total_words;
} Trie;

/**
 * Create a new Trie node
 */
TrieNode* trie_node_create() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->is_end_of_word = false;
    node->word_count = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }

    return node;
}

/**
 * Create a new Trie
 */
Trie* trie_create() {
    Trie* trie = (Trie*)malloc(sizeof(Trie));
    trie->root = trie_node_create();
    trie->total_words = 0;
    return trie;
}

/**
 * Insert a word into the Trie
 *
 * Process:
 * 1. Start at root
 * 2. For each character:
 *    - Convert to lowercase and get index (a=0, b=1, ...)
 *    - If child doesn't exist, create it
 *    - Move to child node
 * 3. Mark final node as end of word
 */
void trie_insert(Trie* trie, const char* word) {
    TrieNode* current = trie->root;
    int len = strlen(word);

    printf("Inserting '%s': root", word);

    for (int i = 0; i < len; i++) {
        char c = tolower(word[i]);
        int index = c - 'a';

        if (index < 0 || index >= ALPHABET_SIZE) {
            printf(" (skipping non-alphabetic char '%c')", word[i]);
            continue;
        }

        if (current->children[index] == NULL) {
            current->children[index] = trie_node_create();
            printf(" → %c (new)", c);
        } else {
            printf(" → %c", c);
        }

        current = current->children[index];
    }

    if (!current->is_end_of_word) {
        current->is_end_of_word = true;
        current->word_count++;
        trie->total_words++;
        printf(" [END]\n");
    } else {
        printf(" (already exists)\n");
    }
}

/**
 * Search for exact word in Trie
 *
 * Returns: true if word exists, false otherwise
 */
bool trie_search(Trie* trie, const char* word) {
    TrieNode* current = trie->root;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        char c = tolower(word[i]);
        int index = c - 'a';

        if (index < 0 || index >= ALPHABET_SIZE || current->children[index] == NULL) {
            return false;
        }

        current = current->children[index];
    }

    return current != NULL && current->is_end_of_word;
}

/**
 * Check if any word in Trie starts with given prefix
 *
 * Returns: true if prefix exists, false otherwise
 */
bool trie_starts_with(Trie* trie, const char* prefix) {
    TrieNode* current = trie->root;
    int len = strlen(prefix);

    for (int i = 0; i < len; i++) {
        char c = tolower(prefix[i]);
        int index = c - 'a';

        if (index < 0 || index >= ALPHABET_SIZE || current->children[index] == NULL) {
            return false;
        }

        current = current->children[index];
    }

    return true;
}

/**
 * Helper function to print all words with given prefix
 */
void trie_print_words_helper(TrieNode* node, char* prefix, int level) {
    if (node->is_end_of_word) {
        prefix[level] = '\0';
        printf("  - %s\n", prefix);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            prefix[level] = 'a' + i;
            trie_print_words_helper(node->children[i], prefix, level + 1);
        }
    }
}

/**
 * Print all words with given prefix (autocomplete)
 */
void trie_autocomplete(Trie* trie, const char* prefix) {
    TrieNode* current = trie->root;
    int len = strlen(prefix);

    // Navigate to prefix node
    for (int i = 0; i < len; i++) {
        char c = tolower(prefix[i]);
        int index = c - 'a';

        if (index < 0 || index >= ALPHABET_SIZE || current->children[index] == NULL) {
            printf("No words found with prefix '%s'\n", prefix);
            return;
        }

        current = current->children[index];
    }

    // Print all words from this node
    printf("Words starting with '%s':\n", prefix);
    char buffer[100];
    strcpy(buffer, prefix);
    trie_print_words_helper(current, buffer, len);
}

/**
 * Destroy Trie and free memory
 */
void trie_destroy_helper(TrieNode* node) {
    if (node == NULL) return;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        trie_destroy_helper(node->children[i]);
    }

    free(node);
}

void trie_destroy(Trie* trie) {
    trie_destroy_helper(trie->root);
    free(trie);
}

// ============================================================
// RABIN-KARP ALGORITHM
// ============================================================

/**
 * Rabin-Karp String Matching Algorithm
 *
 * What is Rabin-Karp?
 * -------------------
 * A string matching algorithm that uses hashing to find a pattern in text.
 * Key insight: Use ROLLING HASH to efficiently compute hash for each substring.
 *
 * How it Works:
 * -------------
 * 1. Compute hash of pattern
 * 2. Compute hash of first window in text
 * 3. Slide window through text:
 *    a. If hashes match → verify with character comparison (handle collisions)
 *    b. Roll hash: remove leftmost char, add new rightmost char
 * 4. Report all matches
 *
 * Rolling Hash Formula (Rabin fingerprint):
 * ------------------------------------------
 * hash = (c[0] * d^(m-1) + c[1] * d^(m-2) + ... + c[m-1] * d^0) % q
 *
 * Where:
 * - d = base (size of alphabet, typically 256 for ASCII)
 * - q = large prime (to minimize collisions)
 * - m = pattern length
 *
 * Rolling Hash Update:
 * --------------------
 * Remove leftmost char: hash -= (c[0] * d^(m-1)) % q
 * Shift left: hash *= d
 * Add new char: hash += c[new]
 * Take modulo: hash %= q
 *
 * Example:
 *   Text:    "ABCDEFG"
 *   Pattern: "CDE"
 *
 *   hash("ABC") → slide → hash("BCD") → slide → hash("CDE") ✓
 *
 * Time Complexity:
 * ----------------
 * - Average: O(n + m) where n = text length, m = pattern length
 * - Worst: O(n * m) if many hash collisions occur
 *
 * Space Complexity: O(1)
 *
 * When to Use Rabin-Karp:
 * -----------------------
 * ✅ Multiple pattern matching (compute hash once per pattern)
 * ✅ 2D pattern matching (images, grids)
 * ✅ Plagiarism detection (find similar documents)
 * ✅ Good for average case when hash collisions are rare
 *
 * ❌ NOT ideal for: Single pattern match (KMP is better)
 *
 * vs. KMP:
 * - Rabin-Karp: Simple, good for multiple patterns, uses hashing
 * - KMP: Faster worst-case, no hash collisions, uses prefix function
 */

#define PRIME 101  // Large prime for modulo
#define BASE 256   // Number of characters in ASCII

/**
 * Rabin-Karp pattern matching
 *
 * @param text The text to search in
 * @param pattern The pattern to search for
 * @return Number of matches found
 */
int rabin_karp(const char* text, const char* pattern, bool verbose) {
    int n = strlen(text);
    int m = strlen(pattern);

    if (m > n) return 0;

    if (verbose) {
        printf("\n=== Rabin-Karp Algorithm ===\n");
        printf("Text:    '%s' (length %d)\n", text, n);
        printf("Pattern: '%s' (length %d)\n\n", pattern, m);
    }

    // Calculate d^(m-1) % PRIME for rolling hash
    int h = 1;
    for (int i = 0; i < m - 1; i++) {
        h = (h * BASE) % PRIME;
    }

    if (verbose) {
        printf("d^(m-1) mod q = %d^%d mod %d = %d\n\n", BASE, m-1, PRIME, h);
    }

    // Calculate hash of pattern and first window
    int pattern_hash = 0;
    int text_hash = 0;

    for (int i = 0; i < m; i++) {
        pattern_hash = (BASE * pattern_hash + pattern[i]) % PRIME;
        text_hash = (BASE * text_hash + text[i]) % PRIME;
    }

    if (verbose) {
        printf("Pattern hash: %d\n", pattern_hash);
        printf("Initial window '%.*s' hash: %d\n\n", m, text, text_hash);
        printf("Sliding window:\n");
    }

    int matches = 0;

    // Slide pattern over text
    for (int i = 0; i <= n - m; i++) {
        if (verbose) {
            printf("Position %2d: '%.*s' (hash %3d) ", i, m, &text[i], text_hash);
        }

        // If hashes match, verify with character comparison
        if (pattern_hash == text_hash) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                matches++;
                if (verbose) {
                    printf("✓ MATCH!\n");
                }
            } else {
                if (verbose) {
                    printf("✗ Hash collision (false positive)\n");
                }
            }
        } else {
            if (verbose) {
                printf("\n");
            }
        }

        // Calculate hash for next window (rolling hash)
        if (i < n - m) {
            // Remove leading digit, add trailing digit
            text_hash = (BASE * (text_hash - text[i] * h) + text[i + m]) % PRIME;

            // Handle negative hash
            if (text_hash < 0) {
                text_hash += PRIME;
            }
        }
    }

    if (verbose) {
        printf("\nTotal matches: %d\n", matches);
    }

    return matches;
}

// ============================================================
// KNUTH-MORRIS-PRATT (KMP) ALGORITHM
// ============================================================

/**
 * Knuth-Morris-Pratt (KMP) String Matching Algorithm
 *
 * What is KMP?
 * ------------
 * An efficient string matching algorithm that NEVER backtracks in the text.
 * Key insight: Use information from previous comparisons to skip redundant checks.
 *
 * Core Concept: LPS (Longest Proper Prefix which is also Suffix)
 * ---------------------------------------------------------------
 * For pattern "ABABC":
 *   lps[0] = 0  (A: no proper prefix/suffix)
 *   lps[1] = 0  (AB: no match)
 *   lps[2] = 1  (ABA: "A" is both prefix and suffix)
 *   lps[3] = 2  (ABAB: "AB" is both prefix and suffix)
 *   lps[4] = 0  (ABABC: no match)
 *
 * Why LPS Matters:
 * ----------------
 * When mismatch occurs at position i:
 * - Don't restart from beginning
 * - Skip to position lps[i-1] in pattern
 * - Continue from there
 *
 * Example:
 *   Text:    A B A B A B C A
 *   Pattern: A B A B C
 *
 *   Mismatch at position 4 (B vs C):
 *   - lps[3] = 2, so we know first 2 chars match
 *   - Skip to position 2 in pattern, continue from there
 *   - No need to recheck characters already matched!
 *
 * Algorithm Steps:
 * ----------------
 * 1. PREPROCESS: Compute LPS array for pattern
 *    - Time: O(m)
 * 2. SEARCH: Scan through text once
 *    - When match: advance both pointers
 *    - When mismatch: use LPS to skip in pattern
 *    - Time: O(n)
 *
 * Total Time: O(n + m)
 * Space: O(m) for LPS array
 *
 * Why KMP Never Backtracks:
 * -------------------------
 * - Text pointer ALWAYS moves forward
 * - Pattern pointer may move backward (via LPS)
 * - But we never re-examine text characters
 *
 * When to Use KMP:
 * ----------------
 * ✅ Single pattern matching in long text
 * ✅ Real-time text processing (streaming)
 * ✅ When worst-case O(n+m) guarantee needed
 * ✅ DNA sequence matching
 * ✅ Text editors (Ctrl+F)
 *
 * vs. Rabin-Karp:
 * - KMP: Guaranteed O(n+m), no hash collisions, more complex
 * - Rabin-Karp: Simple, good for multiple patterns, average O(n+m)
 *
 * vs. Boyer-Moore:
 * - KMP: Scans left-to-right, works well on all inputs
 * - Boyer-Moore: Can skip large portions, but complex preprocessing
 */

/**
 * Compute LPS (Longest Proper Prefix which is also Suffix) array
 *
 * The LPS array tells us: "If we have a mismatch at position i,
 * how many characters can we skip?"
 *
 * Process:
 * 1. lps[0] = 0 (base case)
 * 2. For each position i:
 *    - Try to extend previous LPS
 *    - If can't extend, fall back to smaller LPS
 *    - Repeat until match or reach beginning
 */
void compute_lps(const char* pattern, int m, int* lps) {
    int len = 0;  // Length of previous longest prefix suffix
    lps[0] = 0;   // lps[0] is always 0
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            // Characters match, extend LPS
            len++;
            lps[i] = len;
            i++;
        } else {
            // Mismatch
            if (len != 0) {
                // Try smaller LPS
                len = lps[len - 1];
                // Don't increment i, try same char again
            } else {
                // No LPS possible
                lps[i] = 0;
                i++;
            }
        }
    }
}

/**
 * KMP pattern matching
 *
 * @param text The text to search in
 * @param pattern The pattern to search for
 * @return Number of matches found
 */
int kmp_search(const char* text, const char* pattern, bool verbose) {
    int n = strlen(text);
    int m = strlen(pattern);

    if (m > n) return 0;

    if (verbose) {
        printf("\n=== Knuth-Morris-Pratt (KMP) Algorithm ===\n");
        printf("Text:    '%s' (length %d)\n", text, n);
        printf("Pattern: '%s' (length %d)\n\n", pattern, m);
    }

    // Step 1: Compute LPS array
    int* lps = (int*)malloc(m * sizeof(int));
    compute_lps(pattern, m, lps);

    if (verbose) {
        printf("LPS Array (Longest Proper Prefix which is also Suffix):\n");
        printf("Pattern: ");
        for (int i = 0; i < m; i++) {
            printf("%c ", pattern[i]);
        }
        printf("\nLPS:     ");
        for (int i = 0; i < m; i++) {
            printf("%d ", lps[i]);
        }
        printf("\n\n");

        printf("Matching process:\n");
    }

    // Step 2: Search for pattern
    int matches = 0;
    int i = 0;  // Index for text
    int j = 0;  // Index for pattern

    while (i < n) {
        if (verbose && j == 0) {
            printf("Position %2d: ", i);
        }

        if (pattern[j] == text[i]) {
            // Characters match
            if (verbose) {
                printf("%c", text[i]);
            }
            i++;
            j++;
        }

        if (j == m) {
            // Found pattern
            matches++;
            if (verbose) {
                printf(" ✓ MATCH at position %d\n", i - j);
            }
            j = lps[j - 1];  // Continue searching for overlapping patterns
        } else if (i < n && pattern[j] != text[i]) {
            // Mismatch after j matches
            if (j != 0) {
                if (verbose) {
                    printf(" (mismatch, skip to lps[%d]=%d)\n", j-1, lps[j-1]);
                }
                j = lps[j - 1];  // Use LPS to skip
            } else {
                if (verbose) {
                    printf("\n");
                }
                i++;  // No match at all, move to next character
            }
        }
    }

    if (verbose) {
        printf("\nTotal matches: %d\n", matches);
    }

    free(lps);
    return matches;
}

// ============================================================
// BOYER-MOORE ALGORITHM
// ============================================================

/**
 * Boyer-Moore Algorithm - Advanced Pattern Matching
 *
 * What is Boyer-Moore?
 * -------------------
 * An efficient string matching algorithm that scans from RIGHT to LEFT
 * in the pattern, but moves LEFT to RIGHT in the text. Often faster
 * than KMP in practice because it can skip large sections of text.
 *
 * Key Insight: Scan pattern from right to left
 * --------------------------------------------
 * When a mismatch occurs, we know more about what didn't match,
 * allowing bigger jumps forward.
 *
 * Example:
 *   Text:    "HERE IS A SIMPLE EXAMPLE"
 *   Pattern: "EXAMPLE"
 *
 *   Step 1: Align pattern at start
 *           HERE IS A SIMPLE EXAMPLE
 *           EXAMPLE
 *                  ^ Start comparison from right
 *           'E' vs ' ' - mismatch!
 *           ' ' not in pattern → skip entire pattern length
 *
 *   Step 2: Jump forward 7 positions
 *           HERE IS A SIMPLE EXAMPLE
 *                  EXAMPLE
 *                        ^ Compare from right
 *
 * Two Heuristics (We implement Bad Character only for simplicity):
 * ----------------------------------------------------------------
 * 1. Bad Character Rule:
 *    When mismatch occurs, align pattern so mismatched text character
 *    matches its rightmost occurrence in pattern.
 *    If character not in pattern, skip past it entirely.
 *
 * 2. Good Suffix Rule: (not implemented here for simplicity)
 *    When partial match found, align next occurrence of matched suffix.
 *
 * Bad Character Table:
 * -------------------
 * Preprocessing: For each character in alphabet, store its rightmost
 * position in pattern (or -1 if not present).
 *
 * Pattern: "EXAMPLE"
 * Index:    0123456
 *
 * bad_char['E'] = 6 (rightmost E)
 * bad_char['X'] = 1
 * bad_char['A'] = 2
 * bad_char['M'] = 3
 * bad_char['P'] = 4
 * bad_char['L'] = 5
 * bad_char['Z'] = -1 (not in pattern)
 *
 * Algorithm Steps:
 * ---------------
 * 1. Preprocess: Build bad character table - O(m + σ) where σ = alphabet size
 * 2. Align pattern at text start
 * 3. Compare from RIGHT to LEFT of pattern
 * 4. On mismatch:
 *    - If bad char exists in pattern: align to rightmost occurrence
 *    - Else: skip entire pattern
 * 5. On match: Record position, shift pattern by 1 (or use good suffix rule)
 *
 * Why Boyer-Moore is Often Faster:
 * --------------------------------
 * - Scans right-to-left: mismatches found quickly
 * - Skips large sections: can shift by m positions
 * - Best case: O(n/m) - sublinear! (e.g., searching for "ZZZZZ" in English text)
 * - Average case: O(n/m) to O(n)
 * - Worst case: O(n*m) - when many false matches
 *
 * When to Use Boyer-Moore:
 * -----------------------
 * ✅ Long patterns (m > 5)
 * ✅ Large alphabet (English text, DNA with large k-mers)
 * ✅ Pattern unlikely to match (few false positives)
 * ✅ Real-world text search (grep, text editors)
 *
 * ❌ Short patterns (overhead not worth it)
 * ❌ Small alphabet with many matches (DNA: AAAA...)
 * ❌ Streaming data (needs random access to text)
 *
 * Comparison with KMP:
 * -------------------
 * KMP:
 *   - Scans left-to-right
 *   - Never backtracks in text (good for streams)
 *   - O(n+m) guaranteed
 *   - Better for small alphabets
 *
 * Boyer-Moore:
 *   - Scans right-to-left
 *   - Can skip sections (may "backtrack" start position)
 *   - O(n/m) to O(n*m)
 *   - Better for large alphabets and long patterns
 */

#define CHAR_MAX 256  // ASCII character set

/**
 * Preprocess Bad Character Table
 *
 * For each character in the alphabet, stores the rightmost position
 * where it occurs in the pattern (or -1 if it doesn't occur).
 *
 * Example: pattern = "EXAMPLE"
 *   bad_char['E'] = 6, bad_char['X'] = 1, bad_char['A'] = 2
 *   bad_char['Z'] = -1 (not in pattern)
 *
 * Time: O(m + σ) where σ = alphabet size
 */
void compute_bad_char(const char* pattern, int m, int bad_char[CHAR_MAX]) {
    // Initialize all occurrences as -1
    for (int i = 0; i < CHAR_MAX; i++) {
        bad_char[i] = -1;
    }

    // Fill actual positions (rightmost occurrence)
    for (int i = 0; i < m; i++) {
        bad_char[(unsigned char)pattern[i]] = i;
    }
}

/**
 * Boyer-Moore Search
 *
 * Searches for all occurrences of pattern in text using Bad Character rule.
 *
 * Returns: number of matches found
 *
 * Time Complexity:
 *   Best: O(n/m) - can skip m characters at a time
 *   Average: O(n) - typically much better than naive
 *   Worst: O(n*m) - many partial matches (rare in practice)
 *
 * Space: O(σ) for bad character table
 */
int boyer_moore_search(const char* text, const char* pattern, bool verbose) {
    int n = strlen(text);
    int m = strlen(pattern);
    int matches = 0;

    if (m == 0 || m > n) {
        return 0;
    }

    // Preprocess: compute bad character table
    int bad_char[CHAR_MAX];
    compute_bad_char(pattern, m, bad_char);

    if (verbose) {
        printf("\nBad Character Table (showing non-empty entries):\n");
        for (int i = 0; i < CHAR_MAX; i++) {
            if (bad_char[i] != -1 && i >= 32 && i < 127) {
                printf("  '%c' -> %d\n", (char)i, bad_char[i]);
            }
        }
        printf("\n");
    }

    // Search: align pattern and scan right-to-left
    int shift = 0;  // Pattern position in text
    while (shift <= (n - m)) {
        int j = m - 1;  // Start from rightmost character of pattern

        if (verbose) {
            printf("Shift %d: ", shift);
            for (int k = 0; k < n; k++) {
                if (k == shift + j) {
                    printf("[%c]", text[k]);
                } else {
                    printf("%c", text[k]);
                }
            }
            printf("\n          ");
            for (int k = 0; k < shift; k++) printf(" ");
            for (int k = 0; k < m; k++) {
                if (k == j) {
                    printf("[%c]", pattern[k]);
                } else {
                    printf("%c", pattern[k]);
                }
            }
            printf("\n");
        }

        // Compare pattern from right to left
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }

        if (j < 0) {
            // Match found!
            matches++;
            if (verbose) {
                printf("  ✓ Match at position %d\n\n", shift);
            }
            // Shift pattern past the match
            // Could use good suffix rule here for better performance
            shift += (shift + m < n) ? m - bad_char[(unsigned char)text[shift + m]] : 1;
        } else {
            // Mismatch at position j
            char bad = text[shift + j];
            int bad_pos = bad_char[(unsigned char)bad];

            if (verbose) {
                printf("  Mismatch: pattern[%d]='%c' vs text[%d]='%c'\n",
                       j, pattern[j], shift + j, bad);
                if (bad_pos == -1) {
                    printf("  '%c' not in pattern → skip %d positions\n\n",
                           bad, j + 1);
                } else {
                    printf("  '%c' last seen at pattern[%d] → skip %d positions\n\n",
                           bad, bad_pos, j - bad_pos);
                }
            }

            // Shift pattern to align bad character
            // Maximum of 1 (don't go backwards) and bad character rule
            int skip = j - bad_pos;
            shift += (skip > 0) ? skip : 1;
        }
    }

    if (verbose) {
        printf("Total matches: %d\n", matches);
    }

    return matches;
}

// ============================================================
// LEVENSHTEIN DISTANCE (EDIT DISTANCE)
// ============================================================

/**
 * Levenshtein Distance - String Similarity Measure
 *
 * What is Edit Distance?
 * ---------------------
 * The minimum number of single-character edits (insertions, deletions,
 * or substitutions) required to change one string into another.
 *
 * Named after Vladimir Levenshtein (1965).
 *
 * Examples:
 * --------
 * "kitten" → "sitting"
 *   k → s  (substitute)    1 edit
 *   e → i  (substitute)    2 edits
 *   insert t               3 edits
 *   Distance = 3
 *
 * "saturday" → "sunday"
 *   delete a               1 edit
 *   delete t               2 edits
 *   u → n (substitute)     3 edits
 *   Distance = 3
 *
 * "book" → "back"
 *   o → a (substitute)     1 edit
 *   o → c (substitute)     2 edits
 *   Distance = 2
 *
 * Dynamic Programming Approach:
 * -----------------------------
 * Build a table dp[i][j] = edit distance between first i characters
 * of string1 and first j characters of string2.
 *
 * Base Cases:
 *   dp[0][j] = j  (insert j characters)
 *   dp[i][0] = i  (delete i characters)
 *
 * Recurrence:
 *   If str1[i-1] == str2[j-1]:
 *     dp[i][j] = dp[i-1][j-1]  (no edit needed)
 *   Else:
 *     dp[i][j] = 1 + min(
 *       dp[i-1][j],     // delete from str1
 *       dp[i][j-1],     // insert into str1
 *       dp[i-1][j-1]    // substitute
 *     )
 *
 * Example Table: "kitten" → "sitting"
 *
 *       ""  s  i  t  t  i  n  g
 *   ""   0  1  2  3  4  5  6  7
 *   k    1  1  2  3  4  5  6  7
 *   i    2  2  1  2  3  4  5  6
 *   t    3  3  2  1  2  3  4  5
 *   t    4  4  3  2  1  2  3  4
 *   e    5  5  4  3  2  2  3  4
 *   n    6  6  5  4  3  3  2  3
 *
 * Result: dp[6][7] = 3
 *
 * Applications:
 * ------------
 * ✅ Spell checking (find closest dictionary word)
 * ✅ DNA sequence alignment (bioinformatics)
 * ✅ Fuzzy string matching (autocorrect, search)
 * ✅ Diff tools (version control systems)
 * ✅ Plagiarism detection (similarity measure)
 * ✅ Speech recognition (match phonetic similarity)
 * ✅ Record linkage (match database records)
 *
 * Complexity:
 * ----------
 * Time: O(m*n) where m, n are string lengths
 * Space: O(m*n) for full table
 *        O(min(m,n)) if only storing two rows (space optimization)
 *
 * Variants:
 * --------
 * - Damerau-Levenshtein: Also allows transpositions (ab → ba = 1 edit)
 * - Hamming Distance: Only substitutions, equal length strings
 * - Longest Common Subsequence: Related DP problem
 *
 * When to Use:
 * -----------
 * ✅ Fuzzy matching needed
 * ✅ Measuring string similarity
 * ✅ Strings of different lengths
 * ✅ Need to know actual distance
 *
 * ❌ Exact matching (use KMP/Boyer-Moore)
 * ❌ Very long strings (becomes expensive)
 * ❌ Only care about equality (use strcmp)
 */

/**
 * Helper function: minimum of three integers
 */
int min3(int a, int b, int c) {
    int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

/**
 * Compute Levenshtein Distance
 *
 * Returns the minimum number of edits to transform str1 into str2.
 *
 * If show_table is true, prints the DP table for visualization.
 *
 * Time: O(m*n)
 * Space: O(m*n)
 */
int levenshtein_distance(const char* str1, const char* str2, bool show_table) {
    int m = strlen(str1);
    int n = strlen(str2);

    // Allocate DP table
    int** dp = (int**)malloc((m + 1) * sizeof(int*));
    for (int i = 0; i <= m; i++) {
        dp[i] = (int*)malloc((n + 1) * sizeof(int));
    }

    // Base cases: empty string transformations
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;  // Delete all characters from str1
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;  // Insert all characters of str2
    }

    // Fill DP table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i-1] == str2[j-1]) {
                // Characters match, no edit needed
                dp[i][j] = dp[i-1][j-1];
            } else {
                // Take minimum of three operations
                dp[i][j] = 1 + min3(
                    dp[i-1][j],      // Delete from str1
                    dp[i][j-1],      // Insert into str1
                    dp[i-1][j-1]     // Substitute
                );
            }
        }
    }

    // Print table if requested
    if (show_table) {
        printf("\nLevenshtein Distance DP Table:\n");
        printf("(Transforming \"%s\" → \"%s\")\n\n", str1, str2);

        // Print header
        printf("      \"\"  ");
        for (int j = 0; j < n; j++) {
            printf(" %c ", str2[j]);
        }
        printf("\n");

        // Print rows
        for (int i = 0; i <= m; i++) {
            if (i == 0) {
                printf("  \"\" ");
            } else {
                printf("  %c  ", str1[i-1]);
            }

            for (int j = 0; j <= n; j++) {
                printf("%2d ", dp[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    int result = dp[m][n];

    // Free memory
    for (int i = 0; i <= m; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
}

/**
 * Space-Optimized Levenshtein Distance
 *
 * Only keeps two rows of the DP table at a time.
 * Same result, but uses O(min(m,n)) space instead of O(m*n).
 *
 * Useful when strings are very long and you only need the distance
 * (not the actual edit sequence).
 */
int levenshtein_distance_optimized(const char* str1, const char* str2) {
    int m = strlen(str1);
    int n = strlen(str2);

    // Ensure n is the smaller dimension
    if (m < n) {
        const char* temp = str1;
        str1 = str2;
        str2 = temp;
        int t = m;
        m = n;
        n = t;
    }

    // Only need two rows
    int* prev_row = (int*)malloc((n + 1) * sizeof(int));
    int* curr_row = (int*)malloc((n + 1) * sizeof(int));

    // Initialize first row
    for (int j = 0; j <= n; j++) {
        prev_row[j] = j;
    }

    // Fill rows one at a time
    for (int i = 1; i <= m; i++) {
        curr_row[0] = i;

        for (int j = 1; j <= n; j++) {
            if (str1[i-1] == str2[j-1]) {
                curr_row[j] = prev_row[j-1];
            } else {
                curr_row[j] = 1 + min3(
                    prev_row[j],      // Delete
                    curr_row[j-1],    // Insert
                    prev_row[j-1]     // Substitute
                );
            }
        }

        // Swap rows
        int* temp = prev_row;
        prev_row = curr_row;
        curr_row = temp;
    }

    int result = prev_row[n];
    free(prev_row);
    free(curr_row);

    return result;
}

// ============================================================
// TEST FUNCTIONS
// ============================================================

void test_trie() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║              Test 1: Trie (Prefix Tree)          ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    Trie* trie = trie_create();

    printf("=== Building Dictionary ===\n");
    const char* words[] = {"cat", "car", "card", "care", "dog", "dodge", "door"};
    int num_words = sizeof(words) / sizeof(words[0]);

    for (int i = 0; i < num_words; i++) {
        trie_insert(trie, words[i]);
    }

    printf("\n=== Search Operations ===\n");
    const char* search_words[] = {"cat", "card", "can", "door", "do"};
    for (int i = 0; i < 5; i++) {
        bool found = trie_search(trie, search_words[i]);
        printf("Search '%s': %s\n", search_words[i], found ? "FOUND" : "NOT FOUND");
    }

    printf("\n=== Prefix Search ===\n");
    const char* prefixes[] = {"ca", "do", "ca", "x"};
    for (int i = 0; i < 4; i++) {
        bool has_prefix = trie_starts_with(trie, prefixes[i]);
        printf("Prefix '%s': %s\n", prefixes[i],
               has_prefix ? "EXISTS" : "DOESN'T EXIST");
    }

    printf("\n=== Autocomplete ===\n");
    trie_autocomplete(trie, "car");
    printf("\n");
    trie_autocomplete(trie, "do");

    trie_destroy(trie);
}

void test_rabin_karp() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║         Test 2: Rabin-Karp Algorithm             ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");

    const char* text = "ABABCABABA";
    const char* pattern = "ABA";

    rabin_karp(text, pattern, true);

    printf("\n--- Testing with longer text ---\n");
    const char* text2 = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG";
    const char* pattern2 = "THE";
    rabin_karp(text2, pattern2, true);
}

void test_kmp() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║    Test 3: Knuth-Morris-Pratt (KMP) Algorithm    ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");

    const char* text = "ABABCABABA";
    const char* pattern = "ABA";

    kmp_search(text, pattern, true);

    printf("\n--- Testing with longer pattern ---\n");
    const char* text2 = "AABAACAADAABAABA";
    const char* pattern2 = "AABA";
    kmp_search(text2, pattern2, true);
}

void test_comparison() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║  Test 4: Performance Comparison (RK vs KMP)      ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    const char* text = "AAAAAAAAAAAAAAAAAAAAAB";
    const char* pattern = "AAAAB";

    printf("Text:    '%s'\n", text);
    printf("Pattern: '%s'\n\n", pattern);

    printf("Rabin-Karp:\n");
    int rk_matches = rabin_karp(text, pattern, false);

    printf("\nKMP:\n");
    int kmp_matches = kmp_search(text, pattern, false);

    printf("\nResults:\n");
    printf("  Rabin-Karp found: %d matches\n", rk_matches);
    printf("  KMP found:        %d matches\n", kmp_matches);
    printf("  ✓ Both algorithms agree!\n");
}

void test_boyer_moore() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║          Test 5: Boyer-Moore Algorithm           ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    // Test 1: Simple pattern matching
    printf("=== Test 1: Simple Pattern ===\n");
    const char* text1 = "HERE IS A SIMPLE EXAMPLE";
    const char* pattern1 = "EXAMPLE";
    printf("Text:    '%s'\n", text1);
    printf("Pattern: '%s'\n", pattern1);
    boyer_moore_search(text1, pattern1, true);

    // Test 2: Multiple matches
    printf("\n=== Test 2: Multiple Matches ===\n");
    const char* text2 = "AABAACAADAABAABA";
    const char* pattern2 = "AABA";
    printf("Text:    '%s'\n", text2);
    printf("Pattern: '%s'\n", pattern2);
    boyer_moore_search(text2, pattern2, true);

    // Test 3: No match (shows large jumps)
    printf("\n=== Test 3: No Match (Large Jumps) ===\n");
    const char* text3 = "ABCDEFGHIJKLMNOP";
    const char* pattern3 = "XYZ";
    printf("Text:    '%s'\n", text3);
    printf("Pattern: '%s'\n", pattern3);
    boyer_moore_search(text3, pattern3, true);

    // Test 4: Real-world example
    printf("\n=== Test 4: Real-World Text ===\n");
    const char* text4 = "The quick brown fox jumps over the lazy dog. The dog was very lazy.";
    const char* pattern4 = "lazy";
    printf("Text:    '%s'\n", text4);
    printf("Pattern: '%s'\n", pattern4);
    printf("\n");
    int matches4 = boyer_moore_search(text4, pattern4, false);
    printf("Found %d occurrences of '%s'\n", matches4, pattern4);

    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║  Key Insight: Boyer-Moore scans RIGHT-TO-LEFT    ║\n");
    printf("║  and can skip large sections when mismatches     ║\n");
    printf("║  occur. Often faster than KMP on large alphabets.║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
}

void test_levenshtein() {
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║       Test 6: Levenshtein Distance (Edit)        ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    // Test 1: Classic example
    printf("=== Test 1: kitten → sitting ===\n");
    const char* str1_1 = "kitten";
    const char* str2_1 = "sitting";
    int dist1 = levenshtein_distance(str1_1, str2_1, true);
    printf("Distance: %d\n", dist1);
    printf("Operations: k→s, e→i, insert t\n");

    // Test 2: Another classic
    printf("\n=== Test 2: saturday → sunday ===\n");
    const char* str1_2 = "saturday";
    const char* str2_2 = "sunday";
    int dist2 = levenshtein_distance(str1_2, str2_2, true);
    printf("Distance: %d\n", dist2);

    // Test 3: Spell checking scenario
    printf("\n=== Test 3: Spell Checking (Finding Closest Match) ===\n");
    const char* typo = "recieve";  // Common typo
    const char* dictionary[] = {"receive", "deceive", "relieve", "believe", "achieve"};
    int num_words = sizeof(dictionary) / sizeof(dictionary[0]);

    printf("Typo: '%s'\n\n", typo);
    printf("Finding closest dictionary match:\n");

    int min_dist = 999;
    const char* closest = NULL;

    for (int i = 0; i < num_words; i++) {
        int dist = levenshtein_distance_optimized(typo, dictionary[i]);
        printf("  '%s' → distance = %d", dictionary[i], dist);
        if (dist < min_dist) {
            min_dist = dist;
            closest = dictionary[i];
            printf(" ✓ (new best)");
        }
        printf("\n");
    }

    printf("\nSuggestion: Did you mean '%s'? (distance: %d)\n", closest, min_dist);

    // Test 4: String similarity
    printf("\n=== Test 4: String Similarity ===\n");
    const char* pairs[][2] = {
        {"book", "back"},
        {"hello", "hallo"},
        {"algorithm", "altruistic"},
        {"", "abc"},
        {"xyz", ""}
    };
    int num_pairs = sizeof(pairs) / sizeof(pairs[0]);

    for (int i = 0; i < num_pairs; i++) {
        int dist = levenshtein_distance_optimized(pairs[i][0], pairs[i][1]);
        printf("'%s' ↔ '%s' = %d edits\n", pairs[i][0], pairs[i][1], dist);
    }

    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║  Applications: Spell check, fuzzy search, DNA    ║\n");
    printf("║  alignment, diff tools, plagiarism detection     ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
}

// ============================================================
// MAIN
// ============================================================

int main() {
    char choice;

    while (1) {
        printf("\n╔═══════════════════════════════════════════════════╗\n");
        printf("║          STRING ALGORITHMS - Menu                 ║\n");
        printf("╚═══════════════════════════════════════════════════╝\n\n");

        printf("1. Trie (Prefix Tree)\n");
        printf("2. Rabin-Karp Algorithm\n");
        printf("3. Knuth-Morris-Pratt (KMP)\n");
        printf("4. Performance Comparison (RK vs KMP)\n");
        printf("5. Boyer-Moore Algorithm\n");
        printf("6. Levenshtein Distance (Edit Distance)\n");
        printf("\nx. Exit\n");
        printf("\nEnter choice: ");
        scanf(" %c", &choice);

        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            test_trie();
        } else if (choice == '2') {
            test_rabin_karp();
        } else if (choice == '3') {
            test_kmp();
        } else if (choice == '4') {
            test_comparison();
        } else if (choice == '5') {
            test_boyer_moore();
        } else if (choice == '6') {
            test_levenshtein();
        } else {
            printf("Invalid choice\n");
        }
    }

    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║              Algorithm Summary                    ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    printf("Trie (Prefix Tree):\n");
    printf("  Insert/Search: O(m)  |  Space: O(N*M*ALPHABET_SIZE)\n");
    printf("  Best for: Autocomplete, prefix search, dictionaries\n\n");

    printf("Rabin-Karp:\n");
    printf("  Time: O(n+m) avg, O(n*m) worst  |  Space: O(1)\n");
    printf("  Best for: Multiple patterns, 2D matching, plagiarism detection\n\n");

    printf("Knuth-Morris-Pratt (KMP):\n");
    printf("  Time: O(n+m) guaranteed  |  Space: O(m)\n");
    printf("  Best for: Single pattern, real-time text, DNA sequences\n\n");

    printf("Boyer-Moore:\n");
    printf("  Time: O(n/m) best, O(n) avg, O(n*m) worst  |  Space: O(σ)\n");
    printf("  Best for: Long patterns, large alphabets, text editors\n\n");

    printf("Levenshtein Distance:\n");
    printf("  Time: O(m*n)  |  Space: O(m*n) or O(min(m,n)) optimized\n");
    printf("  Best for: Spell checking, fuzzy search, DNA alignment\n\n");

    return 0;
}
