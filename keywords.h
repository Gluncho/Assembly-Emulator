#ifndef __keywords__
#define __keywords__


using namespace std;

//flags
#define DEBUG_FLAG "-dbg"
#define HELP "-help"
#define EXIT "-exit"

//IMPORTANT INTEGERS
#define MEMORY_SIZE 20000
#define INSTRUCTION_SIZE 4
#define CAST_SIZE 2


#define MAIN "main"
#define RET "RET"

//BUILT-IN FUNCTIONS
#define PRINTF "printf"
#define STRLEN "strlen"
#define PRINTSTR "printStr"

//SPECIAL PREFIX/SUFFIX CHARACTERS
#define OPEN_BRACKET '['
#define CLOSED_BRACKET ']'
#define CALL_PREFIX '<'
#define CALL_SUFFIX '>'

// SUPPORTED OPERATORS
char ops[] = {'-', '+', '*', '/', '%', '&', '|'};

const string CALL_STR = "CALL";
const string JUMP_STR = "JMP";
const string STACK_NAME_STR = "M";
const string REGISTER_NAME_STR = "R";

//CASTS
const string to_char = ".1";
const string to_short = ".2";

const string regex_int = "%d";

// BRANCHES
#define MIN_BRANCH_TOKENS 4 // for example, BGE R1 R2 200
#define MAX_BRANCH_TOKENS 6 // for example, BGE R1 R2 PC + 200
const string BGE = "BGE";
const string BGT = "BGT";
const string BLE = "BLE";
const string BLT = "BLT";
const string BEQ = "BEQ";
const string BNE = "BNE";

#define SP "SP"
#define RV "RV"
#define PC "PC"


enum line_type {
    ALU,
    STORE,
    LOAD,
    CALL,
    BRANCH,
    JUMP,
    INVALID
};
#endif