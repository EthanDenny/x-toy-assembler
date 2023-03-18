#include <string>

#include "exception.h"
#include "token.h"
#include "common.h"

using namespace std;

bool findNext(string* text, int* code_index, string sub) {
    if ((int) text->find(sub, *code_index) == *code_index) {
        *code_index += sub.length();
        return true;
    }
    else {
        return false;
    }
}

Token getNextToken(string* text, int* code_index, int line) {
    Token t;

    if ((int) text->length() <= *code_index) {
        t.type = END;
    }
    else if (isWhitespace(peek(text, code_index))) {
        t.type = WHITESPACE;
        while (isWhitespace(peek(text, code_index))) {
            consume(text, code_index);
        }
    }
    else if (findNext(text, code_index, ";")) {
        t.type = SEMICOLON;
    }
    else if (findNext(text, code_index, ",")) {
        t.type = COMMA;
    }
    else if (findNext(text, code_index, "{")) {
        t.type = CURLY_BRACE_LEFT;
    }
    else if (findNext(text, code_index, "}")) {
        t.type = CURLY_BRACE_RIGHT;
    }
    else if (findNext(text, code_index, ".data")) {
        t.type = DATA;;
    }
    else if (findNext(text, code_index, ".define")) {
        t.type = DEFINE;
    }
    else if (findNext(text, code_index, "halt")) {
        t.type = HALT;
    }
    else if (findNext(text, code_index, "\n")) {
        t.type = NEWLINE;
    }
    else if (findNext(text, code_index, "add")) {
        t.type = ADD;
    }
    else if (findNext(text, code_index, "and")) {
        t.type = AND;
    }
    else if (findNext(text, code_index, "bz")) {
        t.type = BRANCH_ZERO;
    }
    else if (findNext(text, code_index, "bp")) {
        t.type = BRANCH_POSITIVE;
    }
    else if (findNext(text, code_index, "br")) {
        t.type = BRANCH_REGISTER;
    }
    else if (findNext(text, code_index, "bl")) {
        t.type = BRANCH_LINK;
    }
    else if (findNext(text, code_index, "b")) {
        t.type = BRANCH;
    }
    else if (findNext(text, code_index, "lsl")) {
        t.type = LSL;
    }
    else if (findNext(text, code_index, "lsr")) {
        t.type = LSR;
    }
    else if (findNext(text, code_index, "ldr")) {
        t.type = LDR;
    }
    else if (findNext(text, code_index, "mov")) {
        t.type = MOV;
    }
    else if (findNext(text, code_index, "sub")) {
        t.type = SUB;
    }
    else if (findNext(text, code_index, "str")) {
        t.type = STR;
    }
    else if (findNext(text, code_index, "stdin")) {
        t.type = STDIN;
    }
    else if (findNext(text, code_index, "stdout")) {
        t.type = STDOUT;
    }
    else if (findNext(text, code_index, "xor")) {
        t.type = XOR;
    }
    else {
        char c = consume(text, code_index);

        if (c == '/') {
            if (peek(text, code_index) == '/') {
                t.type = COMMENT;
                char C = '/';
                while (peek(text, code_index) != '\n' && C != EOF) {
                    C = consume(text, code_index);
                }
            }
            else if (peek(text, code_index) == '*') {
                t.type = COMMENT;
                char C;
                while (C != EOF && !(C == '*' && findNext(text, code_index, "/"))) {
                    C = consume(text, code_index);
                }
            }
            else {
                throwException("Expected a comment", line);
            }
        }
        else if (c == '"') {
            t.type = STRING;

            char C;
            bool escape = false;

            while (C != '"') {
                C = consume(text, code_index);

                if (C == '\n' || C == EOF) {
                    throwException("Expected closing quotes", line);
                }

                if (escape) {
                    if (C == 'n') {
                        t.value += '\n';
                    }
                }
                else if (C != '"') {
                    t.value += C;
                }

                escape = C == '\\';
            }
        }
        else if (c == '#') {
            if (!isdigit(peek(text, code_index))) {
                throwException("Expected a digit after '#'", line);
            }
            else {
                t.type = IMMEDIATE;
                while (isdigit(peek(text, code_index))) {
                    t.value += consume(text, code_index);
                };
            };
        }
        else if (c == '0' && findNext(text, code_index, "x")) {
            t.type = HEX;

            int i;

            for (i = 0; i < 2; i++) {
                if (isxdigit(peek(text, code_index))) {
                    t.value += consume(text, code_index);
                }
                else {
                    throwException("Expected a hex digit", line);
                }
            }
            
            if (isxdigit(peek(text, code_index))) {
                for (i = 0; i < 2; i++) {
                    if (isxdigit(peek(text, code_index))) {
                        t.value += consume(text, code_index);
                    }
                    else {
                        throwException("Expected a hex digit", line);
                    }
                }
            }
            else {
                t.value = "00" + t.value;
            }
        }
        else if (c == '_' || isalpha(c)) {
            if (c == 'm') {
                if (isxdigit(peek(text, code_index))) {
                    t.value += consume(text, code_index);
                    if (isxdigit(peek(text, code_index))) {
                        t.value += consume(text, code_index);
                        t.type = MEMORY;
                    }
                }
            }
            else if (c == 'r') {
                if (isxdigit(peek(text, code_index))) {
                    t.value += consume(text, code_index);
                    t.type = REGISTER;
                }
            }

            if (t.type == UNKNOWN) {
                t.type = LABEL;
                
                while (peek(text, code_index) == '_' || isalnum(peek(text, code_index))) {
                    t.value += consume(text, code_index);
                }

                findNext(text, code_index, ":");
            }
        }
        else {
            throwException("Unknown token", line);
        }
    }

    if (t.type == UNKNOWN) {
        throwException("Internal error (unknown token)", line);
    }

    return t;
}
