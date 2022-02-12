#ifndef __emulator__
#define __emulator__
#include <bits/stdc++.h>
#include "error.h"
#include "keywords.h"
void interact_with_user();

void help(); 
void displayLineType(line_type type);

void read_file(string filename);
void read_functions();

void process_file(string filename);
void process_line();

void execute_file();
void execute_branch(string& line);
void store(string& line);
void load(string& line);
void perform_alu(string& line);
void jump(string& line);
void call_function(string& line);
void execute_function(string func_name);

/**
 * @brief 
 * Evaluates an expression. 
 * Expression must be either a constant, a valid register,
 * or an operation between registers/numbers/register and a number.
 * @param expr string to be evaluated
 * @return int value of the expression
 */
int eval(string expr);
line_type get_line_type(string& line);

/**
 * If the line is indeed declaring a function, pop its name in func_name variable.
 */
bool is_function_declaration(string& line, string& func_name);

//self explanatory
bool is_cast(string& cast);
bool is_register(string& str);
bool is_number_or_register(string& str);
bool is_valid_call(string& line);
bool is_valid_branch(string& line);
bool is_valid_store(string& lhs, string& rhs);

/**
 * @brief 
 * Checks whether the expression is valid \a ALU operation.
 * @param lhs 
 * @param rhs 
 * @return true if @p lhs and @p rhs form a valid ALU operation.
 * @return false otherwise
 */
bool is_valid_alu(string& lhs, string& rhs);


/**
 * \brief 
 * Checks whether the expression is valid \a load operation.
 * \note function is space-friendly
 * @param lhs 
 * @param rhs 
 * @return true if @p lhs and @p rhs form a valid load operation
 * @return false otherwise
 */
bool is_valid_load(string& lhs, string& rhs);
bool is_valid_jump(string& line);
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
bool is_valid_expression(string& expr, bool is_alu);

#endif