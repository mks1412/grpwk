#include <stdio.h>
#include <string.h>
#define N 105000

int offset, delta;

int diff(char* a, char* b, int rev) {
    int len1 = strlen(a);
    int len2 = strlen(b);
    // len1 <= len2でなければひっくり返す
    if (rev == 0 && len1 > len2)
        return diff(b, a, true);
    // 変数宣言及び配列初期化
    int k, p;
    offset = len1 + 1;
    delta = len2 - len1;
    int fp[N], ed[N];

    for (p=0; p<len1+len2+3; p++) {
        fp[p] = -1;
        ed[p] = 0;
    }
    // メインの処理
    for (p=0; fp[delta + offset] != len2; p++) {
        for(k = -p; k < delta; k++) snake(k, fp, ed);
        for(k = delta + p; k >= delta; k--) snake(k, fp, ed);
    }
    return ed[delta + offset];
}

// snake
void snake(int k, int* fp, int* ed) {
    int x, y, e0, o, y1=fp[k-1+offset], y2=fp[k+1+offset];
    if(y1 >= y2) { // 経路選択
        y = y1+1;
        x = y-k;
        e0 = ed[k-1+offset];
        o = {edit:rev?'-':'+',arr:arr2, line:y-1}
    } else {
        y = y2;
        x = y-k;
        e0 = ed[k+1+offset];
        o = {edit:rev?'+':'-',arr:arr1, line:x-1}
    }
    // 選択した経路を保存
    if (o.line>=0) ed[k+offset] = e0.concat(o);

    var max = len1-x>len2-y?len1-x:len2-y;
    for (var i=0; i<max && arr1[x+i]===arr2[y+i]; ++i) {
        // 経路追加
        ed[k+offset].push({edit:'=', arr:arr1, line:x+i});
    }
    fp[k + offset] = y+i;
}
