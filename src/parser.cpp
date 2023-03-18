#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "exception.h"
#include "lexer.h"
#include "token.h"
#include "common.h"

using namespace std;

string memory[256];
int global_ptr = 0x10;
int line;
int code_index;

typedef struct label_hook {
    string label;
    int ptr;
}  LabelHook;

vector<LabelHook> hook_put;
vector<LabelHook> hook_get;

void skipNextTokenIf(string* text, TokenType type);

string* getMemory(void) {
    return memory;
}

void writeMemory(string statement) {
    memory[global_ptr] = statement;
    global_ptr++;
}

bool isNextToken(string* text, TokenType type) {
    int old_index = code_index;
    Token t = getNextToken(text, &code_index, line);
    code_index = old_index;
    return t.type == type;
}

string tryGetToken(string* text, TokenType type) {
    Token t;

    if (type == TERMINATOR) {
        skipNextTokenIf(text, WHITESPACE);
        skipNextTokenIf(text, COMMENT);

        t = getNextToken(text, &code_index, line);

        if (t.type == NEWLINE) {
            line++;
        }

        if (t.type != SEMICOLON && t.type != NEWLINE && t.type != END) {
            throwException("Expected end of statement", line);
        }
    }
    else {
        t = getNextToken(text, &code_index, line);

        if (t.type != type) {
            throwException("Expected " + TokenTypeDescriptors[type] + " got " + TokenTypeDescriptors[t.type], line);
        }
    }

    return t.value;
}

void skipNextTokenIf(string* text, TokenType type) {
    if (isNextToken(text, type)) tryGetToken(text, type);
}

void writeALStatement(string* text, string opcode) {
    string a;
    string b;
    string c;
    
    tryGetToken(text, WHITESPACE);
    a = tryGetToken(text, REGISTER);
    tryGetToken(text, COMMA);
    tryGetToken(text, WHITESPACE);
    b = tryGetToken(text, REGISTER);

    if (isNextToken(text, COMMA)) {
        tryGetToken(text, COMMA);
        tryGetToken(text, WHITESPACE);
        c = tryGetToken(text, REGISTER);
        tryGetToken(text, TERMINATOR);
        writeMemory(opcode + a + b + c);
    }
    else {
        tryGetToken(text, TERMINATOR);
        writeMemory(opcode + a + a + b);
    }
}

void writeSingleRegStatement(string* text, string opcode, string addr) {
    string reg;

    tryGetToken(text, WHITESPACE);
    reg = tryGetToken(text, REGISTER);
    tryGetToken(text, TERMINATOR);

    writeMemory(opcode + reg + addr);
}

void writeBranchStatement(string* text, string opcode, string reg) {
    string label;
    string addr;

    tryGetToken(text, WHITESPACE);
    label = tryGetToken(text, LABEL);
    tryGetToken(text, TERMINATOR);
    
    for(LabelHook hook : hook_get) {
        if (hook.label == label) {
            addr += decimalToHex(hook.ptr);
        }
    }
    
    if (addr == "") { // Did not find label
        LabelHook hook;
        hook.label = label;
        hook.ptr = global_ptr;

        hook_put.push_back(hook);
    }

    writeMemory(opcode + reg + addr);
}

void writeBranchRegStatement(string* text, string opcode) {
    string reg;

    tryGetToken(text, WHITESPACE);
    reg = tryGetToken(text, REGISTER);
    tryGetToken(text, COMMA);

    writeBranchStatement(text, opcode, reg);
}

void writeHex(string addr, string hex) {
    int ptr = hexToDecimal(addr);

    if (hex.length() == 2) {
        hex = "00" + hex;
    }
    
    memory[ptr] = hex;
}

void writeString(string addr, string str) {
    int ptr = hexToDecimal(addr);
    int i;

    for (i = 0; i < (int) str.length(); i += 2) {
        string ch = decimalToHex(str[i]);
        string ch2 = decimalToHex(str[i+1]);
        memory[ptr] = ch + ch2;
        ptr++;
    }

    if (i == (int) str.length()) {
        memory[ptr] = "0000";
    }
}

void parse(string* text) {
    Token t;
    line = 1;
    code_index = 0;

    while (t.type != END) {
        t = getNextToken(text, &code_index, line);

        if (t.type == DATA) {
            string mem;
            
            tryGetToken(text, WHITESPACE);
            mem = tryGetToken(text, MEMORY);
            tryGetToken(text, COMMA);
            tryGetToken(text, WHITESPACE);
            
            if (isNextToken(text, HEX)) {
                string hex = tryGetToken(text, HEX);
                tryGetToken(text, TERMINATOR);
                writeHex(mem, hex);
            }
            else if (isNextToken(text, STRING)) {
                string str = tryGetToken(text, STRING);
                tryGetToken(text, TERMINATOR);
                writeString(mem, str);
            }
            else if (isNextToken(text, CURLY_BRACE_LEFT)) {
                tryGetToken(text, CURLY_BRACE_LEFT);
                for (int ptr = hexToDecimal(mem); ptr < 256; ptr++) {
                    skipNextTokenIf(text, WHITESPACE);
                    string hex = tryGetToken(text, HEX);
                    memory[ptr] = hex;
                    if (isNextToken(text, CURLY_BRACE_RIGHT)) {
                        tryGetToken(text, CURLY_BRACE_RIGHT);
                        tryGetToken(text, TERMINATOR);
                        break;
                    }
                    else {
                        tryGetToken(text, COMMA);
                    }
                }
            }
            else {
                throwException("Expected HEX, STRING, or array of HEX", line);
            }
        }
        else if (t.type == LABEL) {
            tryGetToken(text, TERMINATOR);

            for(LabelHook hook : hook_put) {
                if (hook.label == t.value) {
                    memory[hook.ptr] += decimalToHex(global_ptr);
                }
            }
            
            LabelHook hook;
            hook.label = t.value;
            hook.ptr = global_ptr;

            hook_get.push_back(hook);
        }
        else if (t.type == HALT) {
            writeMemory("0000");
        }
        else if (t.type == ADD) {
            writeALStatement(text, "1");
        }
        else if (t.type == SUB) {
            writeALStatement(text, "2");
        }
        else if (t.type == AND) {
            writeALStatement(text, "3");
        }
        else if (t.type == XOR) {
            writeALStatement(text, "4");
        }
        else if (t.type == LSL) {
            writeALStatement(text, "5");
        }
        else if (t.type == LSR) {
            writeALStatement(text, "6");
        }
        else if (t.type == MOV) {
            string dest;
            
            tryGetToken(text, WHITESPACE);
            dest = tryGetToken(text, REGISTER);
            tryGetToken(text, COMMA);
            tryGetToken(text, WHITESPACE);

            if (isNextToken(text, REGISTER)) {
                string reg = tryGetToken(text, REGISTER);
                tryGetToken(text, TERMINATOR);
                
                writeMemory("1" + dest + reg + "0");
            }
            else if (isNextToken(text, IMMEDIATE)) {
                string imm = tryGetToken(text, IMMEDIATE);
                tryGetToken(text, TERMINATOR);
                
                imm = decimalToHex(stoi(imm));
                writeMemory("7" + dest + imm);
            }
            else if (isNextToken(text, HEX)) {
                string hex = tryGetToken(text, HEX);
                if (hex[0] != '0' || hex[1] != '0') {
                    throwException("Hex value too large for MOV", line);
                }

                tryGetToken(text, TERMINATOR);

                writeMemory("7" + dest + hex[2] + hex[3]);
            }
            else {
                throwException("Expected REGISTER, IMMEDIATE, or HEX", line);
            }
        }
        else if (t.type == LDR) {
            tryGetToken(text, WHITESPACE);
            string dest = tryGetToken(text, REGISTER);
            tryGetToken(text, COMMA);
            tryGetToken(text, WHITESPACE);

            if (isNextToken(text, MEMORY)) { // Direct
                string src = tryGetToken(text, MEMORY);
                tryGetToken(text, TERMINATOR);

                writeMemory("8" + dest + src);
            }
            else if (isNextToken(text, REGISTER)) { // Indirect
                string src = tryGetToken(text, REGISTER);
                tryGetToken(text, TERMINATOR);

                writeMemory("A" + dest + "0" + src);
            }
        }
        else if (t.type == STR) {
            tryGetToken(text, WHITESPACE);

            if (isNextToken(text, MEMORY)) { // Direct
                string dest = tryGetToken(text, MEMORY);
                tryGetToken(text, COMMA);
                tryGetToken(text, WHITESPACE);
                string src = tryGetToken(text, REGISTER);
                tryGetToken(text, TERMINATOR);

                writeMemory("9" + dest + src);
            }
            else if (isNextToken(text, REGISTER)) { // Indirect
                string dest = tryGetToken(text, REGISTER);
                tryGetToken(text, COMMA);
                tryGetToken(text, WHITESPACE);
                string src = tryGetToken(text, REGISTER);
                tryGetToken(text, TERMINATOR);

                writeMemory("B" + src + "0" + dest);
            }
        }
        else if (t.type == BRANCH) {
            writeBranchStatement(text, "C", "0");
        }
        else if (t.type == BRANCH_ZERO) {
            writeBranchRegStatement(text, "C");
        }
        else if (t.type == BRANCH_POSITIVE) {
            writeBranchRegStatement(text, "D");
        }
        else if (t.type == BRANCH_REGISTER) {
            writeSingleRegStatement(text, "E", "00");
        }
        else if (t.type == BRANCH_LINK) {
            string reg;
            string mem;

            tryGetToken(text, WHITESPACE);
            reg = tryGetToken(text, REGISTER);
            tryGetToken(text, COMMA);
            tryGetToken(text, WHITESPACE);
            mem = tryGetToken(text, MEMORY);
            tryGetToken(text, TERMINATOR);

            writeMemory("F" + reg + mem);
        }
        else if (t.type == STDIN) {
            writeSingleRegStatement(text, "8", "FF");
        }
        else if (t.type == STDOUT) {
            writeSingleRegStatement(text, "9", "FF");
        }
        else if (t.type == NEWLINE) {
            line++;
        }
        else if (t.type != COMMENT && t.type != WHITESPACE) {
            // Throw an error: Unexpected token
        }
    }

    writeMemory("0000");
}
