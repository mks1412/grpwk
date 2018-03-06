#include "data.h"


Data* streaming_data(Data* data, char* input_fn, char* output_fn) {
  FILE* fp;

  if(input_fn != NULL) {
    if((fp = fopen(input_fn, "r")) == NULL) {
      fprintf(stderr, "failed to open file \"%s\", in read_data.", input_fn);
      exit(1);
    }
    input_data(data, fp);
    fclose(fp);
  }

  // if(output_fn != NULL) {
  //   if((fp = fopen(output_fn, "r")) == NULL) {
  //     fprintf(stderr, "failed to open file \"%s\", in read_data.", output_fn);
  //     exit(1);
  //   }
  //   read_data_answer(data, fp);
  //   fclose(fp);
  // }

  return data;
}

Data* input_data(Data* data, FILE* fp) {
  int i;
  char tmpT[T_LEN+1], tmpS[S_LEN+1];

  fscanf(fp, "%s", tmpT);
  str_generate(&data->Tp, tmpT, 0, strlen(tmpT));

  for(i=0; i<S_SIZE; i++) {
    fscanf(fp, "%s", tmpS);
    str_generate(&data->S[i], tmpS, 0, strlen(tmpS));
  }
  printf("%s\n", data->S[1].s);


  return data;
}
