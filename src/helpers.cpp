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
