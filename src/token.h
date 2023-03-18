#ifndef TOKEN
#define TOKEN

#include <string>

typedef enum token_type {
    DATA,
    DEFINE,
    COMMENT,
    LABEL,
    HALT,

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

    TERMINATOR,

} TokenType;

static std::string TokenTypeDescriptors[] = {
    "DATA",
    "DEFINE",
    "COMMENT",
    "LABEL",
    "HALT",

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
    TokenType type = UNKNOWN;
    std::string value;
} Token;

#endif
