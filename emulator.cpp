#include <bits/stdc++.h>
#include "error.h"
#include "keywords.h"

using namespace std;

/* Mapping Function names to its addresses */
map<string,int> functions;
vector<string> lines;
// stack
char M[MEMORY_SIZE];

int SP = MEMORY_SIZE - 1, RV, PC;
void help(); 
void process_file(char* filename);
void read_file(char* filename);
void execute_file();
void read_functions();
void execute_function(string func_name);

string trim(string& line);
bool is_function_declaration(string& line, string& func_name);

int main(int argc, char* argv[]){
    if (argc == 1){
        cout<<"Please enter the file you want to execute, or use \"-h\" flag to get more information."<<endl;
    }else if (argc == 2){
        if(strlen(argv[1]) == 2 && strcmp(argv[1],"-h") == 0){
            help();
        }else process_file(argv[1]);
    }
    return 0;
}

void process_file(char* filename){
    read_file(filename);
    execute_file();
}

void read_file(char* filename){
    ifstream file;
    file.open(filename);
    if(!file) ERROR_NO_FILE_FOUND
    string line;
    while(getline(file,line)){
        if(line == "" || trim(line) == "") continue;
        lines.push_back(trim(line));
    }
}

string trim(string& line){
    int start = 0, end = line.size()-1;
    while(start < line.size() && line[start]==' ') start++;
    while(end >= 0  && line[end]==' ') end--;
    if(start > end) return "";
    return line.substr(start, end - start + 1);
}
void execute_file(){
    read_functions();
    execute_function(MAIN);
}

void read_functions(){
    for(int i = 0; i < lines.size(); i++){
        string func_name = "";
        if(is_function_declaration(lines[i], func_name)){
            if(functions.count(func_name)) ERROR_REDECLARATION_OF_FUNCTION(func_name)
            // Map function name to its respective line
            functions[func_name] = i;
        }
            
    }
}

void execute_function(string func_name){
    if(functions.count(func_name) == 0) ERROR_FUNC_NOT_FOUND(func_name)
    
}

void process_line(string line){
    
}
void help(){
    cout<<"this is help"<<endl;
}

/* If the line is indeed declaring a function, pop its name in func_name variable.
**/
bool is_function_declaration(string& line, string& func_name){
    if(line.size() < 2 || line.back() !=':') return false;
    string name = line.substr(0, line.size()-1);
    name = trim(name);
    if(!isalpha(name[0])) return false;
    for(int i = 1; i < name.size(); i++){
        if(!isalnum(name[i])) return false;
    }
    func_name = name;
    return true;
}