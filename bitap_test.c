#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int SearchString(const char *text, const char *pattern, int k)
{
	int result = -1;
	int m = strlen(pattern);
	unsigned long *R;
	unsigned long patternMask[CHAR_MAX + 1];
	int i, d;

	if (pattern[0] == '\0') return 0;
	if (m > 31) return -1; //Error: The pattern is too long!

	R = (unsigned long*)malloc((k + 1) * sizeof *R);
	for (i = 0; i <= k; ++i)
		R[i] = ~1;

	for (i = 0; i <= CHAR_MAX; ++i)
		patternMask[i] = ~0;

	for (i = 0; i < m; ++i)
		patternMask[pattern[i]] &= ~(1UL << i);

	for (i = 0; text[i] != '\0'; ++i)
	{
		unsigned long oldRd1 = R[0];

		R[0] |= patternMask[text[i]];
		R[0] <<= 1;

		for (d = 1; d <= k; ++d)
		{
			unsigned long tmp = R[d];

			R[d] = (oldRd1 & (R[d] | patternMask[text[i]])) << 1;
			oldRd1 = tmp;
		}

		if (0 == (R[k] & (1UL << m)))
		{
			result = (i - m) + 1;
			break;
		}
	}

	free(R);
	return result;
}

int main(void) {
  int i;
  FILE* fp;
  char text[100000];
	char* t;
  if((fp = fopen("dat/dat0_test", "r")) == NULL) {
    fprintf(stderr, "failed to open file.");
    exit(1);
  }
  fscanf(fp, "%s", text);
  fclose(fp);
  char pattern[] = "cbaacabbaaaacc";
	t = text;

	while(1) {
  	int r = SearchString(t, pattern, 2);
		t = t + r + 14;
		if(r == -1) break;
		printf("%d\n", r+14);
	}
}
