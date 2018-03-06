#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MATCH 1
#define SPACE -2
#define MISMATCH -1

typedef struct cell {
  int i;
  int j;
  int score;
  struct cell *from;
} Cell, *PCell;

PCell highScoreCell;
Cell zero = {0, 0, 0, NULL};

void fillInCell(char* a, char* b, PCell current, PCell above, PCell left, PCell aboveLeft) {
  int rowScore = above->score + SPACE;
  int colScore = left->score + SPACE;
  int matchOrMismatchScore = aboveLeft->score;
  if (b[current->i - 1] == a[current->j - 1]) {
     matchOrMismatchScore += MATCH;
  } else {
     matchOrMismatchScore += MISMATCH;
  }
  if (rowScore >= colScore) {
     if (matchOrMismatchScore >= rowScore) {
        if (matchOrMismatchScore > 0) {
           current->score = matchOrMismatchScore;
           current->from = aboveLeft;
        }
     } else {
        if (rowScore > 0) {
           current->score = rowScore;
           current->from = above;
        }
     }
  } else {
     if (matchOrMismatchScore >= colScore) {
        if (matchOrMismatchScore > 0) {
           current->score = matchOrMismatchScore;
           current->from = aboveLeft;
        }
     } else {
        if (colScore > 0) {
           current->score = colScore;
           current->from = left;
        }
     }
  }
  if (current->score > highScoreCell->score) {
     highScoreCell = current;
  }
}


int localAlignment(char* a, char* b, char* x) {
  int i, j, offset = 0;
  int n = strlen(a) + 1;
  int m = strlen(b) + 1;
  PCell current;
  Cell** dp = malloc(sizeof(PCell) * n);
  PCell row = malloc(sizeof(Cell) * n * m);
  for (i=0; i<n; i++) dp[i] = row + i * m;
  /* DP行列初期化 */
  for(i=0; i<n; i++) {
    dp[i][0].score = 0;
    dp[i][0].from = NULL;
    for (j=0; j<m; j++) {
      dp[0][j].score = 0;
      dp[0][j].from = NULL;
      dp[i][j].i = i;
      dp[i][j].j = j;
    }
  }

  for (i=1; i<n; i++) {
    for (j=1; j<m; j++) {
      fillInCell(a, b, &dp[i][j], &dp[i-1][j], &dp[i][j-1], &dp[i-1][j-1]);
    }
  }

  for (i=0; i<n; i++) {
    for (j=0; j<m; j++) {
      printf("%3d", dp[i][j].score);
    }
    printf("\n");
  }
  // char sa[60];
  // char sb[60];
  // int ida = 0, idb = 0;
  // current = highScoreCell;
  //
  // while (current->score != 0) {
  //   if (current->i - current->from->i == 1) {
  //     sa[ida++] = b[current->i - 1];
  //   } else {
  //     sa[ida++] = '-';
  //   }
  //   if (current->j - current->from->j == 1) {
  //     sb[idb++] = a[current->j - 1];
  //   } else {
  //     sb[idb++] = '-';
  //   }
  //   current = current->from;
  // }
  //
  // sa[ida] = '\0'; sb[idb] = '\0';
  // offset = current->i - current->j;
  //
  // printf("highScoreCell is (%d, %d)\n", highScoreCell->i, highScoreCell->j);
  // printf("high score is %d\n", highScoreCell->score);
  // printf("%d\n", offset);
  // for(i=strlen(sa)-1; i>=0; i--) printf("%c", sa[i]);
  // printf("\n");
  // for(i=strlen(sb)-1; i>=0; i--) printf("%c", sb[i]);
  // printf("\n");
  //
  // if(x != NULL) {
  //   if(offset > 0) {
  //     for(i = 0; i < offset; i++) x[i] = b[i];
  //     for(i = 0; i < strlen(a); i++) x[i+offset] = a[i];
  //   } else {
  //     offset *= -1;
  //     for(i = 0; i < offset; i++) x[i] = a[i];
  //     for(i = 0; i < strlen(b); i++) x[i+offset] = b[i];
  //   }
  //   x[strlen(a) + offset] = '\0';
  // }
  free(row);
  free(dp);
  return offset > 0 ? offset : -offset;
}


int main(void) {
  char S1[] = "cbaacabbaaaacccbcbaabcaba";
  char S2[] = "caaabccaccccbaacabbaaaacc";
  char X[100];

  int offset = localAlignment(S1, S2, X);
  printf("%s\n", X);
  return 0;
}
