#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "exception.h"
#include "lexer.h"
#include "types.h"

using namespace std;

string memory[256];
int global_ptr = 0x10;
int line;
int code_index = 0;

typedef struct label_hook {
    string label;
    int ptr;
}  LabelHook;

vector<LabelHook> hook_put;
vector<LabelHook> hook_get;

string convertToHex(int num) {
    stringstream ss;
    ss << hex << num;
    string imm = ss.str();
    for (auto &c: imm) c = (char) toupper(c); // Convert to uppercase
    if (imm.length() == 1) imm = "0" + imm;
    return imm;
}

int hexToInt(string hex_str) {
    int x;
    stringstream ss;
    ss << hex << hex_str;
    ss >> x;
    return x;
}

void writeMemory(string statement) {
    memory[global_ptr] = statement;
    global_ptr++;
}

void printMemory() {
    bool has_printed_const = false;
    for (int i = 0; i < 256; i++) {
        if (i == 0x10 && has_printed_const) {
            cout << endl;
        }
        if (memory[i] != "") {
            if (i < 0x10) {
                has_printed_const = true;
            }
            cout << convertToHex(i) << ": " << memory[i] << endl;

             // Add halt
            if (i >= 0x10 && memory[i + 1] == "") {
                cout << convertToHex(i + 1) << ": 0000" << endl;
            }
        }
    }
    cout << endl;
}

bool isNextToken(string* text, token_type type) {
    int old_index = code_index;
    Token t = grabToken(text, &code_index, line);
    code_index = old_index;
    return t.type == type;
}

string tryGrabToken(string* text, token_type type) {
    Token t;

    if (type == TERMINATOR) {
        if (isNextToken(text, WHITESPACE)) tryGrabToken(text, WHITESPACE);
        if (isNextToken(text, COMMENT)) tryGrabToken(text, COMMENT);

        t = grabToken(text, &code_index, line);

        if (t.type == NEWLINE) {
            line++;
        }

        if (t.type != SEMICOLON && t.type != NEWLINE && t.type != END) {
            throwException("Expected end of statement", line);
        }
    }
    else {
        t = grabToken(text, &code_index, line);

        if (t.type != type) {
            throwException("Expected " + TokenTypeDescriptorsFull[type] + " got " + TokenTypeDescriptorsFull[t.type], line);
        }
    }

    return t.value;
}

void ALStatement(string* text, string opcode) {
    string a;
    string b;
    string c;
    
    tryGrabToken(text, WHITESPACE);
    a = tryGrabToken(text, REGISTER);
    tryGrabToken(text, COMMA);
    tryGrabToken(text, WHITESPACE);
    b = tryGrabToken(text, REGISTER);

    if (isNextToken(text, COMMA)) {
        tryGrabToken(text, COMMA);
        tryGrabToken(text, WHITESPACE);
        c = tryGrabToken(text, REGISTER);
        tryGrabToken(text, TERMINATOR);
        writeMemory(opcode + a + b + c);
    }
    else {
        tryGrabToken(text, TERMINATOR);
        writeMemory(opcode + a + a + b);
    }
}

void singleRegStatement(string* text, string opcode, string addr) {
    string reg;

    tryGrabToken(text, WHITESPACE);
    reg = tryGrabToken(text, REGISTER);
    tryGrabToken(text, TERMINATOR);

    writeMemory(opcode + reg + addr);
}

void branchStatement(string* text, string opcode, string reg) {
    string label;
    string addr;

    tryGrabToken(text, WHITESPACE);
    label = tryGrabToken(text, LABEL);
    tryGrabToken(text, TERMINATOR);
    
    for(LabelHook hook : hook_get) {
        if (hook.label == label) {
            addr += convertToHex(hook.ptr);
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

void branchRegStatement(string* text, string opcode) {
    string reg;

    tryGrabToken(text, WHITESPACE);
    reg = tryGrabToken(text, REGISTER);
    tryGrabToken(text, COMMA);

    branchStatement(text, opcode, reg);
}

void writeHex(string addr, string hex) {
    int ptr = hexToInt(addr);

    if (hex.length() == 2) {
        hex = "00" + hex;
    }
    
    memory[ptr] = hex;
}

void writeString(string addr, string str) {
    int ptr = hexToInt(addr);
    int i;

    for (i = 0; i < (int) str.length(); i += 2) {
        string ch = convertToHex(str[i]);
        string ch2 = convertToHex(str[i+1]);
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

    while (t.type != END) {
        t = grabToken(text, &code_index, line);

        if (t.type == DATA) {
            string mem;
            
            tryGrabToken(text, WHITESPACE);
            mem = tryGrabToken(text, MEMORY);
            tryGrabToken(text, COMMA);
            tryGrabToken(text, WHITESPACE);
            
            if (isNextToken(text, HEX)) {
                string hex = tryGrabToken(text, HEX);
                tryGrabToken(text, TERMINATOR);
                writeHex(mem, hex);
            }
            else if (isNextToken(text, STRING)) {
                string str = tryGrabToken(text, STRING);
                tryGrabToken(text, TERMINATOR);
                writeString(mem, str);
            }
            else {
                throwException("Expected HEX or STRING", line);
            }

            // Array version of .data still needs to be added
        }
        else if (t.type == DEFINE) {
            // This is complicated to implement, and unnecessary right now
            // TL;DR: Do later
        }
        else if (t.type == LABEL) {
            tryGrabToken(text, TERMINATOR);

            for(LabelHook hook : hook_put) {
                if (hook.label == t.value) {
                    memory[hook.ptr] += convertToHex(global_ptr);
                }
            }
            
            LabelHook hook;
            hook.label = t.value;
            hook.ptr = global_ptr;

            hook_get.push_back(hook);
        }
        else if (t.type == ADD) {
            ALStatement(text, "1");
        }
        else if (t.type == SUB) {
            ALStatement(text, "2");
        }
        else if (t.type == AND) {
            ALStatement(text, "3");
        }
        else if (t.type == XOR) {
            ALStatement(text, "4");
        }
        else if (t.type == LSL) {
            ALStatement(text, "5");
        }
        else if (t.type == LSR) {
            ALStatement(text, "6");
        }
        else if (t.type == MOV) {
            string dest;
            
            tryGrabToken(text, WHITESPACE);
            dest = tryGrabToken(text, REGISTER);
            tryGrabToken(text, COMMA);
            tryGrabToken(text, WHITESPACE);

            if (isNextToken(text, REGISTER)) {
                string reg = tryGrabToken(text, REGISTER);
                tryGrabToken(text, TERMINATOR);
                
                writeMemory("1" + dest + reg + "0");
            }
            else if (isNextToken(text, IMMEDIATE)) {
                string imm = tryGrabToken(text, IMMEDIATE);
                tryGrabToken(text, TERMINATOR);
                
                imm = convertToHex(stoi(imm));
                writeMemory("7" + dest + imm);
            }
            else if (isNextToken(text, HEX)) {
                string hex = tryGrabToken(text, HEX);
                if (hex[0] != '0' || hex[1] != '0') {
                    throwException("Hex value too large for MOV", line);
                }

                tryGrabToken(text, TERMINATOR);

                writeMemory("7" + dest + hex[2] + hex[3]);
            }
            else {
                throwException("Expected REGISTER, IMMEDIATE, or HEX", line);
            }
        }
        else if (t.type == LDR) {
            tryGrabToken(text, WHITESPACE);
            string dest = tryGrabToken(text, REGISTER);
            tryGrabToken(text, COMMA);
            tryGrabToken(text, WHITESPACE);

            if (isNextToken(text, MEMORY)) { // Direct
                string src = tryGrabToken(text, MEMORY);
                tryGrabToken(text, TERMINATOR);

                writeMemory("8" + dest + src);
            }
            else if (isNextToken(text, REGISTER)) { // Indirect
                string src = tryGrabToken(text, REGISTER);
                tryGrabToken(text, TERMINATOR);

                writeMemory("A" + dest + "0" + src);
            }
        }
        else if (t.type == STR) {
            tryGrabToken(text, WHITESPACE);

            if (isNextToken(text, MEMORY)) { // Direct
                string dest = tryGrabToken(text, MEMORY);
                tryGrabToken(text, COMMA);
                tryGrabToken(text, WHITESPACE);
                string src = tryGrabToken(text, REGISTER);
                tryGrabToken(text, TERMINATOR);

                writeMemory("9" + dest + src);
            }
            else if (isNextToken(text, REGISTER)) { // Indirect
                string dest = tryGrabToken(text, REGISTER);
                tryGrabToken(text, COMMA);
                tryGrabToken(text, WHITESPACE);
                string src = tryGrabToken(text, REGISTER);
                tryGrabToken(text, TERMINATOR);

                writeMemory("B" + src + "0" + dest);
            }
        }
        else if (t.type == BRANCH) {
            branchStatement(text, "C", "0");
        }
        else if (t.type == BRANCH_ZERO) {
            branchRegStatement(text, "C");
        }
        else if (t.type == BRANCH_POSITIVE) {
            branchRegStatement(text, "D");
        }
        else if (t.type == BRANCH_REGISTER) {
            singleRegStatement(text, "E", "00");
        }
        else if (t.type == BRANCH_LINK) {
            string reg;
            string mem;

            tryGrabToken(text, WHITESPACE);
            reg = tryGrabToken(text, REGISTER);
            tryGrabToken(text, COMMA);
            tryGrabToken(text, WHITESPACE);
            mem = tryGrabToken(text, MEMORY);
            tryGrabToken(text, TERMINATOR);

            writeMemory("F" + reg + mem);
        }
        else if (t.type == STDIN) {
            singleRegStatement(text, "8", "FF");
        }
        else if (t.type == STDOUT) {
            singleRegStatement(text, "9", "FF");
        }
        else if (t.type == NEWLINE) {
            line++;
        }
        else if (t.type != COMMENT && t.type != WHITESPACE) {
            // Throw an error: Unexpected token
        }
    }

    printMemory();
}
