#include <iostream>
#include <string>

using namespace std;

void throwException(string message, int line) {
    if (line > 0) {
        cout << "Line " + to_string(line) + ": " + message;
    }
    else {
        cout << message;
    }
    exit(69);
}
