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
    Token t = grabToken(text, line);

    if (type == TERMINATOR) {
        if (t.type != SEMICOLON && t.type != NEWLINE && t.type != END) {
            throwException("Expected end of statement", line);
        }
    }
    else if (t.type != type && throw_exception) {
        throwException("Expected " + TokenTypeDescriptorsFull[type], line);
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
            // Add functionality
        }
        else if (t.type == SUB) {
            // Add functionality
        }
        else if (t.type == AND) {
            // Add functionality
        }
        else if (t.type == XOR) {
            // Add functionality
        }
        else if (t.type == LSL) {
            // Add functionality
        }
        else if (t.type == LSR) {
            // Add functionality
        }
        else if (t.type == MOV) {
            // Add functionality
        }
        else if (t.type == LDR) {
            // Add functionality
        }
        else if (t.type == STR) {
            // Add functionality
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
            // Add functionality
        }
        else if (t.type == STDOUT) {
            // Add functionality
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
