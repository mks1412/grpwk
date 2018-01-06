#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "global.h"
#define BM_TABLE_SIZE 256
#define PRM 7

int main_prg(int, char**);

typedef struct {
  char* s;
  int l;
} Datum;

typedef struct {
  int a;
  int b;
  int c;
} Counter;

char tmpT[T_LEN+1], tmpS[S_SIZE][S_LEN+1], tmpR[T_LEN+1];
Datum Tp, S[S_SIZE], R;
Counter cntTp = {0,0,0}, cntR = {0,0,0};


int main(int argc, char** argv){

  struct rusage u;
  struct timeval start, end;

  getrusage(RUSAGE_SELF, &u);
  start = u.ru_utime;

  main_prg(argc, argv);

  getrusage(RUSAGE_SELF, &u );
  end = u.ru_utime;

  fprintf(stderr, "%lf\n", (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)*1.0E-6);

  return(0);
}

int bitap_search(const char *text, const char *pattern, int k)
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


/*!
 * @brief         ずらし表（照合再開位置テーブル）を作成する
 * @param[in/out] table    テーブルへのアドレス
 * @param[in]     pattern  探索文字列
 * @param[in]     pat_len  パターン文字列長
 */
static void
bm_table_init(int *table, const char *pattern, int ptn_len)
{
    int cnt = 0;

    /* パターンに無い文字はパターン文字列長をずらし幅にする */
    for(cnt = 0; cnt < BM_TABLE_SIZE; cnt++){
	table[cnt] = ptn_len;
    }

    /* パターンに含まれる文字のずらし幅を決定する */
    for(cnt = 0; cnt < ptn_len; cnt++){
	table[(int)pattern[cnt]] = ptn_len - cnt - 1;
    }

    /* デバッグ出力 */
    // printf("[table]  : default: step=%d\n", ptn_len);
    // for(cnt = 0; cnt < BM_TABLE_SIZE; cnt++){
    //     if(table[cnt] != ptn_len)
    //         printf("         : char=%c: table[%03d]: step=%d\n",
    //                (char)cnt,cnt,(int)table[cnt]);
    // }
}

/*!
 * @brief      パターンを進める加算値を求める
 * @param[in]  table  ずらし表
 * @param[in]  target 比較点の文字
 * @param[in]  remain パターンの未比較部分の文字列長
 * @return     パターンを進める加算値。
 */
static int
next_step(int *table, char target, int remain)
{
    /* ループ防止のために確認する */
    if(table[(int)target] > remain){
        /* ずらし表から値を取得する */
        return(table[(int)target]);
    }else{
        /* 照合を開始した地点の次の文字に進める */
        return(remain);
    }
}

/*!
 * @brief      文字列を探索する
 * @param[in]  text     検索対象文字列
 * @param[in]  pattern  検索文字列
 * @return     発見位置のポインタを返す。失敗したらNULLを返す。
 */
int bm_search(const char *text, const char *pattern)
{
    int table[BM_TABLE_SIZE];
    int txt_len = 0;
    int ptn_len = 0;
    int i = 0; /* テキストの比較位置 */
    int j = 0; /* パターンの比較位置 */

    ptn_len = strlen(pattern);
    txt_len = strlen(text);

    /* ずらし表を作成する */
    bm_table_init(table, pattern, ptn_len);

    /* 比較処理 */
    i = j = ptn_len - 1; /* 比較位置をパターン末尾にする */
    while((i < txt_len) && (j >= 0)){
        // print_compare_process(text, pattern, i, j);

        if(text[i] != pattern[j]){
            /* ずらし表を参照して、次の比較位置を設定する */
            i += next_step(table, text[i], (ptn_len - j));
            j = ptn_len - 1;   /* 比較位置をパターン末尾にする */
        }else{
            /* 文字が一致したので、前の文字を照合していく */
            j--;
            i--;
        }
    }

    if(j < 0) return(i + 1);
    return -1;
}

int main_prg(int argc, char** argv){
  /** implement here  **/
  int i, j, k, cp = -1, cnt = 0;
  char pattern[S_LEN];
  FILE* ifp;
  FILE* ofp;

  if(argc != 3) {
    printf("argument error\n");
    printf("$ ./grpwk <input_filename> <output_filename>\n");
    exit(1);
  }

  if((ifp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "failed to open file \"%s\", in read_data.", argv[1]);
    exit(1);
  }
  fscanf(ifp, "%s", tmpT);
  Tp.s = tmpT;
  Tp.l = strlen(tmpT);
  for(i = 0; i < S_SIZE; i++) {
    fscanf(ifp, "%s", tmpS[i]);
    S[i].s = tmpS[i];
    S[i].l = S_LEN;
  }
  fclose(ifp);

  R.s = tmpR;
  for(i = 0; i < Tp.l; i++) {
    if(Tp.s[i] == 'a') cntTp.a++;
    else if(Tp.s[i] == 'b') cntTp.b++;
    else cntTp.c++;
    R.s[i] = 'x';
  }
  for(i = 0; i < S_SIZE; i++) {
    /* bitapによるマッピング */
    cp = bitap_search(Tp.s, S[i].s, 5);
    if(cp != -1) {
      for(j = 0; j < S_LEN; j++) {
        R.s[cp + j] = S[i].s[j];
        Tp.s[cp + j] = 'x';
      }
    }
  }
  cp = -1;
  for(i = 0; i < S_SIZE; i++) {
    /* BM法によるマッピング */
    for(j = 0; j < S_LEN-PRM; j++) {
      strncpy(pattern, S[i].s + j, PRM);
      cp = bm_search(Tp.s, pattern);
      if(cp == -1) continue;
      for(k = 0; k < PRM; k++) {
        R.s[cp + k] = pattern[k];
        Tp.s[cp + k] = 'x';
      }
      break;
     }
    // if(i % 1000 == 0) printf("%3d％\n", i/200);
  }

  printf("%s\n", R.s);

  for(i = 0; i < Tp.l; i++) {
    if(R.s[i] == 'a') cntR.a++;
    else if(R.s[i] == 'b') cntR.b++;
    else if(R.s[i] == 'c') cntR.c++;
    else {
      cnt++;
      R.s[i] = Tp.s[i];
    }
  }

  if((ofp = fopen(argv[2], "w")) == NULL) {
    fprintf(stderr, "failed to open file \"%s\", in read_data.", argv[2]);
    exit(1);
  }
  fprintf(ofp, "%s\n", R.s);
  fclose(ofp);

  printf("length of T' is %d\n", Tp.l);
  printf("T' contents\n");
  printf("a: %5d\n", cntTp.a);
  printf("b: %5d\n", cntTp.b);
  printf("c: %5d\n", cntTp.c);
  printf("R contents\n");
  printf("a: %5d\n", cntR.a);
  printf("b: %5d\n", cntR.b);
  printf("c: %5d\n", cntR.c);
  printf("x: %5d\n", cnt);
  printf("Match: %.3f％\n", (Tp.l-cnt)/(double)Tp.l * 100);
  return 0;
}
