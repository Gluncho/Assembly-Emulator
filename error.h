#ifndef __error__
#define __error__

#define ERROR_NO_FILE_FOUND \
        {cout<<"ERROR: Invalid filename. Please enter a valid one next time."<<endl; \
        exit(0); \
        }

#define ERROR_REDECLARATION_OF_FUNCTION(name) \
        {cout<<"ERROR: Function named \""<<name \
        <<"\" is declared more than once"<<endl; \
        exit(0);\
        }


#endif