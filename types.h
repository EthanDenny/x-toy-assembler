#ifndef TYPES

#include <string>

using namespace std;

enum token_type {
    DATA,
    DEFINE,
    COMMENT,
    LABEL,

    REGISTER,
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
    BRANCH_REGISTER,
    BRANCH_LINK,

    STDIN,
    STDOUT,

    NEWLINE,
    SEMICOLON,
    COMMA,
    CURLY_BRACE_LEFT,
    CURLY_BRACE_RIGHT,
    WHITESPACE,

    END,

    UNKNOWN
};

string TokenTypeDescriptors[] =
{
    "DATA",
    "DEFINE",
    "// COMMENT",
    "LABEL",

    "REGISTER",
    "MEMORY",
    "IMMEDIATE",
    "HEX",
    "STRING",

    "ADD",
    "SUB",
    "AND",
    "XOR",
    "LSL",
    "LSR",

    "MOV",
    "LDR",
    "STR",

    "BRANCH",
    "BRANCH_ZERO",
    "BRANCH_POSITIVE",
    "BRANCH_REGISTER",
    "BRANCH_LINK",

    "STDIN",
    "STDOUT",

    "NEWLINE",
    ";",
    ",",
    "{",
    "}",
    "_",

    "END",

    "UNKNOWN"
};

string TokenTypeDescriptorsFull[] =
{
    "DATA",
    "DEFINE",
    "COMMENT",
    "LABEL",

    "REGISTER",
    "MEMORY",
    "IMMEDIATE",
    "HEX",
    "STRING",

    "ADD",
    "SUB",
    "AND",
    "XOR",
    "LSL",
    "LSR",

    "MOV",
    "LDR",
    "STR",

    "BRANCH",
    "BRANCH_ZERO",
    "BRANCH_POSITIVE",
    "BRANCH_REGISTER",
    "BRANCH_LINK",

    "STDIN",
    "STDOUT",

    "NEWLINE",
    "SEMICOLON",
    "COMMA",
    "CURLY_BRACE_LEFT",
    "CURLY_BRACE_RIGHT",
    "WHITESPACE",

    "END",

    "UNKNOWN"
};

typedef struct token {
    token_type type = UNKNOWN;
    string value;
} Token;

typedef struct statement {
    token_type type;
    Token operands[3];
} Statement;

#endif
