#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

#include "types.h"
#include "helpers.h"

using namespace std;

bool reserved_mem[256];
int next_open_instr = 0x10;

bool isMemFree(int locale) {
    return !reserved_mem[locale];
}

bool reserveMem(int locale) {
    reserved_mem[locale] = true;
}

void writeMemory(map<int, string>* mem_values, string value, int locale) {
    if (!isMemFree(locale)) {
        (*mem_values).insert(pair<int, string>(locale, value));
        reserveMem(locale);
        if (locale > next_open_instr) {
            next_open_instr = locale;
        }
        else if (locale == next_open_instr) {
            next_open_instr++;
        }
    }
}

void writeMemory(map<int, string>* mem_values, string value) {
    writeMemory(mem_values, value, next_open_instr);
}

void writeMemory(map<int, string>* mem_values, string value, string locale, int offset) {
    int i_locale = offset;

    for (int i = 0; i < 2; i++) {
        char c = locale[i];
        if (!isxdigit(c)) {
            // Throw an exception: expected a hex digit
        }
        if (c < 58) i_locale += c - 48;
        else if (c < 71) i_locale += c - 55;
        else if (c < 103) i_locale += c - 87;
    }

    writeMemory(mem_values, value, i_locale);
}

void writeMemory(map<int, string>* mem_values, string value, string locale) {
    writeMemory(mem_values, value, locale, 0);
}

void outputMemory(map<int, string> mem_values) {
    bool has_const = false;
    for (int i = 0; i < 256; i++) {
        if (i == 16 && has_const) cout << endl;
        if (mem_values.count(i)) {
            if (i < 16) {has_const = true;}
            cout << convertToHex(i) << ": " << mem_values[i] << endl;
        }
    }
}

Token grabToken(string* text) {
    char c = consume(text);
    Token t;

    switch (c) {
        case '/': {
            if (peek(text) == '/') {
                t.type = COMMENT;
                char C = '/';
                do {
                    C = consume(text);
                } while (C != '\n' && C != EOF);
                break;
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
                break;
            }
            break;
            // Throw an exception here, expected a comment
        }
        case '"': {
            t.type = STRING;
            bool special = false;
            char C = consume(text);
            do {
                if (!special) {
                    t.value += C;
                }
                else {
                    switch (C) {
                       case '0': t.value += '\0';
                    }
                }
                C = consume(text);
                special = C == '\\';
                if (special) {
                    C = consume(text);
                }
            } while (C != '"');
            break;
            // Throw an exception here, expected a comment
        }
        case ';':
            t.type = SEMICOLON;
            break;
        case ',':
            t.type = COMMA;
            break;
        case 'a': {
            if (tryConsume(text, "dd")) {
                t.type = ADD;
                break;
            }
            else if (tryConsume(text, "nd")) {
                t.type = AND;
                break;
            }
        }
        case 'b': {
            if (tryConsume(text, "z")) {
                t.type = BRANCH_ZERO;
                break;
            }
            else if (tryConsume(text, "p")) {
                t.type = BRANCH_LINK;
                break;
            }
            else if (tryConsume(text, "l")) {
                t.type = BRANCH_POSITIVE;
                break;
            }
            else {
                t.type = BRANCH;
                break;
            }
        }
        case 'm': {
            if (tryConsume(text, "ov")) {
                t.type = MOV;
                break;
            }
            else if (isxdigit(peek(text)) && isxdigit(peekNext(text))) {
                t.type = MEMORY;
                t.value += consume(text);
                t.value += consume(text);
                break;
            }
        }
        case 's': {
            if (tryConsume(text, "ub")) {
                t.type = SUB;
                break;
            }
            else if (tryConsume(text, "tr")) {
                t.type = STR;
                break;
            }
            else if (tryConsume(text, "tdin")) {
                t.type = STDIN;
                break;
            }
            else if (tryConsume(text, "tdout")) {
                t.type = STDOUT;
                break;
            }
        }
        case 'l': {
            if (tryConsume(text, "sl")) {
                t.type = LSL;
                break;
            }
            else if (tryConsume(text, "sr")) {
                t.type = LSR;
                break;
            }
            else if (tryConsume(text, "dr")) {
                t.type = LDR;
                break;
            }
        }
        case 'x': {
            if (tryConsume(text, "or")) {
                t.type = XOR;
                break;
            }
        }
        case '#': {
            t.type = IMMEDIATE;
            if (!isdigit(peek(text))) {
                // Throw an exception here: Expected a digit
            }
            do {
                t.value += consume(text);
            } while (isdigit(peek(text)));
            break;
        }
        case 'r': {
            if (isxdigit(peek(text))) {
                t.type = REGISTER;
                t.value = consume(text);
                break;
            }
        }
        case '0': {
            if (tryConsume(text, "x")) {
                t.type = HEX;

                int i = 0;
                char C = consume(text);
                if (!isxdigit(C)) {
                    // Throw an exception here: Expected a hex digit
                }

                while (i < 4 && isxdigit(C)) {
                    t.value += C;
                    C = consume(text);
                    i++;
                }

                for (int j = 0; j < 4 - i; j++) {
                    t.value = '0' + t.value;
                }

                break;
            }
        }
        case '\n': {
            t.type = NEWLINE;
            break;
        }
        case ':': {
            t.type = COLON;
            break;
        }
        case ' ': {
            t.type = WHITESPACE;
            break;
        }
        case '\t': {
            t.type = WHITESPACE;
            break;
        }
        case '{': {
            t.type = CURLY_BRACE_LEFT;
            break;
        }
        case '}': {
            t.type = CURLY_BRACE_RIGHT;
            break;
        }
        case '.': {
            if (tryConsume(text, "data")) {
                t.type = DATA;
                break;
            }
        }
        default: {
            if (isAlpha(c) && !isdigit(c)) {
                t.type = LABEL;
                t.value = c;
                while (isAlpha(peek(text))) {
                    t.value += consume(text);
                }
            }

            // Throw an exception here: Unknown token
        }
    }

    return t;
}

vector<Token> parse(string text) {
    vector<Token> tokens;

    while (true) {
        Token nextToken = grabToken(&text);
        
        if (nextToken.type != WHITESPACE) {
            tokens.push_back(nextToken);
        }
        if (nextToken.type == NONE) {break;}
    }

    return tokens;
}

vector<Statement> lex(vector<Token> tokens) {
    vector<Statement> statements;
    int i = 0;

    while (i < tokens.size()) {
        Token t = tokens.at(i);
        Statement s;
        s.type = t.type;

        if (isALI(t.type)) {
            int patternA[4] = {t.type, REGISTER, COMMA, REGISTER};
            int patternB[6] = {t.type, REGISTER, COMMA, REGISTER, COMMA, REGISTER};

            if (checkPattern(tokens, i, patternA, 4)) {
                s.operands[0] = tokens.at(i+1);
                s.operands[1] = tokens.at(i+1);
                s.operands[2] = tokens.at(i+3);
                i += 4;
            }
            else if (checkPattern(tokens, i, patternB, 6)) {
                s.operands[0] = tokens.at(i+1);
                s.operands[1] = tokens.at(i+3);
                s.operands[2] = tokens.at(i+5);
                i += 6;
            }
            else {
                // Throw an error: Expected a different token
            }

            statements.push_back(s);
        }
        else {
            switch (t.type) {
                case (DATA): {
                    int patternA[4] = {DATA, MEMORY, COMMA, HEX};
                    int patternB[4] = {DATA, REGISTER, COMMA, HEX};
                    int patternC[4] = {DATA, REGISTER, COMMA, REGISTER};

                    if (checkPattern(tokens, i, patternA, 4) ||
                        checkPattern(tokens, i, patternB, 4)) {
                        s.operands[0] = tokens.at(i+1);
                        s.operands[1] = tokens.at(i+3);
                        i += 4;
                    }
                    else if (checkPattern(tokens, i, patternC, 4)) {
                        s.type = ADD;
                        s.operands[0] = tokens.at(i+1);
                        s.operands[1] = tokens.at(i+3);
                        s.operands[2] = r0();
                        i += 4;
                    }
                    else {
                        // Throw an error: Expected a different token
                    }

                    statements.push_back(s);
                    break;
                }
                case (MOV): {
                    int patternA[4] = {MOV, REGISTER, COMMA, IMMEDIATE};
                    int patternB[4] = {MOV, REGISTER, COMMA, HEX};
                    int patternC[4] = {MOV, REGISTER, COMMA, REGISTER};

                    if (checkPattern(tokens, i, patternA, 4) ||
                        checkPattern(tokens, i, patternB, 4)) {
                        s.operands[0] = tokens.at(i+1);
                        s.operands[1] = tokens.at(i+3);
                        i += 4;
                    }
                    else if (checkPattern(tokens, i, patternC, 4)) {
                        s.type = ADD;
                        s.operands[0] = tokens.at(i+1);
                        s.operands[1] = tokens.at(i+3);
                        s.operands[2] = r0();
                        i += 4;
                    }
                    else {
                        // Throw an error: Expected a different token
                    }

                    statements.push_back(s);
                    break;
                }
                case (STDIN): {
                    int pattern[2] = {STDIN, REGISTER};
                    
                    if (checkPattern(tokens, i, pattern, 2)) {
                        s.type = LDR;
                        s.operands[0] = tokens.at(i+1);
                        s.operands[1] = mFF();
                        i += 2;
                    }

                    statements.push_back(s);
                    break;
                }
                case (STDOUT): {
                    int pattern[2] = {STDOUT, REGISTER};
                    
                    if (checkPattern(tokens, i, pattern, 2)) {
                        s.type = STR;
                        s.operands[0] = tokens.at(i+1);
                        s.operands[1] = mFF();
                        i += 2;
                    }

                    statements.push_back(s);
                    break;
                }
                default: i++;
            }
        }
    }

    return statements;
}

vector<Token> preprocess(map<int, string>* mem_values, vector<Token> tokens) {
    int i = 0;
    for (i; i < tokens.size(); i++) {
        Token t = tokens.at(i);
        string instr;

        if (t.type == DATA) {
            switch (tokens.at(i+1).type) {
                case (MEMORY): {
                    int patternA[4] = {DATA, MEMORY, COMMA, HEX};
                    int patternB[4] = {DATA, MEMORY, COMMA, STRING};

                    if (checkPattern(tokens, i, patternA, 4)) {
                        string mem_locale = tokens.at(i+1).value;
                        writeMemory(mem_values, tokens.at(i+3).value, mem_locale);
                        i += 4;
                    }
                    else if (checkPattern(tokens, i, patternB, 4)) {
                        string mem_locale = tokens.at(i+1).value;
                        string str = tokens.at(i+3).value;
                        int words_needed = str.size() / 2 + str.size() % 2;

                        const char* hex_digits = "0123456789ABCDEF";

                        for (int j = 0; j < words_needed; j++) {
                            string output;
                            for (int k = 0; k < 2; k++) {
                                char c = str[j*2+k];
                                output.push_back(hex_digits[c >> 4]);
                                output.push_back(hex_digits[c & 15]);                                
                            }
                            writeMemory(mem_values, output, mem_locale, j);
                        }
                        i += 4;
                    }
                    else {
                        // Throw an error: Expected a different token
                    }
                    break;
                }
                case (CURLY_BRACE_LEFT): {
                    
                }
                default: {
                    // Throw an error: Expected a different token
                }
            }
        }
        else break;
    }

    vector<Token> new_tokens;
    for (int j = i; j < tokens.size(); j++)
    {
        new_tokens.push_back(tokens.at(j));
    }
    return new_tokens;
}

void assemble(map<int, string>* mem_values, vector<Statement> statements) {
    for (int i = 0; i < statements.size(); i++) {
        Statement s = statements.at(i);
        string instr;

        switch (s.type) {
            case (ADD): {
                instr += "1";
                instr += s.operands[0].value;
                if (s.operands[2].type == NONE) {
                    instr += s.operands[0].value;
                    instr += s.operands[1].value;
                }
                else {
                    instr += s.operands[1].value;
                    instr += s.operands[2].value;
                }

                writeMemory(mem_values, instr);
                break;
            }
            case (MOV): {
                if (s.operands[1].type == REGISTER) {
                    instr += "1";
                    instr += s.operands[0].value;
                    instr += s.operands[1].value;
                    instr += "0";
                }
                if (s.operands[1].type == HEX) {
                    instr += "7";
                    instr += s.operands[0].value;
                    instr += s.operands[1].value.substr(2, 2);
                }
                if (s.operands[1].type == IMMEDIATE) {
                    instr += "7";
                    instr += s.operands[0].value;
                    instr += convertToHex(s.operands[1].value);
                }

                writeMemory(mem_values, instr);
                break;
            }
            case (LDR): {
                instr += "8";
                instr += s.operands[0].value;
                instr += s.operands[1].value;

                writeMemory(mem_values, instr);
                break;
            }
            case (STR): {
                instr += "9";
                instr += s.operands[0].value;
                instr += s.operands[1].value;

                writeMemory(mem_values, instr);
                break;
            }
            default: {
                //writeMemory("This instruction was not properly handled");
                writeMemory(mem_values, "0000");
                break;
            }
        }
    }
}

int main(int argc, char** argv) {
    map<int, string> mem_values;

    string filename = argv[1];
    string assembly_code = readFile(filename);

    vector<Token> tokens = parse(assembly_code);
    tokens = preprocess(&mem_values, tokens);
    vector<Statement> statements = lex(tokens);
    assemble(&mem_values, statements);
    outputMemory(mem_values);

    return 0;
}
