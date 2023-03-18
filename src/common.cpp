#include <string>
#include <sstream>

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