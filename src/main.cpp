#include <iostream>
#include <fstream>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "types.h"
#include "common.h"

using namespace std;

string readFile(string filepath) {
    ifstream file(filepath);

    string content;
    char ch;

    while (file.get(ch)) {
        content += ch;
    }

    file.close();

    return content;
}

void printMemory() {
    string* memory = getMemory();

    bool has_printed_const = false;
    for (int i = 0; i < 256; i++) {
        if (i == 0x10 && has_printed_const) {
            cout << endl;
        }
        if (memory[i] != "") {
            if (i < 0x10) {
                has_printed_const = true;
            }
            cout << decimalToHex(i) << ": " << memory[i] << endl;

             // Add halt
            if (i >= 0x10 && memory[i + 1] == "") {
                cout << decimalToHex(i + 1) << ": 0000" << endl;
            }
        }
    }
}

void storeMemory(string filepath) {
    string* memory = getMemory();
    ofstream file(filepath);

    bool has_printed_const = false;
    for (int i = 0; i < 256; i++) {
        if (i == 0x10 && has_printed_const) {
            file << endl;
        }
        if (memory[i] != "") {
            if (i < 0x10) {
                has_printed_const = true;
            }
            file << decimalToHex(i) << ": " << memory[i] << endl;

             // Add halt
            if (i >= 0x10 && memory[i + 1] == "") {
                file << decimalToHex(i + 1) << ": 0000" << endl;
            }
        }
    }

    file.close();
}

int main(int argc, char** argv) {
    string input_file = argv[1];
    string assembly_code = readFile(input_file);
    parse(&assembly_code);

    if (argc == 2) {
        printMemory();
    }
    else {
        storeMemory(argv[2]);
    }
}
