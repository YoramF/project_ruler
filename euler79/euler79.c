/*
solution assumes that the passcode does not contain same digit more than once

Passcode: 73162890

real    0m0.204s
user    0m0.045s
sys     0m0.046s
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define KEYS 50
#define DIGITS 10

int set_r_str (char keys[KEYS][4], char passcode[], int *passcode_l) {
    int digits [DIGITS] = {0,0,0,0,0,0,0,0,0,0};
    int i, j;
    int d;
    char digit;
    int added;

    // mark all digits which are not first in key
    for (i = 0; i < KEYS; i++) {
        if (keys[i][1] > 0) {
            digits[keys[i][1]-'0'] = 1;
        }
        if (keys[i][2] > 0) {
            digits[keys[i][2]-'0'] = 1;
        }       
    }

    d = 0;
    digit = 0;
    // now add all digits not in digits[] into passcode
    for (int i = 0; i < 10; i++) {
        if (digits[i] == 0) {
            digit = i + '0';
            added = 0;
            // reduce all keys with first digit == i
            for (j = 0; j < KEYS; j++) {
                if (keys[j][0] == digit) {
                    if (added == 0) {
                        d++;
                        passcode[(*passcode_l)++] = digit;
                        added++;
                    }

                    keys[j][0] = keys[j][1];
                    keys[j][1] = keys[j][2];
                    keys[j][2] = keys[j][3];
                }
            }
        }
    }

    return d;
}

int main () {
    FILE *f;
    char passcode [11] = "";
    int passcode_l = 0;
    char key [10];          // to make sure we read whole line
    int pc_ind;
    char keys_a [KEYS][4];
    int keys;

    if ((f = fopen("./keylog.txt", "r")) == NULL) {
        fprintf(stderr, "Failed to open file keylog.txt: %s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }

    keys = 0;
    while (!feof(f) && (fgets(key, 10, f) != NULL) && keys < KEYS) {
        strncpy(keys_a[keys], key, 3);
        keys++;
    }


    while (set_r_str(keys_a, passcode, &passcode_l) > 0) {};
    passcode[passcode_l] = '\0';

    printf("Passcode: %s\n", passcode);

    fclose(f);
}
