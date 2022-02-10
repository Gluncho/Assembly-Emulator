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
void perform_alu(string& line);

void process_line();
void call_function(string& line);
bool is_function_declaration(string& line, string& func_name);
line_type get_line_type(string& line);


/* helper methods for lines */
string trim(string line, char delim = ' ');
string remove_spaces(string& str);
string* split_by(string str, char splitter);
int indexOf(string& str, char target);
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
        if(strlen(argv[1]) == 2 && strcmp(argv[1],HELP) == 0){
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
    if(functions.find(MAIN) != functions.end()) registers[PC] = functions[MAIN] + INSTRUCTION_SIZE;

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
            functions[func_name] = i * INSTRUCTION_SIZE;
        } 
    }
}

void execute_function(string func_name){
    if(functions.count(func_name) == 0) ERROR_FUNC_NOT_FOUND(func_name)
    int current_line_num = functions[func_name] / INSTRUCTION_SIZE;
    registers[SP] -= sizeof(int); CHECK_SP
    memcpy(&M[registers[SP]], &registers[PC], sizeof(int)); // SAVED PC
    if(func_name == MAIN) cout<<*(int*)&M[registers[SP]]<<" is saved pc for main"<<endl;
    registers[PC] = current_line_num * INSTRUCTION_SIZE;
    while(lines[line_num] != RET){
        process_line();
    }
    //cout<<"Exiting function "<<func_name<<endl;
    registers[PC] = *(int*)&M[registers[SP]];
    registers[SP] += sizeof(int); 
}

void process_line(){
    registers[PC] += INSTRUCTION_SIZE;
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
        load(lines[line_num - 1]);
    }else if(type == ALU){
        perform_alu(lines[line_num - 1]);
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
    }else if(is_valid_load(lhs,rhs)){
        return LOAD;
    }else if(is_valid_alu(lhs,rhs)){
        return ALU;
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

/**
 * @brief 
 * Checks whether the expression is valid or not,
 * based on the value of @p is_alu
 * @param expr expression that must be checked
 * @param is_alu boolean expressing whether @p expr should be checked for 
 * ALU operation ( @p is_alu = \c true ) or LOAD operation ( @p is_alu = \c false ).
 * @return true if expression is just a register, just a constant, or 
 * operation between register and a constant
 * @return false otherwise
 */
bool is_valid_expression(string& expr, bool is_alu = false){
    if(is_number_or_register(expr)) return true;
    // otherwise, it must be an operation between a constant and a register
    string concatenated = remove_spaces(expr);
    int oper_index = -1;
    for(char op : ops){
        int ind = indexOf(concatenated, op);
        if(ind != -1){
            oper_index =  ind;
            break;
        }
    }
    if(oper_index == -1) return false;
    string LHS = concatenated.substr(0,oper_index);
    string RHS = concatenated.substr(oper_index + 1);
    if(!is_alu && is_register(LHS) && is_register(RHS)) return false;
    return is_number_or_register(LHS) && is_number_or_register(RHS);
}

bool is_valid_call(string& line){
    if(line.size() < CALL_STR.size()) return false;
    if(line.substr(0, CALL_STR.size()) != CALL_STR) return false;
    string func = trim(line.substr(CALL_STR.size() + 1));
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
    if(splitted.size() < 4 || splitted.size() > 6) return false;
    string brc = splitted[0];
    if(!(brc == BGE || brc == BGT || brc == BLE || brc == BLT || brc == BNE || brc ==BEQ )) return false;
    if(!is_number_or_register(splitted[1]) || !is_number_or_register(splitted[2])) return false;
    return true;
}

bool is_valid_store(string& lhs, string& rhs){
    if(lhs.size() < STACK_NAME_STR.size() || lhs.substr(0,STACK_NAME_STR.size()) != STACK_NAME_STR) return false;
    string memo_index = lhs.substr(STACK_NAME_STR.size());
    if(memo_index.front() != OPEN_BRACKET || memo_index.back() != CLOSED_BRACKET) return false;
    string actual_index = memo_index.substr(1,memo_index.length() - 2);
    vector<string> split = multisplit(rhs);
    bool is_valid = is_valid_expression(actual_index);
    is_valid = is_valid && ((split.size() == 1 && is_number_or_register(split[0])) ||
                            (split.size() == 2 && is_cast(split[0]) && is_number_or_register(split[1])));
    return is_valid;
}

/**
 * \brief 
 * Checks whether the expression is valid \a load operation.
 * \note function is space-friendly
 * @param lhs 
 * @param rhs 
 * @return true if @p lhs and @p rhs form a valid load operation
 * @return false otherwise
 */
bool is_valid_load(string& lhs, string& rhs){
    if(is_register(lhs) == false) return false; // LHS should be a register
    string expr = rhs;
    string cast = expr.length() >= CAST_SIZE ? expr.substr(0, CAST_SIZE) : "";
    if(cast != "" && is_cast(cast)){
        string expr_without_cast = expr.substr(CAST_SIZE);
        expr = remove_spaces(expr_without_cast); //trim casting so we can check the rest of the expression
    }
    //expression must be something like M[....]
    if(expr.size() < (STACK_NAME_STR.size() + 2) || expr.substr(0, STACK_NAME_STR.size()) != STACK_NAME_STR) return false;
    if(expr[STACK_NAME_STR.size()] != OPEN_BRACKET || expr.back() != CLOSED_BRACKET) return false;
    //there must be an expression between brackets
    expr = expr.substr(STACK_NAME_STR.size() + 1, expr.size() - STACK_NAME_STR.size() - 2);
    return is_valid_expression(expr);
}

/**
 * @brief 
 * Checks whether the expression is valid \a ALU operation.
 * @param lhs 
 * @param rhs 
 * @return true if @p lhs and @p rhs form a valid ALU operation.
 * @return false otherwise
 */
bool is_valid_alu(string& lhs, string& rhs){
    if(is_register(lhs) == false) return false; //LHS should be a register
    string cast = rhs.length() >= CAST_SIZE ? rhs.substr(0, CAST_SIZE) : "";
    if(cast != "" && is_cast(cast)){
        string rhs_without_cast = trim(rhs.substr(CAST_SIZE));
        return is_number_or_register(rhs_without_cast);
    }
    return is_valid_expression(rhs, true);
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

string remove_spaces(string& str){
    string ans = "";
    for(int i = 0; i < str.size(); i++){
        if(str[i] != ' ') ans += str[i];
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

/**
 * @brief 
 * Finds the first index of target in a string
 * @param string 
 * @param character 
 * @return int first index of the character in a string. If the character is not found, -1 is returned
 */
int indexOf(string& str, char target){
    for(int i = 0; i < str.length(); i++){
        if(str[i] == target) return i;
    }
    return -1;
}


int get_value(string& number) {
    return is_number(number) ? stoi(number) : registers[number];
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

void store(string& line){
    string LHS = trim(multisplit(line,'=')[0]);
    string RHS = multisplit(line, '=')[1];
    string memo_index = LHS.substr(STACK_NAME_STR.size() + 1, LHS.size() - STACK_NAME_STR.size() - 2);
    memo_index = remove_spaces(memo_index);
    int memo_ind = eval(memo_index);
    vector<string> split = multisplit(RHS);
    int to_assign;
    if(split.size() == 1) {
        to_assign = get_value(split[0]);
        *(int*)&M[memo_ind] = to_assign;
    }
    else if(split[0] == to_char){
        M[memo_ind] = (char) get_value(split[1]);
    }else if(split[0] == to_short){
        *(short*)&M[memo_ind] = (short) get_value(split[1]);
    }
    
    //cout<<"Memory at index "<<memo_ind<<" and is equal to "<< (int)M[memo_ind]<<endl;
}

void load(string& line){
    string LHS = trim(multisplit(line,'=')[0]);
    string RHS = trim(multisplit(line, '=')[1]);
    string cast = RHS.length() >= CAST_SIZE ? RHS.substr(0, CAST_SIZE) : "";
    int memo_index;
    string memo = RHS;
    if(cast != "" && is_cast(cast)){
        memo = RHS.substr(CAST_SIZE);
    }
    memo = remove_spaces(memo);
    memo_index = eval(memo.substr(STACK_NAME_STR.size() + 1, memo.size() - STACK_NAME_STR.size() - 2));
    if(cast == to_char) registers[LHS] = M[memo_index];
    else if(cast == to_short) registers[LHS] = *(short*)&M[memo_index];
    else registers[LHS] = *(int*)&M[memo_index];
    //cout<<"register "<<LHS<<" is now equal to "<<registers[LHS]<<" wow."<<endl;
}

void perform_alu(string& line){
    string LHS = trim(multisplit(line,'=')[0]);
    string RHS = trim(multisplit(line, '=')[1]);
    string cast = RHS.length() >= CAST_SIZE ? RHS.substr(0, CAST_SIZE) : "";
    if(is_cast(cast)){
        int reg_value = registers[trim(RHS.substr(CAST_SIZE))];
        if(cast == to_char){
            registers[LHS] = *(char*)&reg_value;
        }else if(cast == to_short){
            registers[LHS] = *(short*)&reg_value;
        }
    }else{
        RHS = remove_spaces(RHS);
        registers[LHS] = eval(RHS);
    }
   // cout<<"register "<<LHS<<" now became "<<registers[LHS]<<endl;
}

/**
 * @brief 
 * Evaluates an expression. 
 * Expression must be either a constant, a valid register,
 * or an operation between registers/numbers/register and a number.
 * @param expr string to be evaluated
 * @return int value of the expression
 */
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