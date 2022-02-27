int lsearch(char** strings, int num_strings, char* target){
    for(int i = 0; i < num_strings; i++){
        if(strcmp(target, strings[i]) == 0){
            return i;
        }
    }
    return -1;
}

int main(){
    char** strings = {"a", "b", "ab", "cd"};
    printf(lsearch(strings, 4, strings[2]));
}


