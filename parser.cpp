#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "types.h"

bool isAlpha(char c) {
    return c == '_' || isalnum(c);
}

char getChar(string* text, int i) {
    if ((int) text->length() > i) {
        return (*text)[i];
    }
    else {
        return EOF;
    }
}

char consume(string* text) {
    char ch = getChar(text, 0);
    
    if (text->length() > 1) {
        *text = text->substr(1, text->length());
    }
    else {
        *text = "";
    }

    return ch;
}

bool tryConsume(string* text, string sub) {
    if (text->rfind(sub, 0) == 0) {
        *text = text->substr(sub.length(), text->length() - sub.length());
        return true;
    }
    else {
        return false;
    }
}

char peek(string* text) {
    return getChar(text, 0);
}

char peekNext(string* text) {
    return getChar(text, 1);
}

Token grabToken(string* text) {
    Token t;

    if (text->length() == 0) {
        t.type = END;
    }
    else if (tryConsume(text, " ") || tryConsume(text, "\t")) {
        t.type = WHITESPACE;
        while (tryConsume(text, " ") || tryConsume(text, "\t")) {}
    }
    else if (tryConsume(text, ";")) {
        t.type = SEMICOLON;
    }
    else if (tryConsume(text, ",")) {
        t.type = COMMA;
    }
    else if (tryConsume(text, "{")) {
        t.type = CURLY_BRACE_LEFT;
    }
    else if (tryConsume(text, "}")) {
        t.type = CURLY_BRACE_RIGHT;
    }
    else if (tryConsume(text, ".data")) {
        t.type = DATA;;
    }
    else if (tryConsume(text, ".define")) {
        t.type = DEFINE;
    }
    else if (tryConsume(text, "\n")) {
        t.type = NEWLINE;
    }
    else if (tryConsume(text, "add")) {
        t.type = ADD;
    }
    else if (tryConsume(text, "and")) {
        t.type = AND;
    }
    else if (tryConsume(text, "bz")) {
        t.type = BRANCH_ZERO;
    }
    else if (tryConsume(text, "bp")) {
        t.type = BRANCH_LINK;
    }
    else if (tryConsume(text, "br")) {
        t.type = BRANCH_REGISTER;
    }
    else if (tryConsume(text, "bl")) {
        t.type = BRANCH_POSITIVE;
    }
    else if (tryConsume(text, "b")) {
        t.type = BRANCH;
    }
    else if (tryConsume(text, "lsl")) {
        t.type = LSL;
    }
    else if (tryConsume(text, "lsr")) {
        t.type = LSR;
    }
    else if (tryConsume(text, "ldr")) {
        t.type = LDR;
    }
    else if (tryConsume(text, "mov")) {
        t.type = MOV;
    }
    else if (tryConsume(text, "sub")) {
        t.type = SUB;
    }
    else if (tryConsume(text, "str")) {
        t.type = STR;
    }
    else if (tryConsume(text, "stdin")) {
        t.type = STDIN;
    }
    else if (tryConsume(text, "stdout")) {
        t.type = STDOUT;
    }
    else if (tryConsume(text, "xor")) {
        t.type = XOR;
    }
    else {
        char c = consume(text);

        if (c == '/') {
            if (peek(text) == '/') {
                t.type = COMMENT;
                char C = '/';
                while (peek(text) != '\n' && C != EOF) {
                    C = consume(text);
                }
            }
            if (peek(text) == '*') {
                t.type = COMMENT;
                char C = '/';
                do {
                    C = consume(text);
                    if (C == '*' && tryConsume(text, "/")) {
                        break;
                    }
                } while (C != EOF);
            }
            // Throw an exception here: Expected a comment
        }
        else if (c == '"') {
            t.type = STRING;

            char C;
            bool escape = false;

            while (C != '"') {
                C = consume(text);

                if (C == '\n' || C == EOF) {
                    // Throw an exception here: Expected closing quotes
                }

                if (escape) {
                    if (C == '0') {
                        t.value += '\0';
                    }
                    else if (C == 'n') {
                        t.value += '\n';
                    }
                }
                else {
                    t.value += C;
                }

                escape = C == '\\';
            }
        }
        else if (c == '#') {
            if (!isdigit(peek(text))) {
                // Throw an exception here: Expected a digit
            }
            else {
                t.type = IMMEDIATE;
                while (isdigit(peek(text))) {
                    t.value += consume(text);
                };
            };
        }
        else if (c == '0' && tryConsume(text, "x")) {
            t.type = HEX;

            int i;
            for (i = 0; i < 4; i++) {
                if (isxdigit(peek(text))) {
                    t.value += consume(text);
                }
                else {
                    // Throw an exception here: Expected a hex digit
                    break;
                }
            }

            for (int j = 0; j < 4 - i; j++) {
                t.value = '0' + t.value;
            }
        }
        else if (c == '_' || isalpha(c)) {
            if (c == 'm') {
                if (isxdigit(peek(text))) {
                    t.value += consume(text);
                    if (isxdigit(peek(text))) {
                        t.value += consume(text);
                        t.type = MEMORY;
                    }
                }
            }
            else if (c == 'r') {
                if (isxdigit(peek(text))) {
                    t.value += consume(text);
                    t.type = REGISTER;
                }
            }

            if (t.type == UNKNOWN) {
                while (isAlpha(peek(text))) {
                    t.value += consume(text);
                }

                if (tryConsume(text, ":")) {
                    t.type = LABEL;
                }
                else {
                    // Throw an exception here: Unknown token
                }
            }
        }
        else {
            // Throw an exception here: Unknown token
        }
    }

    return t;
}

vector<Token> parse(string text) {
    vector<Token> tokens;

    while (true) {
        Token nextToken = grabToken(&text);
        
        if (nextToken.type != END) {
            tokens.push_back(nextToken);
        }

        if (nextToken.type == END) {
            break;
        }
    }

    return tokens;
}

void printLineNum(int n) {
    if (n < 10) {
        cout << "00";
    }
    else if (n < 100) {
        cout << "0";
    }
    cout << n << ": ";
}

void prettyParse(string text, bool use_full) {
    vector<Token> tokens = parse(text);

    int token_count = (int) tokens.size();

    int j = 1;
    printLineNum(j);

    for (int i = 0; i < token_count; i++) {
        if (tokens[i].type == NEWLINE) {
            if (i + 1 < token_count) {
                j++;
                cout << endl;
                printLineNum(j);
            }
        }
        else if (!use_full) {
            cout << TokenTypeDescriptors[tokens[i].type] << " ";
        }
        else {
            cout << TokenTypeDescriptorsFull[tokens[i].type] << " ";
        }
    }
    cout << endl;
}

void prettyParse(string text, int mode, bool use_full) {
    if (mode == 0) {
        prettyParse(text, use_full);
        return;
    }
    if (mode >= 1) {
        vector<Token> tokens = parse(text);

        int token_count = (int) tokens.size();

        int j = 1;

        for (int i = 0; i < token_count; i++) {
            if (tokens[i].type == NEWLINE && mode == 1) {
                if (i + 1 < token_count) {
                    j++;
                    cout << endl;
                }
            }
            else if (!use_full) {
                cout << TokenTypeDescriptors[tokens[i].type] << " ";
            }
            else {
                cout << TokenTypeDescriptorsFull[tokens[i].type] << " ";
            }
        }
        cout << endl;
    }
}

void prettyParse(string text, int mode) {
    prettyParse(text, mode, false);
}

void testFullParse(bool use_full) {
    prettyParse(".data", 2, use_full);
    prettyParse(".define", 2, use_full);
    prettyParse("// Hello, world", 2, use_full);
    prettyParse("label_this_could_be_anything:", 2, use_full);

    cout << endl;

    prettyParse("rF", 2, use_full);
    prettyParse("m0F", 2, use_full);
    prettyParse("#1", 2, use_full);
    prettyParse("0x00FF", 2, use_full);
    prettyParse("\"Hello, world\"", 2, use_full);

    cout << endl;

    prettyParse("add", 2, use_full);
    prettyParse("sub", 2, use_full);
    prettyParse("and", 2, use_full);
    prettyParse("xor", 2, use_full);
    prettyParse("lsl", 2, use_full);
    prettyParse("lsr", 2, use_full);

    cout << endl;

    prettyParse("mov", 2, use_full);
    prettyParse("ldr", 2, use_full);
    prettyParse("str", 2, use_full);

    cout << endl;

    prettyParse("b", 2, use_full);
    prettyParse("bz", 2, use_full);
    prettyParse("bp", 2, use_full);
    prettyParse("br", 2, use_full);
    prettyParse("bl", 2, use_full);

    cout << endl;

    prettyParse("stdin", 2, use_full);
    prettyParse("stdout", 2, use_full);

    cout << endl;

    prettyParse("\n", 2, use_full);
    prettyParse(";", 2, use_full);
    prettyParse(",", 2, use_full);
    prettyParse("{", 2, use_full);
    prettyParse("}", 2, use_full);
    prettyParse(" \t   \t    \t\t ", 2, use_full);
}

string readFile(string filename) {
    ifstream file(filename);

    string content;
    char ch;

    while (file.get(ch)) {
        content += ch;
    }

    file.close();

    return content;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        cout << "Use: ./parser <filename> <mode> <full>" << endl;
        cout << "   <mode>:" << endl;
        cout << "       0: Use line numbers and newlines" << endl;
        cout << "       1: Use newlines" << endl;
        cout << "       2: Use nothing" << endl;
        cout << "   <full>:" << endl;
        cout << "       0: Use shortened token names" << endl;
        cout << "       1: Use full token names" << endl;
        return 0;
    }
    else if (argc < 4) {
        cout << "Not enough args";
        return 0;
    }

    string filename = argv[1];
    string assembly_code = readFile(filename);

    prettyParse(assembly_code, atoi(argv[2]), atoi(argv[3])==1);

    return 0;
}
