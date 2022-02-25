#include "emulator.h"
#include "stringhelper.h"
using namespace std;

/* Mapping Function names to its addresses */
map<string,int> functions;
vector<string> lines;
//stack
char M[MEMORY_SIZE];
// registers
map<string,int> registers;
// built-in functions
set<string> built_in_functions = {"printf", "strlen", "printStr"};

//debugging mode
static bool DEBUG = false;

void init(){
    registers.clear();
    functions.clear();
    lines.clear();
    DEBUG = 0;
    memset(M, 0, sizeof(M));
}
int main(int argc, char* argv[]){
    interact_with_user();
    return 0;
}

void interact_with_user(){
    cout<<"----------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"\tEnter a path of a file which you want to be executed, type \"-help\" for more information."<<endl;
    while(true){
        init();
        cout<<"\n\tEnter the command: ";
        string input;
        getline(cin,input);
        input = trim(input);
        if(input == EXIT) EXIT_CODE;
        if(input == HELP){
            help();
        }else if(starts_with(input, DEBUG_FLAG)){
            DEBUG = 1;
            process_file(multisplit(input)[1]);
        }else{
            process_file(input);
        }
    }
   
}

void help(){
    
    cout<<"\t----------------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"\t----------------------------------------------------------HELP--------------------------------------------------------------"<<endl;
    cout<<"\t\tThis is an Assembly Emulator. "<<endl;
    cout<<"\t\tYou can use it to execute your own assembly code (Note: Should be a valid code that RISC processor uses."<<endl;
    cout<<"\t\tIf you don't know the language, see \"DOCUMENTATION.md\" to get a grasp on it."<<endl;
    cout<<"\t\tOtherwise, you can either run your own tests, or tests provided in \"tests\" folder"<<endl<<endl;
    cout<<"\t\tYou can run file with the debugging flag, which means that for each executed line,\n\tyou're gonna see what has really changed in the stack, or registers."<<endl;
    cout<<"\t\tAnd also, you can see what is the type of the currently executing line."<<endl;
    cout<<"\t\tTo do that, you can use -dbg flag before entering a filename."<<endl;
    cout<<"\t----------------------------------------------------------------------------------------------------------------------------"<<endl;

}

void process_file(string filename){
    read_file(filename);
    execute_file();
}

void read_file(string filename){
    ifstream file;
    file.open(filename);
    if(!file) ERROR_NO_FILE_FOUND
    string line;
    while(getline(file,line)){
        if(line == "" || trim(line) == "") continue; //Skip empty lines
        lines.push_back(trim(line));
    }
}


void execute_file(){
    read_functions();
    registers[SP] = MEMORY_SIZE;
    if(functions.find(MAIN) != functions.end()) registers[PC] = functions[MAIN] + INSTRUCTION_SIZE;

    cout<<"\t\t------------------------------- STARTING EXECUTION -------------------------------"<<endl;
    clock_t begin = clock();
    execute_function(MAIN);
    clock_t end = clock();
    double elapsed_secs = double(end - begin);
    cout<<"\t\t------------------------ EXECUTION COMPLETED SUCCESSFULLY ------------------------"<<endl;
    cout<<"\tElapsed time(ms) : "<<end-begin<<endl;
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
    if(built_in_functions.count(func_name)) {
        execute_prebuilt_function(func_name);
        return;
    }
    if(functions.count(func_name) == 0) ERROR_FUNC_NOT_FOUND(func_name)
    int current_line_num = functions[func_name] / INSTRUCTION_SIZE;
    if(func_name != MAIN){
        registers[SP] -= sizeof(int); CHECK_SP
        memcpy(&M[registers[SP]], &registers[PC], sizeof(int)); // SAVED PC
    }
    registers[PC] = current_line_num * INSTRUCTION_SIZE;
    while(lines[line_num] != RET){
        process_line();
    }

    if(func_name != MAIN) registers[PC] = *(int*)&M[registers[SP]], registers[SP]+= sizeof(int); //update SP, PC

    if(DEBUG) {
        cout<<"\tEXITING FUNCTION "<<func_name;
        if(func_name != MAIN) cout<<" (RETURNING TO SAVED PC, WHICH TURNS OUT TO BE LINE "<<line_num<<").";
        cout<<endl;
    } 
}

void execute_prebuilt_function(string func_name){
    if(func_name == PRINTF) call_printf();
    else if(func_name == STRLEN) call_strlen();
    else if(func_name == PRINTSTR) call_printStr();
}

void process_line(){
    registers[PC] += INSTRUCTION_SIZE;
    string line = lines[line_num - 1];
    line_type type = get_line_type(line);
    if(type == INVALID) ERROR_INVALID_LINE
    if(DEBUG) {
        cout<<"\tEXECUTING LINE NUMBER "<<line_num<<". ---------------- Line : \""<<line<<"\""<<endl;
        displayLineType(type);
    }
    if(type == CALL){
        call_function(line);
    }else if(type == BRANCH){
        branch(line);
    }else if(type == STORE){
        store(line);
    }else if(type == LOAD){
        load(line);
    }else if(type == ALU){
        perform_alu(line);
    }else if(type == JUMP){
        jump(line);
    }
    if(DEBUG) cout<<endl<<endl;
}

void displayLineType(line_type type){
    string str;
    if(type == ALU) str = "ALU";
    else if(type == STORE) str = "STORE";
    else if (type == LOAD) str = "LOAD";
    else if(type == CALL) str = "CALL";
    else if(type == BRANCH) str = "BRANCH";
    else if(type == JUMP) str = "JUMP";
    else if(type == INVALID) str = "INVALID";
    cout<<"\tType of this line is "<<str<<" type."<<endl;
}

line_type get_line_type(string& line){
    string* splitted = split_by(line,'='); 
    if(splitted == NULL){
        if(is_valid_call(line)) return CALL;
        else if(is_valid_branch(line)) return BRANCH;
        else if(is_valid_jump(line)) return JUMP;
        return INVALID;
    }
    string lhs = splitted[0];
    string rhs = splitted[1];
    if(is_valid_store(lhs,rhs)){
        delete[] splitted;
        return STORE;
    }else if(is_valid_load(lhs,rhs)){
        delete[] splitted;
        return LOAD;
    }else if(is_valid_alu(lhs,rhs)){
        delete[] splitted;
        return ALU;
    }
    delete[] splitted;
    return INVALID;
}

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
    if(text.front() == '-' && text.size() > 1){
        return text[1]>='0' && text[1]<='9' && is_number(text.substr(1));
    }
    for(int i = 0; i < text.size(); i++){
        if(text[i]<'0' || text[i] > '9') return false;
    }
    return true;
}

bool is_register(string& str){
    if(str == RV || str == SP || str == PC) return true;
    return starts_with(str, REGISTER_NAME_STR) && is_number(str.substr(REGISTER_NAME_STR.size()));
}

bool is_number_or_register(string& str){
    return is_number(str) || is_register(str);
}

bool is_casting_number(string& rhs){
    vector<string> tokenized = multisplit(rhs);
    return tokenized.size() == 2 && is_cast(tokenized[0]) && is_number_or_register(tokenized[1]);
}

bool is_valid_expression(string& expr, bool is_alu = false){
    if(is_number_or_register(expr) && !is_alu) return true; 

    // otherwise, it must be an operation between a constant and a register
    string concatenated = remove_spaces(expr);
    int oper_index = NO_OPERATOR_FOUND;
    for(char op : ops){
        size_t ind = concatenated.find(op);
        if(ind != string::npos){
            oper_index =  ind;
            break;
        }
    }
    if(oper_index == NO_OPERATOR_FOUND) return false;
    string LHS = concatenated.substr(0,oper_index);
    string RHS = concatenated.substr(oper_index + 1);

    // ALU can be an operation of register and a register, but memory address can't.
    if(!is_alu && is_register(LHS) && is_register(RHS)) return false;
    return is_number_or_register(LHS) && is_number_or_register(RHS);
}

bool is_valid_call(string& line){
    if(!starts_with(line, CALL_STR)) return false;
    string func = trim(line.substr(CALL_STR.size() + 1));
    if(func == "") ERROR_INVALID_CALL
    if(func.front() == '<' && func.back() == '>'){
        string name = func.substr(1, func.size() - 2);
        if(functions.count(name) == 0 && built_in_functions.count(name) == 0) ERROR_INVALID_CALL
        else return true;
    }
    if(!is_number_or_register(func)) ERROR_INVALID_CALL
    return true;
}

bool is_valid_branch(string& line){
    if(line.size() < BGE.size()) return false;
    vector<string> splitted = multisplit(line);
    if(splitted.size() < MIN_BRANCH_TOKENS || splitted.size() > MAX_BRANCH_TOKENS) return false;
    string brc = splitted[0];
    if(!(brc == BGE || brc == BGT || brc == BLE || brc == BLT || brc == BNE || brc ==BEQ )) return false;
    if(!is_number_or_register(splitted[1]) || !is_number_or_register(splitted[2])) return false;
    return true;
}

bool is_valid_store(string& lhs, string& rhs){
    if(!starts_with(lhs, STACK_NAME_STR)) return false;
    string memo_index = lhs.substr(STACK_NAME_STR.size());
    if(memo_index.front() != OPEN_BRACKET || memo_index.back() != CLOSED_BRACKET) return false;
    string actual_index = memo_index.substr(1,memo_index.length() - 2); // cut parenthesis
    vector<string> split = multisplit(rhs);
    bool is_valid = is_valid_expression(actual_index);
    is_valid = is_valid && ((split.size() == 1 && is_number_or_register(split[0])) ||
                            (split.size() == 2 && is_cast(split[0]) && is_number_or_register(split[1])));
    return is_valid;
}

bool is_valid_load(string& lhs, string& rhs){
    if(is_register(lhs) == false) return false; // LHS should be a register
    return is_number_or_register(rhs) || is_casting_number(rhs) || is_loading_from_memory(rhs);
}

bool is_valid_alu(string& lhs, string& rhs){
    if(is_register(lhs) == false) return false; //LHS should be a register
    string cast = rhs.length() >= CAST_SIZE ? rhs.substr(0, CAST_SIZE) : "";
    if(is_cast(cast)){
        string rhs_without_cast = trim(rhs.substr(CAST_SIZE));
        return is_number_or_register(rhs_without_cast);
    }
    return is_valid_expression(rhs, true);
}

bool is_valid_jump(string& line){
    if(!starts_with(line, JUMP_STR)) return false;
    vector<string> tokenized = multisplit(line);
    tokenized.erase(tokenized.begin());
    string pc_address = toString(tokenized);
    return is_valid_expression(pc_address);
}

int get_value(string& number) {
    return is_number(number) ? stoi(number) : registers[number];
}

void call_function(string& line){
    string name = trim(line.substr(CALL_STR.size() + 1));
    if(name.front() == CALL_PREFIX && name.back() == CALL_SUFFIX){
        execute_function(name.substr(1, name.size() - 2));
    }else{
        int newPC = get_value(name);
        name = lines[newPC / 4];
        string func_name;
        if(!is_function_declaration(name, func_name)) ERROR_INVALID_CALL
        execute_function(func_name);
    }
}

void call_printf(){
    cout<<"\t\t-------------------------------------- EXECUTING PRINTF... ----------------------------------------------------"<<endl;
    cout<<"\t\t";
    int cur = registers[SP];
    cout<<*(int*)&M[cur]<<endl;
    cout<<"\t\t ----------------------------- PRINTF EXECUTION ENDED SUCCESSFULLY---------------------------------------------"<<endl;

}

void call_printStr(){
    int char_pointer = *(int*)&M[registers[SP]];
    cout<<"\t\t-------------------------------------- EXECUTING PRINTSTR... ----------------------------------------------------"<<endl;
    cout<<"\t\t";
    printf("%s\n", &M[char_pointer]);
    cout<<"\t\t ----------------------------- PRINTF EXECUTION ENDED SUCCESSFULLY---------------------------------------------"<<endl;
}

void call_strlen(){
    int str_ind = *(int*)&M[registers[SP]];
    int len = 0;
    while(M[str_ind++] != '\0') len++;
    registers[RV] = len;
}
void branch(string& line){
    vector<string> splitted = multisplit(line);
    string expression = "";
    for(int i = 3; i < splitted.size(); i++){
        expression += splitted[i];
    }
    int toJump = eval(expression);
    
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
            if(DEBUG) cout<<"\tExecuting branch jump....\n\tJumping on line "<<toJump/4 + 1<<endl;
            registers[PC] = toJump - INSTRUCTION_SIZE;
    }
}

void jump(string& line){
    string expr = remove_spaces(line.substr(JUMP_STR.size()));
    int toJump = eval(expr);
    registers[PC] = toJump - INSTRUCTION_SIZE;
    if(DEBUG) cout<<"\tJumping on line "<<line_num + 1<<endl;
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
        to_assign = (char) get_value(split[1]);
        M[memo_ind] = to_assign;
    }else if(split[0] == to_short){
        to_assign = (short) get_value(split[1]);
        *(short*)&M[memo_ind] = to_assign;
    }
    
    if(DEBUG) cout<<"\tMemory at index "<<memo_ind<<" and is equal to "<<to_assign<<endl;
}

void load(string& line){
    string LHS = trim(multisplit(line,'=')[0]);
    string RHS = trim(multisplit(line, '=')[1]);
    if(!is_loading_from_memory(RHS)) load_number(LHS,RHS);
    else{
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
    }
    if(DEBUG) cout<<"\tREGISTER "<<LHS<<" NOW HAS A VALUE EQUAL TO "<<registers[LHS]<<endl;
}

void perform_alu(string& line){
    string LHS = trim(multisplit(line,'=')[0]);
    string RHS = remove_spaces(multisplit(line, '=')[1]);
    registers[LHS] = eval(RHS);
    if(DEBUG) cout<<"\tREGISTER "<<LHS<<" NOW HAS A VALUE EQUAL TO "<<registers[LHS]<<endl;
}

int eval(string expr){
    if(is_number(expr)) return stoi(expr);
    if(registers.find(expr) != registers.end()) return registers[expr];
    char oper = ' ';
    int index = -1;
    for(char op : ops){
        if(expr.find(op) != string::npos){
            oper = op;
            index = expr.find(op);
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
    else if(oper == '/') return LHS / RHS;
    else if(oper == '%') return LHS % RHS;
    else if(oper == '&') return LHS & RHS;
    else if(oper == '|') return LHS | RHS;
    return 0;
}

bool is_loading_from_memory(string& rhs){
    string expr = rhs;
    string cast = expr.length() >= CAST_SIZE ? expr.substr(0, CAST_SIZE) : "";
    if(cast != "" && is_cast(cast)){
        string expr_without_cast = expr.substr(CAST_SIZE);
        expr = remove_spaces(expr_without_cast); //trim casting so we can check the rest of the expression
    }
    //expression must be something like M[....]
    if(!starts_with(expr, STACK_NAME_STR)) return false;
    if(expr[STACK_NAME_STR.size()] != OPEN_BRACKET || expr.back() != CLOSED_BRACKET) return false;
    //there must be an expression between brackets
    expr = expr.substr(STACK_NAME_STR.size() + 1, expr.size() - STACK_NAME_STR.size() - 2);
    return is_valid_expression(expr);
}

void load_number(string& lhs, string& rhs){
    if(is_number_or_register(rhs)) registers[lhs] = get_value(rhs);
    else{
        string cast = rhs.substr(0, CAST_SIZE);
        string rest = trim(rhs.substr(CAST_SIZE));
        int value = get_value(rest);
        if(cast == to_char) registers[lhs] = (char) value;
        else if(cast == to_short) registers[lhs] = (short) value;
    }
}