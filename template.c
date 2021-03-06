#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "global.h"
#include "bitap.h"

int main_prg(int, char**);

/* sは文字列, lはT'では長さ, Sでは使用済みフラグとして利用 */
typedef struct {
  char* s;
  int l;
} Datum;

char tmpT[T_LEN+1], tmpS[S_SIZE][S_LEN+1], tmpR[T_LEN+1];
Datum Tp, S[S_SIZE], R;

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


int main_prg(int argc, char** argv){
  /** implement here  **/
  int i, j, cp;

  fscanf(stdin, "%s", tmpT);
  Tp.s = tmpT;
  Tp.l = strlen(tmpT);

  for(i = 0; i < S_SIZE; i++) {
    fscanf(stdin, "%s", tmpS[i]);
    S[i].s = tmpS[i];
    S[i].l = 0;
  }

  /* T'をコピー */
  R.s = tmpR;
  for(i = 0; i < Tp.l; i++) R.s[i] = Tp.s[i];

  /* bitapによるアラインメント */
  for(i = 0; i < S_SIZE; i++) {
    cp = bitap_search(R.s, S[i].s, 1);
    if(cp == -1) cp = bitap_search(R.s, S[i].s, 2);
    if(cp == -1) cp = bitap_search(R.s, S[i].s, 3);
    if(cp != -1) {
      S[i].l = 1; // 使用したSはフラグを立てる
      for(j = 0; j < S_LEN; j++) {
        R.s[cp+j] = S[i].s[j];
        Tp.s[cp+j] = 'x';
      }
    }
  }

  /* bitapによるアラインメント */
  for(i = 0; i < S_SIZE; i++) {
    if(S[i].l == 1) continue; // 使用済みSはスキップ
    cp = bitap_search(Tp.s, S[i].s, 4);
    if(cp != -1) {
      S[i].l = 1;
      for(j = 0; j < S_LEN; j++) {
        R.s[cp+j] = S[i].s[j];
        Tp.s[cp+j] = 'x';
      }
    }
  }

  fprintf(stdout, "%s\n", R.s);

  return 0;
}
