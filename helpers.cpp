// DO NOT MODIFY THIS FILE IN ANY WAY!

#include "helpers.hpp"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>

std::string tokenToString(Token toConvert) {
    switch (toConvert) {
        case T_ADD:
            return std::string("+");
        case T_SUB:
            return std::string("-");
        case T_MUL:
            return std::string("*");
        case T_DIV:
            return std::string("/");
        case T_SEMICOLON:
            return std::string(";");
        case T_COLON:
            return std::string(":");
        case T_LPAREN:
            return std::string("(");
        case T_RPAREN:
            return std::string(")");
        case T_QSMARK:
            return std::string("?");
        case T_INTEGER:
            return std::string("Integer");
        case T_EOL:
            return std::string("EOL");
        case T_EOF:
            return std::string("EOF");
        default:
            return std::string("Unknown");
    }
}

void mismatchError(int line, Token expected, Token found) {
    std::cerr << "Parse error: expected " << tokenToString(expected) << " found " << tokenToString(found) << " at line " << line << std::endl;
    exit(1);
}

void parseError(int line, Token found) {
    std::cerr << "Parse error: found invalid token " << tokenToString(found) << " at line " << line << std::endl;
    exit(1);
}

void scanError(int line, char badCharacter) {
    std::cerr << "Scan error: found invalid character " << badCharacter << " at line " << line << std::endl;
    exit(1);
}

void outOfBoundsError(int line, long number) {
    std::cerr << "Semantic error: number " << number << " out of bounds at line " << line << std::endl;
    exit(1);
}

void divideByZeroError(int line, long number) {
    std::cerr << "Semantic error: dividing " << number << " with zero at line " << line << std::endl;
    exit(1);
}
