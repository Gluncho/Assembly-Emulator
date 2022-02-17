int reverseNum(int n){
    int res = 0;
    while(n > 0){
        res = 10*res + n%10;
        n = n/10;
    }
    return res;
}

int main(){
    int a = reverseNum(1898);
    int b = reverseNum(a);
    if(b == 1898) return 0;
    return 1;
}