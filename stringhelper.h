#ifndef __stringhelper__
#define __stringhelper__

#include <bits/stdc++.h>

using namespace std;

/**
 * Cuts the extra symbols at the beggining and the end of the string.
 * @param line string to be trimmed
 * @param delim symbol to be cut
 * @return string - Trimmed version of line @line
 */
string trim(string line, char delim = ' ');

//self-explanatory
string remove_spaces(string str);

/**
 * @brief Splits the string by a character.
 * Splitting is done WRT first occurence of the character.
 * Passes the ownership of a pointer returned to the user. 
 * Returns NULL if the character is not found.
 * @param str string to be splitted
 * @param splitter character to be splitted by 
 * @return string* - Pointer to the string array of length 2.
 */
string* split_by(string str, char splitter);

/**
 * Checks whether @p str starts with @p prefix
 * @return true if @p prefix is a prefix of @p str
 * @return false otherwise
 */
bool starts_with(string str, string prefix);

/**
 * Tokenizes the string by @p delim , and returns list of tokens as a vector.
 * @param str 
 * @param delim 
 * @return vector<string> 
 */
vector<string> multisplit(string str, char delim = ' ');

/**
 * Returns string obtained by concatenating all of the elements of the vector @p vec
 * @param vec 
 * @return string - concatenated vector.
 */
string toString(vector<string>& vec);

#endif