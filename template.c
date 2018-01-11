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
  int i, j, cp = -1, cnt = 0;
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
    S[i].l = 0;
  }
  fclose(ifp);

  R.s = tmpR;

  /* bitapによるアラインメント */
  for(i = 0; i < S_SIZE; i++) {
    if(S[i].l == 1) continue;
    cp = bitap_search(R.s, S[i].s, 1);
    if(cp == -1) cp = bitap_search(R.s, S[i].s, 2);
    if(cp == -1) cp = bitap_search(R.s, S[i].s, 3);
    if(cp != -1) {
      S[i].l = 1;
      for(j = 0; j < S_LEN; j++) {
        R.s[cp+j] = S[i].s[j];
        Tp.s[cp+j] = 'x';
      }
    }
  }

  /* bitapによるアラインメント */
  for(i = 0; i < S_SIZE; i++) {
    if(S[i].l == 1) continue;
    cp = bitap_search(Tp.s, S[i].s, 4);
    if(cp != -1) {
      S[i].l = 1;
      for(j = 0; j < S_LEN; j++) {
        R.s[cp+j] = S[i].s[j];
        Tp.s[cp+j] = 'x';
      }
    }
  }

  if((ofp = fopen(argv[2], "w")) == NULL) {
    fprintf(stderr, "failed to open file \"%s\", in read_data.", argv[2]);
    exit(1);
  }
  fprintf(ofp, "%s\n", R.s);
  fclose(ofp);

  return 0;
}
