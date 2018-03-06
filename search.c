#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "global.h"
#include "search.h"

int dp[N+1][N+1];

/* 2つの文字列から編集距離がED_LIMIT以下なら1を返す関数 */
/* 前期アルゴリ編集距離part2のやつ流用(改良の余地しかない) */
int editDistance(char *x, char *y) {
    int i, j, xl, yl, m, finish;
    // 入力文字数
    xl = strlen(x);
    yl = strlen(y);
    // 文字列長の差がN以上ならば計算する必要なし
    if(abs(xl-yl) >= ED_LIMIT) return ED_LIMIT;

    // 漸化式に基づいて計算
    for(i=0; i<=xl; i++) dp[i][0] = i;
    for(j=0; j<=yl; j++) dp[0][j] = j;

    for(i=1; i<=xl; i++) {
      finish = 1;
      // ED_LIMIT行目までは幅 i+ED_LIMIT まで計算
      if(i <= ED_LIMIT) {
        for(j=1; j<=i+ED_LIMIT; j++) {
          m = (x[i] == y[j]) ? 0 : 1;
          // 行の先頭では左を参照しない
          if(j == i+ED_LIMIT) dp[i][j] = MIN(dp[i-1][j-1] + m, dp[i][j-1] + 1);
          else if(j < i+ED_LIMIT) dp[i][j] = MIN3(dp[i-1][j-1] + m, dp[i-1][j] + 1, dp[i][j-1] + 1);
          if(dp[i][j] < ED_LIMIT) finish = 0;
        }
      // 1ED_LIMIT+1行目以降は対角成分±ED_LIMITの幅で計算
      } else {
        for(j=i-ED_LIMIT; j<=i+ED_LIMIT; j++) {
          m = (x[i] == y[j]) ? 0 : 1;
          if(j>i-ED_LIMIT && j<i+ED_LIMIT) dp[i][j] = MIN3(dp[i-1][j-1] + m, dp[i-1][j] + 1, dp[i][j-1] + 1);
          // 行の先頭では左を参照しない
          else if(j == i-ED_LIMIT) dp[i][j] = MIN(dp[i-1][j-1] + m, dp[i-1][j] + 1);
          // 行の末尾では上を参照しない
          else if(j == i+ED_LIMIT) dp[i][j] = MIN(dp[i-1][j-1] + m, dp[i][j-1] + 1);
          if(dp[i][j] < ED_LIMIT) finish = 0;
        }
      }
      // 途中でED_LIMITを超えるようであれば計算打ち切り
      if(finish) {
        dp[xl][yl] = ED_LIMIT;
        break;
      }
    }
    return dp[xl][yl];
}

/**
 * @fn
 * @brief Sから部分文字列を検索する(完全一致検索のため未使用)
 * @param (int n) Sの番号(0~n~19999)
 * @param (char* part) 検索する部分文字列
 * @return 発見すればその位置、なければ-1
 */
int searchPart(int n, char* part) {
  char* adr = strstr(s[n], part);
  if(adr != NULL) {
    return (int)(adr - s[n]);
  } else {
    return -1;
  }
}

/**
 * @fn
 * @brief T'から部分文字列を生成する
 * @param (char* part) 部分文字列を格納する箇所のポインタ
 * @param (int pos) T'から部分文字列を取り出す先頭位置
 * @param (int len) 部分文字列の長さ
 */
void generatePart(char* part, int pos, int len) {
   strncpy(part, tp+pos, len);
   part[len] = '\0';
}
