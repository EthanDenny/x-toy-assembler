#include <string>

#include "exception.h"
#include "token.h"

using namespace std;

char peek(string* text, int* index) {
    return (*text)[*index];
}

char consume(string* text, int* index) {
    char ch = peek(text, index);
    *index += 1;
    return ch;
}

bool findToken(string* text, int* index, string sub) {
    if ((int) text->find(sub, *index) == *index) {
        *index += sub.length();
        return true;
    }
    else {
        return false;
    }
}

Token getNextToken(string* text, int* index, int line) {
    Token t;

    if ((int) text->length() <= *index) {
        t.type = END;
    }
    else if (findToken(text, index, " ") || findToken(text, index, "\t")) {
        t.type = WHITESPACE;
        while (findToken(text, index, " ") || findToken(text, index, "\t")) {}
    }
    else if (findToken(text, index, ";")) {
        t.type = SEMICOLON;
    }
    else if (findToken(text, index, ",")) {
        t.type = COMMA;
    }
    else if (findToken(text, index, "{")) {
        t.type = CURLY_BRACE_LEFT;
    }
    else if (findToken(text, index, "}")) {
        t.type = CURLY_BRACE_RIGHT;
    }
    else if (findToken(text, index, ".data")) {
        t.type = DATA;;
    }
    else if (findToken(text, index, ".define")) {
        t.type = DEFINE;
    }
    else if (findToken(text, index, "\n")) {
        t.type = NEWLINE;
    }
    else if (findToken(text, index, "add")) {
        t.type = ADD;
    }
    else if (findToken(text, index, "and")) {
        t.type = AND;
    }
    else if (findToken(text, index, "bz")) {
        t.type = BRANCH_ZERO;
    }
    else if (findToken(text, index, "bp")) {
        t.type = BRANCH_POSITIVE;
    }
    else if (findToken(text, index, "br")) {
        t.type = BRANCH_REGISTER;
    }
    else if (findToken(text, index, "bl")) {
        t.type = BRANCH_LINK;
    }
    else if (findToken(text, index, "b")) {
        t.type = BRANCH;
    }
    else if (findToken(text, index, "lsl")) {
        t.type = LSL;
    }
    else if (findToken(text, index, "lsr")) {
        t.type = LSR;
    }
    else if (findToken(text, index, "ldr")) {
        t.type = LDR;
    }
    else if (findToken(text, index, "mov")) {
        t.type = MOV;
    }
    else if (findToken(text, index, "sub")) {
        t.type = SUB;
    }
    else if (findToken(text, index, "str")) {
        t.type = STR;
    }
    else if (findToken(text, index, "stdin")) {
        t.type = STDIN;
    }
    else if (findToken(text, index, "stdout")) {
        t.type = STDOUT;
    }
    else if (findToken(text, index, "xor")) {
        t.type = XOR;
    }
    else {
        char c = consume(text, index);

        if (c == '/') {
            if (peek(text, index) == '/') {
                t.type = COMMENT;
                char C = '/';
                while (peek(text, index) != '\n' && C != EOF) {
                    C = consume(text, index);
                }
            }
            else if (peek(text, index) == '*') {
                t.type = COMMENT;
                char C;
                while (C != EOF && !(C == '*' && findToken(text, index, "/"))) {
                    C = consume(text, index);
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
                C = consume(text, index);

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
            if (!isdigit(peek(text, index))) {
                throwException("Expected a digit after '#'", line);
            }
            else {
                t.type = IMMEDIATE;
                while (isdigit(peek(text, index))) {
                    t.value += consume(text, index);
                };
            };
        }
        else if (c == '0' && findToken(text, index, "x")) {
            t.type = HEX;

            int i;

            for (i = 0; i < 2; i++) {
                if (isxdigit(peek(text, index))) {
                    t.value += consume(text, index);
                }
                else {
                    throwException("Expected a hex digit", line);
                }
            }
            
            if (isxdigit(peek(text, index))) {
                for (i = 0; i < 2; i++) {
                    if (isxdigit(peek(text, index))) {
                        t.value += consume(text, index);
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
                if (isxdigit(peek(text, index))) {
                    t.value += consume(text, index);
                    if (isxdigit(peek(text, index))) {
                        t.value += consume(text, index);
                        t.type = MEMORY;
                    }
                }
            }
            else if (c == 'r') {
                if (isxdigit(peek(text, index))) {
                    t.value += consume(text, index);
                    t.type = REGISTER;
                }
            }

            if (t.type == UNKNOWN) {
                t.type = LABEL;
                
                while (peek(text, index) == '_' || isalnum(peek(text, index))) {
                    t.value += consume(text, index);
                }

                findToken(text, index, ":");
            }
        }
        else {
            throwException("Unknown token", line);
        }
    }

    if (t.type == UNKNOWN) {
        throwException("Lexer error (unknown token)", line);
    }

    return t;
}
