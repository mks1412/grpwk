#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "global.h"
#include "bitap.h"
#define PRM 15

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


int main_prg(int argc, char** argv){
  /** implement here  **/
  int i, j, k, cp = -1, cnt = 0, align_num = 0, offset;
  char pattern[S_LEN];
  char part[1001];
  FILE* ifp;
  FILE* ofp;
  highScoreCell = &zero;

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
    S[i].l = 0;
  }
  fclose(ifp);

  R.s = tmpR;
  for(i = 0; i < Tp.l; i++) {
    if(Tp.s[i] == 'a') cntTp.a++;
    else if(Tp.s[i] == 'b') cntTp.b++;
    else cntTp.c++;
    R.s[i] = 'x';
  }

  /* S同士の類似検索 */
  // for(j = 0; j < S_LEN-PRM+1; j++) {
  // for(j = 0; j < 1; j++) {
  //   strncpy(pattern, S[0].s + j, PRM);
  //   for(i = 0; i < S_SIZE; i++) {
  //     if(i == 0) continue;
  //     cp = bitap_search(S[i].s, pattern, 0);
  //     if(cp != -1) {
  //       printf("%6d: %s\n", i, S[i].s);
  //       if(align_num == 0) {
  //         localAlignment(S[0].s, S[i].s, part);
  //       } else {
  //         localAlignment(part, S[i].s, part);
  //       }
  //       align_num++;
  //       printf("%s\n", part);
  //     }
  //   }
  // }

  /* bitapによるアラインメント */
  for(i = 0; i < S_SIZE; i++) {
    if(S[i].l == 1) continue;
    cp = bitap_search(Tp.s, S[i].s, 0);
    if(cp != -1) {
      S[i].l = 1;
      // printf("S[%d] = T'[%d]\n", i, cp);
      /* Rにマッピング */
      for(j = 0; j < S_LEN; j++) {
        if(R.s[cp+j] == 'x') {
          R.s[cp+j] = S[i].s[j];
          Tp.s[cp+j] = 'x';
        }
      }
    }
  }


  FILE* tmpfp = fopen("dat/dat0_tmp", "w");
  fprintf(tmpfp, "%s\n", R.s);
  fclose(tmpfp);

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

  printf("R contents\n");
  printf("a: %5d\n", cntR.a);
  printf("b: %5d\n", cntR.b);
  printf("c: %5d\n", cntR.c);
  printf("x: %5d\n", cnt);
  printf("Match: %.3f％\n", (Tp.l-cnt)/(double)Tp.l * 100);
  return 0;
}
