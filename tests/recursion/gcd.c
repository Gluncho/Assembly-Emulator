int gcd(int a, int b){
    if(b==0) return a;
    return gcd(b, a%b);
}

int main(){
    return gcd(178, 200);
}