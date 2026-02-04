#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========================================
// 0/1 Knapsack Problem (Dynamic Programming)
// ========================================

// Structure to represent an item
struct Item {
    int weight;
    int value;
};

// Solve 0/1 Knapsack problem using dynamic programming
// Returns the maximum value that can be obtained
int knapsack(struct Item items[], int n, int capacity) {
    // Create DP table: dp[i][w] = max value with first i items and capacity w
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)calloc(capacity + 1, sizeof(int));
    }

    // Build DP table bottom-up
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            // Current item: items[i-1] (0-indexed)
            int item_weight = items[i - 1].weight;
            int item_value = items[i - 1].value;

            // If item can fit in current capacity
            if (item_weight <= w) {
                // Take maximum of:
                // 1. Not taking the item: dp[i-1][w]
                // 2. Taking the item: item_value + dp[i-1][w-item_weight]
                int with_item = item_value + dp[i - 1][w - item_weight];
                int without_item = dp[i - 1][w];
                dp[i][w] = (with_item > without_item) ? with_item : without_item;
            } else {
                // Item doesn't fit, carry forward previous value
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int max_value = dp[n][capacity];

    // Free DP table
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return max_value;
}

// Print the DP table for visualization
void print_knapsack_table(struct Item items[], int n, int capacity) {
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)calloc(capacity + 1, sizeof(int));
    }

    // Build DP table
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            int item_weight = items[i - 1].weight;
            int item_value = items[i - 1].value;

            if (item_weight <= w) {
                int with_item = item_value + dp[i - 1][w - item_weight];
                int without_item = dp[i - 1][w];
                dp[i][w] = (with_item > without_item) ? with_item : without_item;
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Print table
    printf("\nDP Table (rows=items, cols=capacity):\n");
    printf("    ");
    for (int w = 0; w <= capacity; w++) {
        printf("%3d ", w);
    }
    printf("\n");

    for (int i = 0; i <= n; i++) {
        if (i == 0) {
            printf("  0:");
        } else {
            printf("  %d:", i);
        }
        for (int w = 0; w <= capacity; w++) {
            printf("%3d ", dp[i][w]);
        }
        printf("\n");
    }

    // Free DP table
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);
}

// Test function for Knapsack problem
void test_knapsack() {
    printf("=== 0/1 Knapsack Problem (Dynamic Programming) ===\n\n");

    // Example: Classic knapsack problem
    struct Item items[] = {
        {2, 12},  // Item 0: weight=2, value=12
        {1, 10},  // Item 1: weight=1, value=10
        {3, 20},  // Item 2: weight=3, value=20
        {2, 15}   // Item 3: weight=2, value=15
    };
    int n = 4;
    int capacity = 5;

    printf("Items:\n");
    for (int i = 0; i < n; i++) {
        printf("  Item %d: weight=%d, value=%d\n", i, items[i].weight, items[i].value);
    }
    printf("\nKnapsack capacity: %d\n", capacity);

    int max_value = knapsack(items, n, capacity);
    printf("\nMaximum value: %d\n", max_value);

    print_knapsack_table(items, n, capacity);

    printf("\n--- How Dynamic Programming Works ---\n");
    printf("dp[i][w] = max value using first i items with capacity w\n\n");
    printf("For each item, we have two choices:\n");
    printf("1. Don't take it: dp[i][w] = dp[i-1][w]\n");
    printf("2. Take it (if it fits): dp[i][w] = value[i] + dp[i-1][w-weight[i]]\n\n");
    printf("We take the maximum of these two options.\n");
    printf("Time Complexity: O(n*capacity)\n");
    printf("Space Complexity: O(n*capacity)\n");
}

// ========================================
// N-Queens Problem (Backtracking)
// ========================================

// Check if placing a queen at (row, col) is safe
int is_safe(int** board, int n, int row, int col) {
    // Check column above
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 1) {
            return 0;
        }
    }

    // Check upper left diagonal
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1) {
            return 0;
        }
    }

    // Check upper right diagonal
    for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 1) {
            return 0;
        }
    }

    return 1;
}

// Print the board
void print_board(int** board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                printf("Q ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Recursive backtracking to solve N-Queens
int solve_n_queens_util(int** board, int n, int row, int* solution_count, int max_solutions) {
    // Base case: all queens placed
    if (row == n) {
        (*solution_count)++;
        printf("Solution %d:\n", *solution_count);
        print_board(board, n);

        if (*solution_count >= max_solutions) {
            return 1; // Stop after finding max_solutions
        }
        return 0;
    }

    // Try placing queen in each column of current row
    for (int col = 0; col < n; col++) {
        if (is_safe(board, n, row, col)) {
            // Place queen
            board[row][col] = 1;

            // Recurse to place rest of queens
            if (solve_n_queens_util(board, n, row + 1, solution_count, max_solutions)) {
                return 1;
            }

            // Backtrack: remove queen
            board[row][col] = 0;
        }
    }

    return 0;
}

// Solve N-Queens problem
void solve_n_queens(int n, int max_solutions) {
    // Allocate board
    int** board = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        board[i] = (int*)calloc(n, sizeof(int));
    }

    int solution_count = 0;
    solve_n_queens_util(board, n, 0, &solution_count, max_solutions);

    if (solution_count == 0) {
        printf("No solution exists for N=%d\n", n);
    } else {
        printf("Total solutions shown: %d\n", solution_count);
    }

    // Free board
    for (int i = 0; i < n; i++) {
        free(board[i]);
    }
    free(board);
}

// Test function for N-Queens problem
void test_n_queens() {
    printf("=== N-Queens Problem (Backtracking) ===\n\n");

    printf("The N-Queens problem: Place N chess queens on an N×N chessboard\n");
    printf("so that no two queens threaten each other.\n\n");

    // Test with 4-Queens (shows 2 solutions)
    printf("Testing 4-Queens problem:\n");
    printf("------------------------\n");
    solve_n_queens(4, 2);

    printf("\n--- How Backtracking Works ---\n");
    printf("1. Place queen in first available safe position in current row\n");
    printf("2. Move to next row and repeat\n");
    printf("3. If no safe position exists, backtrack to previous row\n");
    printf("4. Try next position in that row\n");
    printf("5. Continue until all queens are placed or all options exhausted\n\n");
    printf("Time Complexity: O(N!)\n");
    printf("Space Complexity: O(N²)\n");
}

// ========================================
// Main Menu
// ========================================

int main() {
    char choice;

    while (1) {
        printf("\n=== Dynamic Programming & Backtracking Menu ===\n");
        printf("1. 0/1 Knapsack Problem (DP)\n");
        printf("2. N-Queens Problem (Backtracking)\n");
        printf("x. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            test_knapsack();
        } else if (choice == '2') {
            test_n_queens();
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
