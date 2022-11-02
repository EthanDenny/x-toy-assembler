#ifndef TYPES

#include <string>

using namespace std;

enum token_type {
    DATA,
    COMMENT,

    REGISTER,
    LABEL,
    MEMORY,
    IMMEDIATE,
    HEX,
    STRING,

    ADD,
    SUB,
    AND,
    XOR,
    LSL,
    LSR,

    MOV,

    LDR,
    STR,

    BRANCH,
    BRANCH_ZERO,
    BRANCH_POSITIVE,
    BRANCH_LINK,

    STDIN,
    STDOUT,

    NEWLINE,
    SEMICOLON,
    COLON,
    COMMA,
    CURLY_BRACE_LEFT,
    CURLY_BRACE_RIGHT,
    WHITESPACE,
    NONE
};

typedef struct {
    token_type type = NONE;
    string value = "";
} Token;

typedef struct {
    token_type type;
    Token operands[3];
} Statement;

#endif
