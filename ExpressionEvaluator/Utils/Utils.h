#ifndef Utils_h
#define Utils_h

#include <string>
using std::string;
using std::to_string;

// This file contains several shared functions used in PostfixEval and InfixToPostfix
// Because these functions are shared amongs different files (included twice), they have to be 'inline'.

/*
 Checks if a character is an operator
*/
inline bool isoperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

/*
 Returns the precedence (priority) of operators
*/
inline unsigned priority(char c) {
    if (isoperator(c)) {
        switch (c) {
            case '^':
                return 3;
                break;
            case '*':
            case '/':
                return 2;
            case '+':
            case '-':
                return 1;
            // No 'default' because isOperator() checked that case in the first place.
        }
    }
    // Brackets, etc.
    return 0;
}

/* Generate an syntax error description
    err_noti: error notification string/type of the error.
    position: where the error occured
    err_char: the character caused the error
    addt: Additional info (optional)
 */
inline string error_string_gen(string err_noti, unsigned position, char err_char, string addt = "") {
    return err_noti + to_string(position) + ": '" + err_char + "'. " + addt;
}
// Example: Syntax error at 3: 'a'. Invalid operand.

#endif /* Utils_h */
