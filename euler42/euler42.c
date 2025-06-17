/*
The n`th term of the sequence of triangle numbers is given by, Tn=n*(n+1)/2,
so the first ten triangle numbers are: 1, 3, 6, 10, 15, 21, 28, 36, 45, 55...

By converting each letter in a word to a number corresponding to its alphabetical
position and adding these values we form a word value. For example, the word value for
 SKY is 19 + 11 + 25 = 55 = T10. 
If the word value is a triangle number then we shall call the word a triangle word.
Using "words.txt" (right click and 'Save Link/Target As...'), a 16K text file containing
nearly two-thousand common English words, how many are triangle words?

Solution:
1) Iterative approach where Tn is generated using the Tn formula above and if Tn is equal
 input value, than we know that word is a triangle
2) Square root calculation, and if the positive solution is a real number that Input value
 is a triangle. n(1,2) = (-1 + sqrt(1 + 8*Tn))/2. Since we are looking for a positive
 real number we go with the +sqrt().. option only
*/

#include <stdio.h>
#include <math.h>

#define A 0x40		// set to 0x40 instead of 0x41 to normalize 'A' to 1 instead of 0

/*
 Read next word from file f, return number of chars in word. return 0 on end of file, -1 on error.
 Expect text input file, with words surrounded with "" and separated with ,
*/
int get_word (FILE *f, char *word, int max) {

	int n = 0;
	int p = 0;
	char c;

	while (n < max && !feof(f)) {
		if ((c = getc(f)) != EOF) {
			switch (c) {
				case '"':
					if (p == 0) {
						p++;
						continue;
					}
					else
						return n;
				case ',':
					continue;
				default:
					word[n++] = c;
			}
		}
		else if (ferror(f))
			return -1;
	}

	return n;
}

/*
 return the value of a given word
*/
int word_value (char *word, int n) {
	int i, v = 0;

	for (i = 0; i < n; i++)
		v += word[i] - A;

	return v;
}

/*
	return 1 if input value is a triangle number or 0 otherwie
*/
/* iterative approach
int is_triangle (int v) {
	int n, t;

	for (n = 1, t = 0; t < v; n++)
		t = n*(n+1)/2;

	return (t == v? 1: 0);
}
*/
/*
 calculation approach. return 1 if the result of the square root calculation is a
 real number. otherwise, return 0
*/
int is_triangle (int v) {
	float d;
	int i;

	d = (-1 + sqrt(1.0 + 8.0*v))/2.0;
	i = d;

	return (i == d);
}

int main () {
	FILE *fp;
	char word [80];
	int n, v, tc = 0;

	if ((fp = fopen("words.txt", "r")) == NULL) {
		printf("Failed to open file \"words.txt\"\n");
		return 1;
	}

	while ((n = get_word(fp, word, 79)) > 0) {
		v = word_value(word, n);
		if (is_triangle(v)) tc++;
	}

	if (n < 0) {
		printf("Error reading file\n");
	}

	printf("total %d triangle words\n", tc);

	fclose(fp);

	return 0;
}