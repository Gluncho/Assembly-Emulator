#include <bits/stdc++.h>
#include "error.h"
#include "keywords.h"

using namespace std;

/* Mapping Function names to its addresses */
map<string,int> functions;
vector<string> lines;
//stack
char M[MEMORY_SIZE];
// registers
map<string,int> registers;
// built-in functions
set<string> built_in_functions;

void help(); 
void process_file(char* filename);
void read_file(char* filename);
void execute_file();
void read_functions();
void execute_function(string func_name);
void execute_branch(string& line);
void store(string& line);
void load(string& line);

void process_line();
void call_function(string& line);
bool is_function_declaration(string& line, string& func_name);
line_type get_line_type(string& line);


/* helper methods for lines */
string trim(string line, char delim = ' ');
string* split_by(string str, char splitter);
vector<string> multisplit(string str, char delim = ' ');
int eval(string expr);
int get_value(string& number);

bool is_cast(string& cast);
bool is_valid_call(string& line);
bool is_valid_branch(string& line);
bool is_valid_store(string& lhs, string& rhs);
bool is_valid_alu(string& lhs, string& rhs);
bool is_valid_load(string& lhs, string& rhs);

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

void help(){
    cout<<"Hi. This is an Assembly Emulator. "<<endl;
    cout<<"If you want to execute an assembly file, you can "<<endl;
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
        if(line == "" || trim(line) == "") continue; //Skip empty lines
        lines.push_back(trim(line));
    }
}

/**
 * Cuts the extra symbols at the beggining and the end of the string.
 * @param line string to be trimmed
 * @param delim symbol to be cut
 * @return string - Trimmed version of line @line
 */
string trim(string line, char delim){
    int start = 0, end = line.size()-1;
    while(start < line.size() && line[start]==delim) start++;
    while(end >= 0  && line[end]==delim) end--;
    if(start > end) return "";
    return line.substr(start, end - start + 1);
}

void execute_file(){
    read_functions();
    registers[SP] = MEMORY_SIZE - 1;
    if(functions.find(MAIN) != functions.end()) registers[PC] = functions[MAIN] + 4;

    cout<<"----------------- STARTING EXECUTION -----------------"<<endl;
    clock_t begin = clock();
    execute_function(MAIN);
    clock_t end = clock();
    double elapsed_secs = double(end - begin);
    cout<<"----------------- EXECUTION COMPLETED SUCCESSFULLY.-----------------"<<endl;
    cout<<"Elapsed(ms) : "<<end-begin<<endl;
}

void read_functions(){
    for(int i = 0; i < lines.size(); i++){
        string func_name = "";
        if(is_function_declaration(lines[i], func_name)){
            if(functions.count(func_name)) ERROR_REDECLARATION_OF_FUNCTION(func_name)
            // Map function name to its respective line
            functions[func_name] = i * 4;
        } 
    }
}

void execute_function(string func_name){
    if(functions.count(func_name) == 0) ERROR_FUNC_NOT_FOUND(func_name)
    int current_line_num = functions[func_name] / 4;
    registers[SP] -= sizeof(int); CHECK_SP
    memcpy(&M[registers[SP]], &registers[PC], sizeof(int)); // SAVED PC
    registers[PC] = current_line_num * 4;
    while(lines[line_num] != RET){
        process_line();
    }
    cout<<"Exiting function "<<func_name<<endl;
    registers[PC] = *(int*)&M[registers[SP]];
    registers[SP] += sizeof(int); 
}

void process_line(){
    registers[PC] += 4;
    cout<<"Executing Line :: "<<line_num<<";;;; Line : "<<lines[line_num - 1]<<endl;
    line_type type = get_line_type(lines[line_num - 1]);
    if(type == INVALID) ERROR_INVALID_LINE
    else if(type == CALL){
        call_function(lines[line_num - 1]);
    }else if(type == BRANCH){
        execute_branch(lines[line_num - 1]);
    }else if(type == STORE){
        store(lines[line_num - 1]);
    }else if(type == LOAD){
        //load(lines[line_num - 1]);
    }
}

line_type get_line_type(string& line){
    string* splitted = split_by(line,'='); 
    if(splitted == NULL){
        if(is_valid_call(line)) return CALL;
        else if(is_valid_branch(line)) return BRANCH;
        return INVALID;
    }
    string lhs = splitted[0];
    string rhs = splitted[1];
    if(is_valid_store(lhs,rhs)){
        delete[] splitted;
        return STORE;
    }
    return INVALID;
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

bool is_cast(string& cast){
    return cast == to_char || cast == to_short;
}

bool is_number(string text){
    for(int i = 0; i < text.size(); i++){
        if((text[i] < '0') || (text[i] > '9')) return false;
    }
    return true;
}

bool is_register(string& str){
    if(str == RV || str == SP || str == PC) return true;
    if(str.length() < REGISTER_NAME_STR.size()) return false;
    if(str.substr(0, REGISTER_NAME_STR.size()) == REGISTER_NAME_STR 
        && is_number(str.substr(REGISTER_NAME_STR.size()))) return true;
    return false;
}
bool is_number_or_register(string& str){
    return is_number(str) || is_register(str);
}

bool is_valid_call(string& line){
    if(line.size() < CALL_STR.size()) return false;
    if(line.substr(0, CALL_STR.size()) != CALL_STR) return false;
    string func = trim(line.substr(CALL_STR.size()+1));
    if(func == "") ERROR_INVALID_CALL
    if(func.front() == '<' && func.back() == '>'){
        if(functions.count(func.substr(1,func.size()-2)) == 0) ERROR_INVALID_CALL
        else return true;
    }
    if(!is_number_or_register(func)) ERROR_INVALID_CALL
    return false;
}

bool is_valid_branch(string& line){
    if(line.size() < BGE.size()) return false;
    vector<string> splitted = multisplit(line);
    if(splitted.size()<4 || splitted.size() > 6) return false;
    string brc = splitted[0];
    if(!(brc == BGE || brc == BGT || brc == BLE || brc == BLT || brc == BNE || brc ==BEQ )) return false;
    if(!is_number_or_register(splitted[1]) || !is_number_or_register(splitted[2])) return false;
    return true;
}

bool is_valid_store(string& lhs, string& rhs){
    if(lhs.size() < STACK_NAME_STR.size() || lhs.substr(0,STACK_NAME_STR.size()) != STACK_NAME_STR) return false;
    string memo_index = lhs.substr(STACK_NAME_STR.size());
    if(memo_index.front() != '[' || memo_index.back() != ']') return false;
    string actual_index = memo_index.substr(1,memo_index.length() - 2);
    vector<string> split = multisplit(rhs);
    bool is_valid = is_number_or_register(actual_index);
    is_valid = is_valid && ((split.size() == 1 && is_number_or_register(split[0])) ||
                            (split.size() == 2 && is_cast(split[0]) && is_number_or_register(split[1])));
    return is_valid;
}

/**
 * @brief Splits the string by a character.
 * Splitting is done WRT first occurence of the character.
 * Passes the ownership of a pointer returned to the user. 
 * Returns NULL if the character is not found.
 * @param str string to be splitted
 * @param splitter character to be splitted by 
 * @return string* - Pointer to the string array of length 2.
 */
string* split_by(string str, char splitter){
    string* ans = NULL;
    for(int i = 1; i < str.length()-1; i++){
        if(str[i] == splitter){
            ans = new string[2];
            ans[0] = trim(str.substr(0,i));
            ans[1] = trim(str.substr(i+1));
            break;
        }
    }
    return ans;
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
int indexOf(string str, char target){
    for(int i=0; i<str.length(); i++){
        if(str[i] == target) return i;
    }
    return -1;
}

void call_function(string& line){
    string name = trim(line.substr(CALL_STR.size() + 1));
    if(name.front() == '<'){
        execute_function(name.substr(1, name.size() - 2));
    }
}

void execute_branch(string& line){
    vector<string> splitted = multisplit(line);
    string expression = "";
    for(int i = 3; i < splitted.size(); i++){
        expression += splitted[i];
    }
    int toJump = eval(expression);
    //cout<<registers[PC]<< " IS PC. jumping on "<<toJump<<endl;
    int LHS = is_number(splitted[1]) ? stoi(splitted[1]) : registers[splitted[1]];
    int RHS = is_number(splitted[2]) ? stoi(splitted[2]) : registers[splitted[2]];
    bool branch_should_jump = false;
    string brc = splitted[0];
    if(brc == BGE && LHS >= RHS) branch_should_jump = true;
    else if(brc == BGT && LHS > RHS) branch_should_jump = true;
    else if(brc == BLE && LHS <= RHS) branch_should_jump = true;
    else if(brc == BLT && LHS < RHS) branch_should_jump = true;
    else if(brc == BEQ && LHS == RHS) branch_should_jump = true;
    else if(brc == BNE && LHS != RHS) branch_should_jump = true;
    if(branch_should_jump){
            registers[PC] = toJump - INSTRUCTION_SIZE;
    }
}

int get_value(string& number) {
    return is_number(number) ? stoi(number) : registers[number];
}

void store(string& line){
    string LHS = trim(multisplit(line,'=')[0]);
    string RHS = multisplit(line, '=')[1];
    string memo_index = LHS.substr(STACK_NAME_STR.size() + 1, LHS.size() - STACK_NAME_STR.size() - 2);
    int memo_ind = get_value(memo_index);
    vector<string> split = multisplit(RHS);
    int to_assign;
    if(split.size() == 1) to_assign = get_value(split[0]);
    else if(split[0] == to_char){
        to_assign = (char) get_value(split[1]);
    }else if(split[0] == to_short){
        to_assign = (short) get_value(split[1]);
    }
    M[memo_ind] = to_assign;
    cout<<"memo index is "<< (int)M[memo_ind]<<endl;
}


int eval(string expr){
    if(is_number(expr)) return stoi(expr);
    if(registers.find(expr) != registers.end()) return registers[expr];
    char oper = ' ';
    int index = -1;
    for(char op : ops){
        if(indexOf(expr, op) != -1){
            oper = op;
            index = indexOf(expr,op);
        }
    }
    if(index == -1) ERROR_INVALID_LINE
    string lhs = expr.substr(0,index);
    string rhs = expr.substr(index + 1);
    int LHS, RHS;
    if(is_number(lhs)) LHS = stoi(lhs);
    else if(is_register(lhs)) LHS = registers[lhs];
    else ERROR_INVALID_LINE
    if(is_number(rhs)) RHS = stoi(rhs);
    else if(is_register(rhs)) RHS = registers[rhs];
    else ERROR_INVALID_LINE
    if(oper == '-') return LHS - RHS;
    else if(oper == '+') return LHS + RHS;
    else if(oper == '*') return LHS * RHS;
    return LHS / RHS;
}