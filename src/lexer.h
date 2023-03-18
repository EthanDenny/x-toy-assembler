#ifndef LEXER
#define LEXER

#include <string>
#include "token.h"

Token getNextToken(std::string* text, int* index, int line);

#endif
