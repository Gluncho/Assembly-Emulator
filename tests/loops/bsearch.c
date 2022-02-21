int bsearch(int* arr, int len, int elem){
    int l = 0;
    int r = len-1;
    while(l <= r){
        int mid = (l+r)/2;
        if(arr[mid] == elem) return mid;
        if(arr[mid] < elem){
            l = mid + 1;
        }else r = mid - 1;
    }
    return -1;
}

int main() {
    int a[6] = {10, 20, 30 , 40 , 50, 60};
    int index = bsearch(a, 6, 30);
    printf(index);
}
