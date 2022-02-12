#include "stringhelper.h"

string trim(string line, char delim){
    int start = 0, end = line.size()-1;
    while(start < line.size() && line[start]==delim) start++;
    while(end >= 0  && line[end]==delim) end--;
    if(start > end) return "";
    return line.substr(start, end - start + 1);
}

string remove_spaces(string str){
    string ans = "";
    for(int i = 0; i < str.size(); i++){
        if(str[i] != ' ') ans += str[i];
    }
    return ans;
}

string* split_by(string str, char splitter){
    string* ans = NULL;
    for(int i = 1; i < str.length() - 1; i++){
        if(str[i] == splitter){
            ans = new string[2];
            ans[0] = trim(str.substr(0,i));
            ans[1] = trim(str.substr(i+1));
            break;
        }
    }
    return ans;
}

bool starts_with(string str, string prefix){
    return str.size() >= prefix.size() && (str.substr(0, prefix.size()) == prefix);
}

vector<string> multisplit(string str, char delim){
    vector<string> ans;
    string cur = "";
    for(int i = 0; i < str.size(); i++){
        if(str[i] == delim && cur != ""){
            ans.push_back(cur);
            cur = "";
        }else if(str[i] != delim){
            cur += str[i];
        }
    }
    if(cur != "") ans.push_back(cur);
    return ans;
}

string toString(vector<string>& vec){
    string ans = "";
    for(string s : vec) ans += s;
    return ans;
}