#include <stdio.h>

int factorial(int n) {
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// given an array check wether array is no sorted order wit hrecursion

int isSortedArray(int arr[], int n) {
    if (n == 1) {
        return 1;
    }
    return isSortedArray(arr, n - 1) && arr[n - 1] >= arr[n - 2];
}

// generateBitStrings: Generates all $2^n$ binary strings of length n.
// Logic:
// 1. Set $n$-th bit A[n-1] to 0. (Backtracking step: ensure clean state)
// 2. Generate all $(n-1)$-bit strings.
// 3. Set $n$-th bit A[n-1] to 1.
// 4. Generate all $(n-1)$-bit strings again.
void generateBitStrings(int A[], int n, int original_n) {
    if (n == 0) {
        for (int i = 0; i < original_n; i++) {
            printf("%d", A[i]);
        }
        printf(" "); // Separate strings with a space
        return;
    }
    A[n - 1] = 0;
    generateBitStrings(A, n - 1, original_n);
    A[n - 1] = 1;
    generateBitStrings(A, n - 1, original_n);
}

// generate all the strings of length n drawn from 0 to k-1
void generateKStrings(int A[], int n, int k, int original_n) {
    if (n == 0) {
        for (int i = 0; i < original_n; i++) {
            printf("%d", A[i]);
        }
        printf(" "); // Separate strings with a space
        return;
    }
    for (int i = 0; i < k; i++) {
        A[n - 1] = i;
        generateKStrings(A, n - 1, k, original_n);
    }
}   

int main() {
    char choice;
    int n;
    int arr[100];
    int i;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Factorial\n");
        printf("2. Fibonacci\n");
        printf("3. isSortedArray\n");
        printf("4. Generate Bit Strings\n");
        printf("5. Generate K-ary Strings\n");
        printf("x. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'x') {
            break;
        } else if (choice == '1') { // factorial
            printf("Enter a positive integer: ");
            scanf("%d", &n);
            if (n < 0) {
                printf("Error: input must be non-negative\n");
            } else {
                printf("Factorial of %d is %d\n", n, factorial(n));
            }
        } else if (choice == '2') { // fibonacci    
            printf("Enter the number of terms: ");
            scanf("%d", &n);
            if (n < 0) {
                 printf("Error: input must be non-negative\n");
            } else {
                printf("Fibonacci Series: ");
                for (i = 0; i < n; i++) {
                    printf("%d ", fibonacci(i));
                }
                printf("\n");
            }
        } else if (choice == '3') { // isSortedArray
            printf("Enter integers separated by space (max 100): ");
            n = 0;
            // scanf parses the integer but leaves the delimiter (space/newline) in the buffer.
            // getchar() then consumes this delimiter to check if we've reached the newline.
            while (n < 100) {
                if (scanf("%d", &arr[n]) != 1) break;
                n++;
                char ch = getchar();
                if (ch == '\n') break;
            }

            if (n == 0) {
                 printf("Error: no input provided\n");
            } else {
                if (isSortedArray(arr, n)) {
                    printf("Array is sorted\n");
                } else {
                    printf("Array is NOT sorted\n");
                }
            }
        } else if (choice == '4') { // generateBitStrings
            printf("Enter n: ");
            scanf("%d", &n);
            if (n <= 0 || n > 100) {
                 printf("Error: n must be between 1 and 100\n");
            } else {
                // Initialize array to 0 since generateStrings logic depends on setting 1s
                for(i=0; i<n; i++) arr[i] = 0;
                generateBitStrings(arr, n, n);
                printf("\n");
            }
        } else if (choice == '5') { // generateKStrings
            int k;
            printf("Enter length n: ");
            scanf("%d", &n);
             printf("Enter k: ");
            scanf("%d", &k);
            if (n <= 0 || n > 100) {
                 printf("Error: n must be between 1 and 100\n");
            } else if (k <= 0) {
                 printf("Error: k must be positive\n");
            } else {
                 generateKStrings(arr, n, k, n);
                 printf("\n");
            }
        }
    }
    return 0;
}