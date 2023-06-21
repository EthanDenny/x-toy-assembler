#include <string>
#include <sstream>
#include <iostream>

#include "tokenizer.h"
#include "token.h"
#include "exception.h"
#include "common.h"

using namespace std;

void preprocess(string* text) {
    Token t;
    int line = 1;
    int code_index = 0;

    while (t.type != END) {
        int start_index = code_index;

        t = getNextToken(text, &code_index, line);

        if (t.type == DEFINE) {
            char C;

            if (!isWhitespace(peek(text, &code_index))) {
                throwException("Expected WHITESPACE", line);
            }
            while (isWhitespace(peek(text, &code_index))) {
                consume(text, &code_index);
            }

            stringstream findThis;
            C = consume(text, &code_index);
            while (C != ',') {
                findThis << C;
                C = consume(text, &code_index);
            }

            code_index++;

            while (isWhitespace(peek(text, &code_index))) {
                consume(text, &code_index);
            }

            stringstream replaceWith;

            C = consume(text, &code_index);
            while (C != ',' && C != ';' && C != '\n' && C != EOF) {
                replaceWith << C;
                C = consume(text, &code_index);
            }
            
            text->erase(start_index, code_index - start_index - 1);
            code_index = start_index;

            string findString = findThis.str();
            string replaceString = resolveEscapeSeqs(replaceWith.str(), line);

            size_t pos = 0;
            while (true) {
                pos = text->find(findString, pos);

                if (pos == string::npos) break;
        
                text->erase(pos, findString.length());
                text->insert(pos, replaceString);

                pos += replaceString.length();
            }
        }
        else if (t.type == NEWLINE) {
            line++;
        }
    }
}
