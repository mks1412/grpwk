#include "str.h"

/* 文字列を標準出力 */
void str_output(String* string) {
  int i; for(i = string->head; i < string->tail; i++) printf("%c", string->s[i]);
}

/* String型文字列生成 */
String* str_generate(String* string, char* s, int h, int t) {
  string->s = s;
  string->head = h;
  string->tail = t;
  string->length = t - h;
  return string;
}

/* 先頭と長さを指定してsから切り出す */
String* substring(String* ret, String* s, int h, int l) {
  return str_generate(ret, s->s, s->head + h, s->head + h + l);
}

String* str_copy(String* d, String* s) {
  int i;
  for(i = 0; i < s->length; i++) d->s[d->head + i] = s->s[s->head + i];
  return d;
}

double str_cmp(String* a, String* b) {
  int i, cnt, length;
  double r;
  if((length = a->length) != b->length) {
    return 0.0;
  }
  for(cnt = 0, i = 0; i < length; i++) {
    if(a->s[a->head + i] != 'x' && b->s[b->head + i] != 'x') {
      if(a->s[a->head + i] == b->s[b->head + i]) {
        cnt++;
      } else {
        return 0.0;
      }
    }
  }
  r = (double) cnt / length;
  return r;
}
