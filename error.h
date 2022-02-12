#ifndef __error__
#define __error__

// utility to use for displaying line when error occurs
#define line_num (registers[PC]/4+1)

#define NO_OPERATOR_FOUND -1

#define ERROR_PREFIX "ERROR : LINE "<<line_num<<". "
#define ERROR_NO_FILE_FOUND \
        {cout<<"ERROR: Invalid filename. Please enter a valid one next time."<<endl; \
        exit(0); \
        }

#define ERROR_REDECLARATION_OF_FUNCTION(name) \
        {cout<<"ERROR: Function named \""<<name \
        <<"\" is declared more than once"<<endl; \
        exit(0);\
        }

#define ERROR_FUNC_NOT_FOUND(name) \
        {cout<< \
        "ERROR: Couldn't find the function named "<<name<<endl; \
        exit(0); \
        }

#define CHECK_SP if(registers[SP] < 0) { \
        cout<<ERROR_PREFIX<<"Stack overflow occured. The program will now terminate."<<endl; \
        exit(0);\
        }

#define ERROR_INVALID_CALL \
        {cout<<ERROR_PREFIX<<"Invalid function call. "<<endl; \
        exit(0); \
        }

#define ERROR_INVALID_LINE \
        {cout<<ERROR_PREFIX<<"INVALID LINE"<<endl; \
        exit(0); \
        }

#define EXIT_CODE \
        {cout<<"----------------------EXITING THE PROGRAM.---------------------------"<<endl; \
        exit(0); \
        }
#endif