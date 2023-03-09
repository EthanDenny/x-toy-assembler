#ifndef TYPES
#define TYPES

#include <string>

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

    UNKNOWN,

    TERMINATOR
};

static std::string TokenTypeDescriptors[] =
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

    "UNKNOWN",

    "TERMINATOR (SPECIAL)"
};

static std::string TokenTypeDescriptorsFull[] =
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

    "UNKNOWN",

    "TERMINATOR (SPECIAL)"
};

typedef struct token {
    token_type type = UNKNOWN;
    std::string value;
} Token;

typedef struct statement {
    token_type type;
    Token operands[3];
} Statement;

#endif
