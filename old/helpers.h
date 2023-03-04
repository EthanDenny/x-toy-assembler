#ifndef HELPERS

#include <string>
#include <variant>

using namespace std;

bool checkPattern(vector<Token> tokens, int i, int pattern[], int size) {
    for (int j = 0; j < size; j++) {
        if (tokens.at(i+j).type != pattern[j]) {
            return false;
        }
    }
    return true;
}

string convertToHex(string decimal) {
    stringstream ss;
    ss << hex << decimal;
    string imm = ss.str();
    if (imm.length() == 1) imm = "0" + imm;
    return imm;
}

string convertToHex(int decimal) {
    return convertToHex(to_string(decimal));
}

bool isAlpha(char c) {
    return c == '_' || isalnum(c);
}

bool isALI(token_type type) {
    return type == ADD || type == SUB || type == AND || type == XOR || type == LSL || type == LSR;
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

char getChar(string* text, int i) {
    if ((*text).length() > 0) {
        return (*text)[0];
    }
    else {
        return EOF;
    }
}

char consume(string* text) {
    char ch = getChar(text, 0);
    
    if ((*text).length() > 1) {
        *text = (*text).substr(1, (*text).length());
    }
    else {
        *text = "";
    }

    return ch;
}

bool tryConsume(string* text, string sub) {
    if ((*text).rfind(sub, 0) == 0) {
        *text = (*text).substr(sub.length(), (*text).length() - sub.length());
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

Token r0() {
    Token t = Token();
    t.type = REGISTER;
    t.value = '0';
    return t;
}

Token mFF() {
    Token t = Token();
    t.type = MEMORY;
    t.value = "FF";
    return t;
}

#endif
