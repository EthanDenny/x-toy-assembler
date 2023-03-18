#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "tokenizer.h"
#include "parser.h"
#include "token.h"
#include "common.h"
#include "preprocessor.h"

using namespace std;

string outputMemory();
string readFile(string filepath);

int main(int argc, char** argv) {
    string input_file = argv[1];
    string assembly_code = readFile(input_file);

    preprocess(&assembly_code);
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

string outputMemory() {
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
        }
    }

    return ss.str();
}

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
