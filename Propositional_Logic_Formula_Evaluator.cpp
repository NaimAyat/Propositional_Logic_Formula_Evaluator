// Written by Naim Ayat
// November 2015
// For Professor Ravikumar, Sonoma State University

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <map>

// Typedefs to make things less verbose.
typedef std::string string;
typedef std::vector<string> stringVector;
typedef stringVector::const_iterator stringIter;
typedef std::map<string, bool> boolMap;

// Parses the input into tokens. "(a V b) ^ c <-> d" becomes
// a vector with the following elements: "(", "a", "V", "b", ")", "^", "c", "<->", and "d".
stringVector getTokens(const string& inputString);

// Boolean functions for Shunting-Yard algorithm.
bool isOperator(const char& ch);
bool isParentheses(const char& ch);
bool isVariable(const string& s);
bool isOperator(const string& s);
bool isLeftParen(const string& s);
bool isRightParen(const string& s);
int getPrecedence(const string& s);

// Obtains the variables from the vector of tokens. A variable is any
// alphabet character that isn't "V", "f", or "t". Those are reserved for
// the OR operator, logical falsity, and logical truth, respectively.
stringVector getVariables(const stringVector& tokenVector);

// Uses Shunting-Yard Algorithm to turn infix notation into Reverse Polish Notation.
stringVector convertRPN(const stringVector& tokenVector);

// Given a vector in RPN and a map of Boolean values, evaluate the result.
bool evalRPN(const stringVector& tokenVector, const boolMap& bmap);

// Uses inside evalRPN to evaluate individual pairs of arguments.
string evalExpression(const string& arg1, const string& arg2,
                      const string& op, const boolMap& bmap);

// Given an integer value, take the bits of that integer and use them to
// set the truth-false values in bmap.
// For example, providing 5 (101) as the generator integer and the elements of
// "p", "q", and "r" in the vector of variables will make bmap set the following
// values: bmap["p"] = true, bmap["q"] = false, bmap["r"] = true.
void setValues(boolMap& bmap, const stringVector& variableVector, int comboGen);

// Combines two vectors, eliminating duplicates,
// and also sorts them. This will then get plugged into setValues to generate 
// truth-false tables for all variables.
stringVector combineVectors(const stringVector& vector1, 
                            const stringVector& vector2);

void printBool(bool b);
void printSpaces(int n);

int main() {
    stringVector tokenVector1, tokenVector2;
    stringVector variableVector1, variableVector2;
    stringVector variableVectorUnion;
    stringVector RPNVector1, RPNVector2;
    string inputString1, inputString2;
    boolMap bmap;
    bool result1, result2;

    std::cout << "This program will build a truth table and determine whether an implication is a tautology.\n\n";
    std::cout << "Valid commands (case-sensitive): \n   ~ (negation)\n   V (or)\n   ^ (and)\n   -> (implies)\n   <-> (iff)\n   XOR (exclusive or)\n   t (true statement)\n   f (false statement)\n   Parentheses also allowed.\n\n";
	std::cout << "Input a premise: ";
    getline(std::cin, inputString1);
    std::cout << "Input a conclusion: ";
    getline(std::cin, inputString2);
	std::cout << "\n";
    tokenVector1 = getTokens(inputString1);
    tokenVector2 = getTokens(inputString2);

    variableVector1 = getVariables(tokenVector1);
    variableVector2 = getVariables(tokenVector2);
    variableVectorUnion = combineVectors(variableVector1, variableVector2);

    RPNVector1 = convertRPN(tokenVector1);
    RPNVector2 = convertRPN(tokenVector2);
    
    bool validConclusion = true;

    // Set initial values of bmap. "f" and "t" never change. ComboGen
    // is initially set to make every variable "true."
    bmap["f"] = false;
    bmap["t"] = true;

    int comboGen = 1;

    for(int i = 0; i < variableVectorUnion.size(); i++) {
        comboGen *= 2;
    }

    comboGen--;

    // Print out the top part of the output table.

    int colWidth1 = inputString1.length();
    int colWidth2 = inputString2.length();

    for(stringIter i = variableVectorUnion.begin(); i !=
            variableVectorUnion.end(); ++i) {
        std::cout << *i << " ";
    }
    std::cout << "| ";

    std::cout << inputString1 << " | " << inputString2 << "\n";

    // Print the truth table values.
    while(comboGen >= 0) {
        setValues(bmap, variableVectorUnion, comboGen);
        result1 = evalRPN(RPNVector1, bmap);
        result2 = evalRPN(RPNVector2, bmap);
		
        for(stringIter i = variableVectorUnion.begin();
                i != variableVectorUnion.end(); ++i) {
            printBool(bmap.at(*i)); 
            std::cout << " ";
        }

        std::cout << "|";
        printSpaces(colWidth1 / 2);

        if(inputString1.length() % 2 == 1) {
            std::cout << " ";
        }

        printBool(result1);
        printSpaces(colWidth1 / 2);

        std::cout << " |";
        printSpaces(colWidth2 / 2); 

        if(inputString2.length() % 2 == 1) {
            std::cout << " ";
        }

        printBool(result2);
        printSpaces(colWidth2 / 2);

        if(result1 = 1 and result1 != result2){
            	if(result1!=result2){
					validConclusion = false;
					std::cout << " Premise does not imply conclusion in this case.";
				}
        }

        std::cout << "\n";
        comboGen--;
    }

    if(validConclusion) {
        std::cout << "\nThe statement [(" << inputString1 << ")" << " " << "->" << " " << "(" << inputString2 << ")]";
        std::cout << "\nis a valid tautology. Premise implies conclusion in every case.\n";
        system("pause");
    }

    else {
    	std::cout << "\nThe statement [(" << inputString1 << ")" << " " << "->" << " " << "(" << inputString2 << ")]";
        std::cout << "\nis not a tautology. Invalid cases are labelled.\n";
        system("pause");
    }

    return 0;
}

stringVector getTokens(const string& inputString) {
    stringVector tokenVector;
    string currentString = "";

    int i = 0;

    while(i < inputString.length()) {

        currentString = "";

        // Test for "implies" operator.
        if(inputString[i] == '-' && inputString.length() - i > 1) {
            if(inputString.substr(i, 2) == "->") {
                tokenVector.push_back("->");
                i += 2;
            }

            else {
                i++;
            }
        }

        // Test for "XOR" operator.
        else if(inputString[i] == 'X' && inputString.length() - i > 2) {
            if(inputString.substr(i, 3) == "XOR") {
                tokenVector.push_back("XOR");
                i += 3;
            }

            else {
                i++;
            }
        }

        // Test for "iff" operator.

        else if(inputString[i] == '<' && inputString.length() - i > 2) {
            if(inputString.substr(i, 3) == "<->") {
                tokenVector.push_back("<->");
                i += 3;
            }

            else {
                i++;
            }
        }

        // Otherwise, it's ignored. 
        else if(isOperator(inputString[i]) || isParentheses(inputString[i]) 
                                           || isalpha(inputString[i])) {
            currentString += inputString[i];
            tokenVector.push_back(currentString);
            i++;
        }

        else {
            i++;
        }
    }

    return tokenVector;
}

bool isOperator(const char& ch) {
    char validOperators[] = {'~', 'V', '^'};

    for(int i = 0; i < 3; i++) {
        if(ch == validOperators[i]) {
            return true;
        }
    }

    return false;
}

bool isParentheses(const char& ch) {
    return ch == '(' || ch == ')';
}

bool isVariable(const string& s) {
    return s.length() == 1 && isalpha(s[0]) && s[0] != 'V';
}

bool isOperator(const string& s) {
    string validOperators[] = {"~", "V", "^", "->", "<->", "XOR"};

    for(int i = 0; i < 6; i++) {
        if(s == validOperators[i]) {
            return true;
        }
    }

    return false;
}

bool isLeftParen(const string& s) {
    return s.length() == 1 && s[0] == '(';
}

bool isRightParen(const string& s) {
    return s.length() == 1 && s[0] == ')';
}

int getPrecedence(const string& s) {
    if(s == "<->") {
        return 0;
    }

    if(s == "->") {
        return 1;
    }

    if(s == "XOR") {
        return 2;
    }

    if(s == "V") {
        return 3;
    }

    if(s == "^") {
        return 4;
    }

    if(s == "~") {
        return 5;
    }

    else {
        return -1;
    }
}

stringVector getVariables(const stringVector& tokenVector) {
    stringVector variableVector;
    for(stringIter i = tokenVector.begin(); i != tokenVector.end(); ++i) {

        if(i->length() == 1 && isalpha(i->at(0)) && i->at(0) != 'V' &&
                                 i->at(0) != 'f' && i->at(0) != 't') {

            if(!(std::find(variableVector.begin(), variableVector.end(), *i) !=
                    variableVector.end())) {
                variableVector.push_back(*i);
            }
        }
    }

    return variableVector;
}

stringVector convertRPN(const stringVector& tokenVector) {
    stringVector RPNVector;
    std::stack<string> operatorStack;
    int precedence;

    for(stringIter i = tokenVector.begin(); i != tokenVector.end(); ++i) {
        if(isVariable(*i)) {
            RPNVector.push_back(*i);
        }

        else if(isOperator(*i)) {
            precedence = getPrecedence(*i);

            if(precedence == -1) {
                std::cout << "\nError: Invalid operator plugged into precedence function.\n";
                system("pause");
                exit (EXIT_FAILURE);
            }

            while(!(operatorStack.empty()) && 
                    precedence <= getPrecedence(operatorStack.top())) {
                RPNVector.push_back(operatorStack.top());
                operatorStack.pop();
            }

            operatorStack.push(*i);
        }

        else if(isLeftParen(*i)) {
            operatorStack.push(*i);
        }

        else if(isRightParen(*i)) {
            if(operatorStack.empty()) {
                std::cout << "Parentheses mismatch.\n";
                system("pause");
                exit (EXIT_FAILURE);
            }

            while(operatorStack.top() != "(") {
                if(operatorStack.empty()) {
                    std::cout << "Parentheses mismatch.\n";
                    system("pause");
                    exit (EXIT_FAILURE);
                }

                RPNVector.push_back(operatorStack.top());
                operatorStack.pop();
            }

            operatorStack.pop();
        }
    }

    while(!(operatorStack.empty())) {
        RPNVector.push_back(operatorStack.top());
        operatorStack.pop();
    }

    return RPNVector;
}

bool evalRPN(const stringVector& RPNVector, const boolMap& bmap) {
    std::stack<string> variableStack;
    string arg1, arg2;

    if(RPNVector.empty()) {
        return false;
    }

    // If it's just one variable in the statement, return the variable.
    if(RPNVector.size() == 1) {
        if(evalExpression(RPNVector[0], "", "", bmap) == "t") {
            return true;
        }

        else {
            return false;
        }
    }

    for(stringIter i = RPNVector.begin(); i != RPNVector.end(); ++i) {
        if(isVariable(*i)) {
            variableStack.push(*i);
        }

        // Otherwise, it's an operator.
        else {
            // Unary operator takes one argument.
            if(*i == "~") {
                if(variableStack.size() > 0) {
                    arg1 = variableStack.top();
                    variableStack.pop();
                    arg2 = "";
                    // Evaluate the arguments and push the result onto the
                    // stack.
                    variableStack.push(evalExpression(arg1, arg2, *i, bmap));
                }

                else {
                    std::cout << "\nError: Invalid input. Not enough arguments for inputted operators.\n";
                    system("pause");
                    exit (EXIT_FAILURE);
                }
            }

            // Otherwise, it's a binary operator and takes two.
            else {
                if(variableStack.size() > 1) {
                    arg1 = variableStack.top();
                    variableStack.pop();
                    arg2 = variableStack.top();
                    variableStack.pop();
                    variableStack.push(evalExpression(arg2, arg1, *i, bmap));
                }

                else {
                    std::cout << "\nError: Invalid input. Not enough arguments for inputted operators.\n";
                    system("pause");
                    exit (EXIT_FAILURE);
                }
            }
        }
    }

    // We should have a single expression at this point - either f or t.

    if(variableStack.size() != 1) {
        std::cout << "\nError: Invalid input. More arguments than inputted operators allow.\n";
        system("pause");
        exit (EXIT_FAILURE);
    }

    if(variableStack.top() == "t") {
        return true;
    }

    else {
        if(variableStack.top() != "f") {
            std::cout << "\nError: Invalid input. Does not simplify to t or f.\n";
            system("pause");
			exit (EXIT_FAILURE);
        }
        return false;
    }
}

string evalExpression(const string& arg1, const string& arg2,
                           const string& op, const boolMap& bmap) {
    bool bArg1, bArg2;
    std::string result;

    if(op == "") {
        bArg1 = bmap.at(arg1);
        if(bArg1) {
            return "t";
        }

        else {
            return "f";
        }
    }

    else if(op == "~") {
        bArg1 = bmap.at(arg1);
        if(bArg1) {
            result = "f";
        }

        else {
            result = "t";
        }
    }

    else {
        bArg1 = bmap.at(arg1);
        bArg2 = bmap.at(arg2);

        // Logical AND.
        if(op == "^") {
            if(bArg1 && bArg2) {
                result = "t";
            }

            else {
                result = "f";
            }
        }

        // Logical OR.
        else if(op == "V") {
            if(bArg1 || bArg2) {
                result = "t";
            }

            else {
                result = "f";
            }
        }

        // Logical XOR.
        else if(op == "XOR") {
            if((bArg1 || bArg2) && !(bArg1 && bArg2)) {
                result = "t";
            }

            else {
                result = "f";
            }
        }

        // Implies operator.
        else if(op == "->") {
            if(!bArg1 || bArg2) {
                result = "t";
            }

            else {
                result = "f";
            }
        }

        // If and only if operator.
        else if(op == "<->") {
            if((bArg1 && bArg2) || (!bArg1 && !bArg2)) {
                result = "t";
            }

            else {
                result = "f";
            }
        }

        else {
            std::cout << "\nError: Invalid operator.\n";
            system("pause");
            exit (EXIT_FAILURE);
        }
    }

    // std::cout << result << "\n";
    return result;
}

void setValues(boolMap& bmap, const stringVector& variableVector, 
                                                  int comboGen) {
    int j = 0;
    for(stringIter i = variableVector.begin(); i != variableVector.end(); i++) {
        if((comboGen >> j) & 0x01) {
            bmap[*i] = true;
        }

        else {
            bmap[*i] = false;
        }
        j++;
    }

    return;
}

stringVector combineVectors(const stringVector& vector1, 
                            const stringVector& vector2) {
    stringVector newVector = vector1;

    for(stringIter i = vector2.begin(); i != vector2.end(); ++i) {
        if(std::find(newVector.begin(), 
                     newVector.end(), *i) == newVector.end()) {
            newVector.push_back(*i);
        }
    }

    std::sort(newVector.begin(), newVector.end());

    return newVector;
}

void printBool(bool b) {
    if(b) {
        std::cout << "t";
    }

    else {
        std::cout << "f";
    }

    return;
}

void printSpaces(int n) {
    for(int i = 0; i < n; i++) {
        std::cout << " ";
    }
    return;
}