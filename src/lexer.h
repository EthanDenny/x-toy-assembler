#ifndef LEXER
#define LEXER

#include <string>
#include "token.h"

Token grabToken(std::string* text, int* index, int line);

#endif
