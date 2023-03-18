#ifndef COMMON
#define COMMON

#include <string>

std::string decimalToHex(int num);
int hexToDecimal(std::string hex_str);

bool isWhitespace(char c);

char peek(std::string* text, int* code_index);
char consume(std::string* text, int* code_index);

std::string resolveEscapeSeqs(std::string original, int line); 

#endif
