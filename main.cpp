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
    if (argc == 1) {
        cout << "Use: ./main <filename> <mode> <full>" << endl;
        cout << "   <mode>:" << endl;
        cout << "       0: Use line numbers and newlines" << endl;
        cout << "       1: Use newlines" << endl;
        cout << "       2: Use nothing" << endl;
        cout << "   <full>:" << endl;
        cout << "       0: Use shortened token names" << endl;
        cout << "       1: Use full token names" << endl;
        return 0;
    }
    else if (argc < 4) {
        cout << "Not enough args";
        return 0;
    }

    string filename = argv[1];
    string assembly_code = readFile(filename);

    prettyLex(assembly_code, atoi(argv[2]), atoi(argv[3])==1);
}
