#include "global.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  String Tp;
  String S[S_SIZE];
  String T;
} Data;


Data* streaming_data(Data*, char*, char*);
Data* input_data(Data*, FILE*);
// Data* read_Data_answer(Data*, FILE*);
// Data* sort_Data_fragments(Data*);
// Data* destroy_Data(Data*);
