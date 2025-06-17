#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

// Function to swap two characters in an array
// Function to swap two characters in an array
void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

/*
 Recursive function to generate all permutations of digits.
 Once a permutation is form, cal_mns() is called with the new permutation
 sent as one of its arguments
*/
int permute(char *a, int l, int r) {
    int i, n = 0;

    if (l == r) {
        printf("%s\n", a);
    }
    else {
        for (i = l; i <= r; i++) {
            swap((a + l), (a + i));
            n += permute(a, l + 1, r);
            swap((a + l), (a + i)); // backtrack
        }
    }

    return n;
}

int main() {
    char digits[9] = "0023";
    // char digits[] = "012";
    // int n = sizeof(digits) / sizeof(digits[0]) - 1; // Calculate the number of digits
    int n = 4;

    printf("Generating all unique 10-digit numbers:\n");
    permute(digits, 0, n - 1); // Call the permutation function (n-1 because array is 0-indexed)

    return 0;
}