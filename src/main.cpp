#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "lexer.h"
#include "parser.h"
#include "token.h"
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

string outputMemory() {
    string* memory = getMemory();
    stringstream ss;

    bool has_printed_const = false;
    for (int i = 0; i < 256; i++) {
        if (i == 0x10 && has_printed_const) {
            ss << endl;
        }
        if (memory[i] != "") {
            if (i < 0x10) {
                has_printed_const = true;
            }
            ss << decimalToHex(i) << ": " << memory[i] << endl;

             // Add halt
            if (i >= 0x10 && memory[i + 1] == "") {
                ss << decimalToHex(i + 1) << ": 0000" << endl;
            }
        }
    }

    return ss.str();
}

int main(int argc, char** argv) {
    string input_file = argv[1];
    string assembly_code = readFile(input_file);
    parse(&assembly_code);

    if (argc == 2) {
        cout << outputMemory();
    }
    else {
        ofstream file(argv[2]);
        file << outputMemory();
        file.close();
    }
}
