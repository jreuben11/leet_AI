#include <stdio.h>

int factorial(int n) {
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main() {
    char choice;
    int n;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Factorial\n");
        printf("x. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            printf("Enter a positive integer: ");
            scanf("%d", &n);
            if (n < 0) {
                printf("Error: input must be non-negative\n");
            } else {
                printf("Factorial of %d is %d\n", n, factorial(n));
            }
        }
    }
    return 0;
}