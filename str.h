#ifndef __INCLUDE_STR__
#define __INCLUDE_STR__

#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char* s; //文字列
  int head; //先頭
  int tail; //末尾
  int length; //長さ
} String;

void str_output(String*);
String* str_generate(String*, char*, int, int);
String* substring(String*, String*, int, int);
String* str_copy(String*, String*);
double str_cmp(String*, String*);

#endif
