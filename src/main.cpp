#include <iostream>
#include <fstream>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "types.h"

using namespace std;

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
    string filename = argv[1];
    string assembly_code = readFile(filename);

    parse(&assembly_code);
}
