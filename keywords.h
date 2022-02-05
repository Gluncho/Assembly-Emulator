#ifndef __keywords__
#define __keywords__

using namespace std;

#define MEMORY_SIZE 10000
#define MAIN "main"
#define RET "RET"

char ops[] = {'-', '+', '*', '/'};
const string CALL_STR = "CALL";
const string STACK_NAME_STR = "M";
const string REGISTER_NAME_STR = "R";

// BRANCHES
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