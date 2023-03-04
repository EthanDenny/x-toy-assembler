#include "lexer.h"
#include "types.h"

using namespace std;

void parse(string text) {
    Token t;
    int line = 1;

    while (t.type != END) {
        t = grabToken(&text, line);

        if (t.type == UNKNOWN) {
            // Throw an error: Unknown token, which should have been handled by the lexer
        }
        else if (t.type == DATA) {
            // Add functionality
        }
        else if (t.type == DEFINE) {
            // Add functionality
        }
        else if (t.type == LABEL) {
            // Add functionality
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
            // Add functionality
        }
        else if (t.type == BRANCH_LINK) {
            // Add functionality
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
