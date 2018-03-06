#include <stdio.h>
#include "stream.h"
#include "global.h"

extern char tp[T_LEN];
extern char s[S_SIZE][S_LEN+2];

// データ入力処理
void inputData(FILE *fp) {
  int i;
  // T'入力
  fgets(tp, T_LEN, fp);
  // S入力
  for(i=0; i<S_SIZE; i++) {
    fgets(s[i], S_LEN+2, fp);
  }
}
