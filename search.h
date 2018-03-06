extern char t[T_LEN];
extern char s[S_SIZE][S_LEN+2];

int searchPart(int n, char* part);
void generatePart(char* part, int pos, int len);
int editDistance(char *x, char *y);
