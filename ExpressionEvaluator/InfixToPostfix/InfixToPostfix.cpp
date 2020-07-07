#include "InfixToPostfix.h"

/*
 Convert an infix expression to a postfix expression
 */
string infix_to_postfix(string i_expression) {
    StackChar *stack = new StackChar;
    
    if (!stackInit(&stack)) {
        throw runtime_error("[Infix to Postfix] Stack init failed.");
    }
        
    string result_expr; // Postfix Expression (final result)
    unsigned index = 0;
    char c = 0;
    
    char last_char = '\0';  // Last non-space char that was scanned. Used for syntax tracking.
    
    /*
     Overall algorithm
     
     Source: based on https://www.geeksforgeeks.org/stack-set-2-infix-to-postfix/
     Addition of support for float numbers, negative numbers and error handling/syntax tracker by me.
     
     - Loop through the infix expression.
     - If the character is a digit or a decimal point '.', add it to the postfix expr.
     - If the character is an opening parenthesis '(', push it into the stack.
     - If the character is an operator,
        + If the precedence of the current operator > precedence of stackTop() (or the stack is empty, or stackTop() == '('), push the operator into the stack.
        + Else, pop the stack while the operator on the top has equal or higher precedence (priority) than the currently processed operator. If a '(' is encountered, stop there. Finally push the operator to the stack.
     - If the character is a closing parenthesis, pop the stack (and append to the postfix expr) until an opening parenthesis is encountered (and ignore both). If the stack is empty before an opening parenthesis is found, raise syntax error.
     - Else, raise syntax error.
     - Finally, when the end of the infix expression is reached, pop all elements from the stack and add them to the postfix expr.
     */
        
    // Loop through the infix expression.
    for (index = 0; index < i_expression.size(); index++) {
        // Abbreviate i_expression[index] to c
        c = i_expression[index];
        
        // If the character is a digit or '.'
        if (isdigit(c) || c == '.') {
            /* Append a space before appending the digit (to get something like '2 5 7 + * ...')
               Except when:
                    + The digit is at the beginning of the expression
                      (if we skip this, there will be a space at the beginning of the resulting postfix expression.
                   +  Before the digit was another digit or '.' or '-' (if we skip this, '1.25' will become '1 . 2 5').
             */
            
            // The previous character
            // Init it with NULL
            char prev_char = '\0';
            
            // If index > 0, get the previous char. If index == 0, there is no previous char
            if (index > 0)
                prev_char = i_expression[index - 1];
            
            // If previous char is not a digit, a dot or a '-', add a space.
            if (index != 0 && !isdigit(prev_char) && prev_char != '.')
                result_expr += ' ';
            
            // Finally append the digit/dot
            result_expr += c;
            
            last_char = c;
        }
        
        // Ignore the space
        else if (c == ' ' ) {
            continue;
        }
        
        // If the char is an open parenthesis
        else if (c == '(') {
            // Push it into the stack
            stackPush(stack, c);
        }
        
        // If the char is an operator
        else if (isoperator(c)) {
            // Negative number handling: if the operator is a '-', and it is followed immediately by a digit, and the previous char must not be a number (if so, it is a minus sign, not a negative sign), then append it into the result expression to form a negative number. Skip processing the '-' and go on to the next char.
            if (c == '-' && isdigit(i_expression[index + 1]) && !isdigit(last_char)) {
                result_expr += ' ';
                result_expr += c;
                last_char = '\0'; // "Yeah I'm not an operator" - negative sign says.
                continue;
            }
            
            // Syntax tracker: before an operator must be a digit.
            if (!isdigit(last_char)) {
                string e_str = error_string_gen("Conversion syntax error: non-operand unexpected before position", index, last_char);
                stackFree(&stack);
                throw runtime_error(e_str);
            }
            
            // Common steps for other cases
            // If the stack is empty, or precedence of c > precedence of stack top
            if (stackIsEmpty(stack) || priority(c) > priority(stackTop(stack))) {
                stackPush(stack, c);
            }
            else {
                // Pop and append while precedence of top > precedence of c
                while (!stackIsEmpty(stack) && priority(c) <= priority(stackTop(stack))) {
                    result_expr += ' ';
                    result_expr += stackPop(stack);
                }
                stackPush(stack, c);
            }
            last_char = c;
        }
        
        // If a closing parenthesis is encountered
        else if (c == ')') {
            try {
                while (stackTop(stack) != '(') {
                    result_expr += ' ';
                    result_expr += stackPop(stack);
                }
                // Finally pop and ignore the '('
                stackPop(stack);
            }
            // While popping, if the stack is empty, raise syntax error
            catch (exception& e) {
                if (strcmp(e.what(), ERR_EMPTY_STACK) == 0) {
                    string e_str = error_string_gen("Conversion syntax error: no matching opening parenthesis with closing at ", index, i_expression[index]);
                    stackFree(&stack);
                    throw runtime_error(e_str);
                }
            }
        }
        
        // Else, raise syntax error
        else {
            string e_str = error_string_gen("Conversion syntax error: Invalid character at ", index, i_expression[index]);
            stackFree(&stack);
            throw runtime_error(e_str);
        }
    }
    
    // Pop all remaining operators from the stack
    while (!stackIsEmpty(stack)) {
        result_expr += ' ';
        result_expr += stackPop(stack);
    }
    
    stackFree(&stack);
    return result_expr;
}
