#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "exception.h"
#include "lexer.h"
#include "types.h"

using namespace std;

int memory = 16;
int line;

string convertToHex(int num) {
    stringstream ss;
    ss << hex << num;
    string imm = ss.str();
    if (imm.length() == 1) imm = "0" + imm;
    return imm;
}

void writeMemory(string statement) {
    cout << convertToHex(memory) << ": " + statement << endl;
    memory++;
}

string tryGrabToken(string* text, token_type type, bool throw_exception) {
    string original_text = *text; // This is terrible design, fix later
    Token t = grabToken(text, line);

    if (type == TERMINATOR) {
        if (t.type != SEMICOLON && t.type != NEWLINE && t.type != END) {
            throwException("Expected end of statement", line);
        }
    }
    else if (t.type != type) {
        if (throw_exception) {
            throwException("Expected " + TokenTypeDescriptorsFull[type], line);
        }
        else {
            *text = original_text; // Return the token, again, terrible
            return "~";
        }
    }

    return t.value;
}

string tryGrabToken(string* text, token_type type) {
    return tryGrabToken(text, type, true);
}

void parse(string text) {
    Token t;
    line = 1;

    while (t.type != END) {
        t = grabToken(&text, line);

        if (t.type == DATA) {
            // This is complicated to implement
        }
        else if (t.type == DEFINE) {
            // This is complicated to implement, and unnecessary right now
            // TL;DR: Do later
        }
        else if (t.type == LABEL) {
            // This is also complicated to implement
        }
        else if (t.type == ADD) {
            string a;
            string b;
            string c;
            
            tryGrabToken(&text, WHITESPACE);
            a = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, COMMA);
            tryGrabToken(&text, WHITESPACE);
            b = tryGrabToken(&text, REGISTER);

            string v = tryGrabToken(&text, COMMA, false);
            if (v == "~") { // The next token was NOT a comma
                tryGrabToken(&text, TERMINATOR);
                writeMemory("1" + a + a + b);
            }
            else {
                tryGrabToken(&text, WHITESPACE);
                c = tryGrabToken(&text, REGISTER);
                tryGrabToken(&text, TERMINATOR);
                writeMemory("1" + a + b + c);
            }
        }
        else if (t.type == SUB) {
            string a;
            string b;
            string c;
            
            tryGrabToken(&text, WHITESPACE);
            a = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, COMMA);
            tryGrabToken(&text, WHITESPACE);
            b = tryGrabToken(&text, REGISTER);

            string v = tryGrabToken(&text, COMMA, false);
            if (v == "~") { // The next token was NOT a comma
                tryGrabToken(&text, TERMINATOR);
                writeMemory("2" + a + a + b);
            }
            else {
                tryGrabToken(&text, WHITESPACE);
                c = tryGrabToken(&text, REGISTER);
                tryGrabToken(&text, TERMINATOR);
                writeMemory("2" + a + b + c);
            }
        }
        else if (t.type == AND) {
            string a;
            string b;
            string c;
            
            tryGrabToken(&text, WHITESPACE);
            a = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, COMMA);
            tryGrabToken(&text, WHITESPACE);
            b = tryGrabToken(&text, REGISTER);

            string v = tryGrabToken(&text, COMMA, false);
            if (v == "~") { // The next token was NOT a comma
                tryGrabToken(&text, TERMINATOR);
                writeMemory("3" + a + a + b);
            }
            else {
                tryGrabToken(&text, WHITESPACE);
                c = tryGrabToken(&text, REGISTER);
                tryGrabToken(&text, TERMINATOR);
                writeMemory("3" + a + b + c);
            }
        }
        else if (t.type == XOR) {
            string a;
            string b;
            string c;
            
            tryGrabToken(&text, WHITESPACE);
            a = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, COMMA);
            tryGrabToken(&text, WHITESPACE);
            b = tryGrabToken(&text, REGISTER);

            string v = tryGrabToken(&text, COMMA, false);
            if (v == "~") { // The next token was NOT a comma
                tryGrabToken(&text, TERMINATOR);
                writeMemory("4" + a + a + b);
            }
            else {
                tryGrabToken(&text, WHITESPACE);
                c = tryGrabToken(&text, REGISTER);
                tryGrabToken(&text, TERMINATOR);
                writeMemory("4" + a + b + c);
            }
        }
        else if (t.type == LSL) {
            string a;
            string b;
            string c;
            
            tryGrabToken(&text, WHITESPACE);
            a = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, COMMA);
            tryGrabToken(&text, WHITESPACE);
            b = tryGrabToken(&text, REGISTER);

            string has_comma = tryGrabToken(&text, COMMA, false);
            if (has_comma == "~") { // The next token was NOT a comma
                tryGrabToken(&text, TERMINATOR);
                writeMemory("5" + a + a + b);
            }
            else {
                tryGrabToken(&text, WHITESPACE);
                c = tryGrabToken(&text, REGISTER);
                tryGrabToken(&text, TERMINATOR);
                writeMemory("5" + a + b + c);
            }
        }
        else if (t.type == LSR) {
            string a;
            string b;
            string c;
            
            tryGrabToken(&text, WHITESPACE);
            a = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, COMMA);
            tryGrabToken(&text, WHITESPACE);
            b = tryGrabToken(&text, REGISTER);

            string has_comma = tryGrabToken(&text, COMMA, false);
            if (has_comma == "~") { // The next token was NOT a comma
                tryGrabToken(&text, TERMINATOR);
                writeMemory("6" + a + a + b);
            }
            else {
                tryGrabToken(&text, WHITESPACE);
                c = tryGrabToken(&text, REGISTER);
                tryGrabToken(&text, TERMINATOR);
                writeMemory("6" + a + b + c);
            }
        }
        else if (t.type == MOV) {
            // Add functionality
        }
        else if (t.type == LDR) {
            string mem;
            string reg;

            tryGrabToken(&text, WHITESPACE);
            mem = tryGrabToken(&text, MEMORY);
            tryGrabToken(&text, COMMA);
            tryGrabToken(&text, WHITESPACE);
            reg = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, TERMINATOR);

            writeMemory("8" + reg + mem);
        }
        else if (t.type == STR) {
            string mem;
            string reg;

            tryGrabToken(&text, WHITESPACE);
            mem = tryGrabToken(&text, MEMORY);
            tryGrabToken(&text, COMMA);
            tryGrabToken(&text, WHITESPACE);
            reg = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, TERMINATOR);

            writeMemory("9" + reg + mem);
        }
        else if (t.type == BRANCH) {
            // Add functionality
        }
        else if (t.type == BRANCH_ZERO) {
            // Add functionality
        }
        else if (t.type == BRANCH_POSITIVE) {
            // Add functionality
        }
        else if (t.type == BRANCH_REGISTER) {
            string reg;

            tryGrabToken(&text, WHITESPACE);
            reg = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, TERMINATOR);

            writeMemory("E" + reg + "00");
        }
        else if (t.type == BRANCH_LINK) {
            string reg;
            string mem;

            tryGrabToken(&text, WHITESPACE);
            reg = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, COMMA);
            tryGrabToken(&text, WHITESPACE);
            mem = tryGrabToken(&text, MEMORY);
            tryGrabToken(&text, TERMINATOR);

            writeMemory("F" + reg + mem);
        }
        else if (t.type == STDIN) {
            string reg;

            tryGrabToken(&text, WHITESPACE);
            reg = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, TERMINATOR);

            writeMemory("8" + reg + "FF");
        }
        else if (t.type == STDOUT) {
            string reg;

            tryGrabToken(&text, WHITESPACE);
            reg = tryGrabToken(&text, REGISTER);
            tryGrabToken(&text, TERMINATOR);

            writeMemory("9" + reg + "FF");
        }
        else if (t.type == COMMENT || t.type == WHITESPACE) {
            // Do nothing
        }
        else if (t.type == NEWLINE) {
            line++;
        }
        else {
            // Throw an error: Unexpected token
        }
    }
}
