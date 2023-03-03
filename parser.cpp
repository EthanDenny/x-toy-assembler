#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "types.h"

bool isAlpha(char c) {
    return c == '_' || isalnum(c);
}

char getChar(string* text, int i) {
    if ((*text).length() > 0) {
        return (*text)[0];
    }
    else {
        return EOF;
    }
}

char consume(string* text) {
    char ch = getChar(text, 0);
    
    if ((*text).length() > 1) {
        *text = (*text).substr(1, (*text).length());
    }
    else {
        *text = "";
    }

    return ch;
}

bool tryConsume(string* text, string sub) {
    if ((*text).rfind(sub, 0) == 0) {
        *text = (*text).substr(sub.length(), (*text).length() - sub.length());
        return true;
    }
    else {
        return false;
    }
}

char peek(string* text) {
    return getChar(text, 0);
}

char peekNext(string* text) {
    return getChar(text, 1);
}

Token grabToken(string* text) {
    Token t;

    if (text->length() == 0) {
        t.type = END;
    }
    else if (tryConsume(text, ":")) {
        t.type = COLON;
    }
    else if (tryConsume(text, ";")) {
        t.type = SEMICOLON;
    }
    else if (tryConsume(text, ",")) {
        t.type = COMMA;
    }
    else if (tryConsume(text, "{")) {
        t.type = CURLY_BRACE_LEFT;
    }
    else if (tryConsume(text, "}")) {
        t.type = CURLY_BRACE_RIGHT;
    }
    else if (tryConsume(text, ".data")) {
        t.type = DATA;;
    }
    else if (tryConsume(text, ".define")) {
        t.type = DEFINE;
    }
    else if (tryConsume(text, "\n")) {
        t.type = NEWLINE;
    }
    else if (tryConsume(text, "add")) {
        t.type = ADD;
    }
    else if (tryConsume(text, "and")) {
        t.type = AND;
    }
    else if (tryConsume(text, "bz")) {
        t.type = BRANCH_ZERO;
    }
    else if (tryConsume(text, "bp")) {
        t.type = BRANCH_LINK;
    }
    else if (tryConsume(text, "br")) {
        t.type = BRANCH_REGISTER;
    }
    else if (tryConsume(text, "bl")) {
        t.type = BRANCH_POSITIVE;
    }
    else if (tryConsume(text, "b")) {
        t.type = BRANCH;
    }
    else if (tryConsume(text, "lsl")) {
        t.type = LSL;
    }
    else if (tryConsume(text, "lsr")) {
        t.type = LSR;
    }
    else if (tryConsume(text, "ldr")) {
        t.type = LDR;
    }
    else if (tryConsume(text, "mov")) {
        t.type = MOV;
    }
    else if (tryConsume(text, "sub")) {
        t.type = SUB;
    }
    else if (tryConsume(text, "str")) {
        t.type = STR;
    }
    else if (tryConsume(text, "stdin")) {
        t.type = STDIN;
    }
    else if (tryConsume(text, "stdout")) {
        t.type = STDOUT;
    }
    else if (tryConsume(text, "xor")) {
        t.type = XOR;
    }
    else {
        char c = consume(text);
        switch (c) {
            case ' ': {
                t.type = WHITESPACE;
                while (tryConsume(text, " ") || tryConsume(text, "\t")) {}
                break;
            }
            case '\t': {
                t.type = WHITESPACE;
                while (tryConsume(text, " ") || tryConsume(text, "\t")) {}
                break;
            }
            case '/': {
                if (peek(text) == '/') {
                    t.type = COMMENT;
                    char C = '/';
                    do {
                        C = consume(text);
                    } while (C != '\n' && C != EOF);
                    break;
                }
                if (peek(text) == '*') {
                    t.type = COMMENT;
                    char C = '/';
                    do {
                        C = consume(text);
                        if (C == '*' && tryConsume(text, "/")) {
                            break;
                        }
                    } while (C != EOF);
                    break;
                }
                break;
                // Throw an exception here, expected a comment
            }
            case '"': {
                t.type = STRING;
                bool special = false;
                char C = consume(text);
                do {
                    if (!special) {
                        t.value += C;
                    }
                    else {
                        switch (C) {
                        case '0': t.value += '\0';
                        }
                    }
                    C = consume(text);
                    special = C == '\\';
                    if (special) {
                        C = consume(text);
                    }
                } while (C != '"' and C != EOF);

                if (C == EOF) {
                    // Throw an exception here, expected closing quotes

                    t.type = ANY;
                }

                break;
                // Throw an exception here, expected a comment
            }
            case 'm': {
                if (isxdigit(peek(text)) && isxdigit(peekNext(text))) {
                    t.type = MEMORY;
                    t.value += consume(text);
                    t.value += consume(text);
                    break;
                }
            }
            case '#': {
                t.type = IMMEDIATE;
                if (!isdigit(peek(text))) {
                    // Throw an exception here: Expected a digit
                }
                do {
                    t.value += consume(text);
                } while (isdigit(peek(text)));
                break;
            }
            case 'r': {
                if (isxdigit(peek(text))) {
                    t.type = REGISTER;
                    t.value = consume(text);
                    break;
                }
            }
            case '0': {
                if (tryConsume(text, "x")) {
                    t.type = HEX;

                    int i = 0;
                    char C = consume(text);
                    if (!isxdigit(C)) {
                        // Throw an exception here: Expected a hex digit
                    }

                    while (i < 4 && isxdigit(C)) {
                        t.value += C;
                        C = consume(text);
                        i++;
                    }

                    for (int j = 0; j < 4 - i; j++) {
                        t.value = '0' + t.value;
                    }

                    break;
                }
            }
            default: {
                if (isAlpha(c)) {
                    t.type = ANY;
                    t.value = c;
                    while (isAlpha(peek(text))) {
                        t.value += consume(text);
                    }
                }

                // Throw an exception here: Unknown token
            }
        }
    }

    return t;
}

vector<Token> parse(string text) {
    vector<Token> tokens;

    while (true) {
        Token nextToken = grabToken(&text);
        
        if (nextToken.type != END) {
            tokens.push_back(nextToken);
        }

        if (nextToken.type == END) {
            break;
        }
    }

    return tokens;
}

void prettyParse(string text) {
    vector<Token> tokens = parse(text);

    int j = 0;

    cout << "0: ";

    for (int i = 0; i < (int) tokens.size(); i++) {
        cout << TokenTypeDescriptors[tokens[i].type] << " ";

        if (tokens[i].type == NEWLINE) {
            j++;
            cout << "\n" << j << ": ";
        }
    }
    cout << "\n";
}

void fullParse() {
    prettyParse("this_could_be_anything");

    cout << endl;

    prettyParse(".data");
    prettyParse(".define");
    prettyParse("// Hello, world");

    cout << endl;

    prettyParse("rF");
    prettyParse("m0F");
    prettyParse("#1");
    prettyParse("0x00FF");
    prettyParse("\"Hello, world\"");

     cout << endl;

    prettyParse("add");
    prettyParse("sub");
    prettyParse("and");
    prettyParse("xor");
    prettyParse("lsl");
    prettyParse("lsr");

    cout << endl;

    prettyParse("mov");
    prettyParse("ldr");
    prettyParse("str");

    cout << endl;

    prettyParse("b");
    prettyParse("bz");
    prettyParse("bp");
    prettyParse("br");
    prettyParse("bl");

    cout << endl;

    prettyParse("stdin");
    prettyParse("stdout");

    cout << endl;

    prettyParse("\n");
    prettyParse(";");
    prettyParse(":");
    prettyParse(",");
    prettyParse("{");
    prettyParse("}");
}

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

    prettyParse(assembly_code);

    return 0;
}
