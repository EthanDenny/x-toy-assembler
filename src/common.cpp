#include <string>
#include <sstream>

#include "exception.h"

using namespace std;

string decimalToHex(int num) {
    stringstream ss;
    ss << hex << num;
    string imm = ss.str();
    for (auto &c: imm) c = (char) toupper(c); // Convert to uppercase
    if (imm.length() == 1) imm = "0" + imm;
    return imm;
}

int hexToDecimal(string hex_str) {
    int x;
    stringstream ss;
    ss << hex << hex_str;
    ss >> x;
    return x;
}

bool isWhitespace(char c) {
    return c == ' ' || c == '\t';
}

char peek(string* text, int* code_index) {
    return (*text)[*code_index];
}

char consume(string* text, int* code_index) {
    char ch = peek(text, code_index);
    *code_index += 1;
    return ch;
}

string resolveEscapeSeqs(string original, int line) {
    stringstream resolved;

    for (size_t i = 0; i < original.length() - 1; i++) {
        char ch = original[i];

        if (ch == '\\') {
            i++;
            char ch2 = original[i];

            if (ch2 == '\\') {
                resolved << '\\';
            }
            else if (ch2 == '0') {
                resolved << '\0';
            }
            else if (ch2 == 't') {
                resolved << '\t';
            }
            else if (ch2 == 'n') {
                resolved << '\n';
            }
            else {
                string s;
                s += ch2;
                throwException("Unknown escape sequence '\\" + s + "' ", line);
            }
        }
        else {
            resolved << ch;
        }
    }

    return resolved.str();
}
