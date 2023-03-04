#ifndef LEXER
#define LEXER

#include <string>
#include "types.h"

Token grabToken(std::string* text, int line);

void prettyLex(std::string text, int mode);
void prettyLex(std::string text, bool use_full);
void prettyLex(std::string text, int mode, bool use_full);

void testFullLex(bool use_full);

#endif
