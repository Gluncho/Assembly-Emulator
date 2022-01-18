#ifndef __instruction__
#define __instruction__

#include <bits/stdc++.h>
#include "keywords.h"
using namespace std;

class Instruction{
    public:
        Instruction(string& line);
        line_type GetType();

    private:
        string line;
        bool checkForStore();
        bool checkForLoad();
        bool checkForReturn();
        bool checkForALU();
};

#endif