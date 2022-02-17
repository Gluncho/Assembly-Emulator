#include <assert.h>
int min(int a, int b){
    if(a > b) return b;
    return a;
}

int minarr(int* arr, int len){
    if(len == 1) return arr[0];
    return min(arr[len-1], minarr(arr,len-1));
}

int main(){
    int a[5];
    a[0] = -2001;
    a[1] = -2000;
    a[2] = -1999;
    a[3] = 979;
    a[4] = 1000;
    return minarr(a, 5);
}