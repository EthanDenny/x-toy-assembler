#ifndef TOKENIZER
#define TOKENIZER

#include <string>
#include "token.h"

Token getNextToken(std::string* text, int* code_index, int line);

#endif
